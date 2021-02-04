#include "FontRuntime.h"

#include "FontDefinition.h"
#include "FontCache.h"

#include "Common/Logger.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Components/Storage/StorageManager.h"
#include "Components/Storage/ProjectDirectory.h"
#include "Components/Storage/File.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Project/Project.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using std::make_shared;
using std::max;


/**
 * https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02
 */
namespace octronic::dream
{
    FontRuntime::FontRuntime(FontDefinition* fd, ProjectRuntime* er)
        : SharedAssetRuntime("FontRuntime",fd, er),
          mAtlasTexture(0),
          mAtlasWidth(0),
          mAtlasHeight(0),
          mSize(0),
          mVao(0),
          mVbo(0),
          mFontConstructionTask(this),
          mFontDestructionTask(nullptr),
          mFontFile(nullptr)
    {
        LOG_TRACE("FontRuntime: {}", __FUNCTION__);
        memset(&mCharacterInfo, 0, sizeof(FontCharacterInfo)*CHAR_INFO_SZ);
    }

    FontRuntime::~FontRuntime()
    {
        LOG_TRACE("FontRuntime: {}", __FUNCTION__);
        mFontDestructionTask = make_shared<FontDestructionTask>();
        mFontDestructionTask->setFontAtlasTexture(mAtlasTexture);
        mFontDestructionTask->setFontVao(mVao);
        mFontDestructionTask->setFontVbo(mVbo);
        mProjectRuntime->getGraphicsComponent()->pushDestructionTask(mFontDestructionTask);
    }

    void FontRuntime::pushConstructionTask()
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_TRACE("FontRuntime: {}", __FUNCTION__);
            mProjectRuntime->getGraphicsComponent()->pushTask(&mFontConstructionTask);
        } dreamElseLockFailed
    }

    bool FontRuntime::useDefinition()
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_TRACE("FontRuntime: {}", __FUNCTION__);
            FontDefinition* fontDef = static_cast<FontDefinition*>(mDefinition);

            mSize = fontDef->getSize();

            string filename = mProjectRuntime->getProject()->getDirectory()->getAssetAbsolutePath(fontDef);
            // Open File
            StorageManager* sm = mProjectRuntime->getStorageManager();
            File* fontFile = sm->openFile(filename);
            if (!fontFile->exists())
            {
                LOG_ERROR("FontCache: Font file {} does not exist", filename);
                sm->closeFile(fontFile);
                fontFile = nullptr;
                return false;
            }

            // Read File
            LOG_DEBUG("FontCache: Loading font: {}",filename);

            if (!fontFile->readBinary())
            {
                LOG_ERROR("FontCache: Unable to read file data");
                sm->closeFile(fontFile);
                fontFile = nullptr;
                return false;
            }

            uint8_t* buffer = fontFile->getBinaryData();
            size_t buffer_sz = fontFile->getBinaryDataSize();

            // Set Runtime Parameters
            setFontFile(fontFile);
            // Push back construction task
            pushConstructionTask();

            // Cache FontRuntime
            fontFile = nullptr;

            return true;
        } dreamElseLockFailed
    }

    int FontRuntime::getSize() const
    {
        return mSize;
    }

    void FontRuntime::setSize(int size)
    {
        if(dreamTryLock())
        {
            dreamLock();
            mSize = size;
        } dreamElseLockFailed
    }

    float FontRuntime::getWidthOf(string s)
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_TRACE("FontRuntime: {} {}", __FUNCTION__,s);
            float width = 0.f;
            for (string::iterator it = s.begin(); it != s.end(); it++)
            {
                width += mCharacterInfo[*it].ax;
            }
            return width/64.f;
        } dreamElseLockFailed
    }

    void FontRuntime::draw(EntityRuntime* entity)
    {
        if(dreamTryLock())
        {
            dreamLock();
            LOG_TRACE("FontRuntime: {}", __FUNCTION__);
            if (mFontConstructionTask.getState() != TaskState::TASK_STATE_COMPLETED)
            {
                LOG_ERROR("FontRuntime: tried to draw before construction task was completed");
                assert(false);
            }

            if (entity == nullptr)
            {
                LOG_ERROR("FontRuntime: tried to draw with null entity");
                assert(false);
            }

            float sx = entity->getFontScale();
            float sy = sx;
            Transform tx = entity->getTransform();
            Vector3 translation = tx.getTranslation();
            float x = translation.x();
            float y = translation.y();
            string text = entity->getFontText();

            glActiveTexture(GL_TEXTURE0);
            GLCheckError();

            glBindTexture(GL_TEXTURE_2D, mAtlasTexture);
            GLCheckError();

            glBindVertexArray(mVao);
            GLCheckError();

            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            GLCheckError();

            unsigned int num_points = 6 * text.size();

            LOG_TRACE("FontRuntime: Generating for string length {}, {} points {} triangles",
                      text.size(), num_points, num_points/6);

            struct point
            {
                GLfloat x;
                GLfloat y;
                GLfloat s;
                GLfloat t;
            } coords[num_points];

            int n = 0;
            const char* c_text = text.c_str();

            for(const char *p = c_text; *p; p++)
            {
                float x2 =  x + mCharacterInfo[*p].bl * sx;
                float y2 = -y - mCharacterInfo[*p].bt * sy;
                float w = mCharacterInfo[*p].bw * sx;
                float h = mCharacterInfo[*p].bh * sy;

                /* Advance the cursor to the start of the next character */
                x += mCharacterInfo[*p].ax * sx;
                y += mCharacterInfo[*p].ay * sy;

                /* Skip glyphs that have no pixels */
                if(!w || !h) continue;

                coords[n++] = (point){x2, -y2, mCharacterInfo[*p].tx, 0};
                coords[n++] = (point){x2 + w, -y2, mCharacterInfo[*p].tx + mCharacterInfo[*p].bw / mAtlasWidth, 0};
        		coords[n++] = (point){x2, -y2 - h, mCharacterInfo[*p].tx, mCharacterInfo[*p].bh / mAtlasHeight}; //remember: each glyph occupies a different amount of vertical space
    			coords[n++] = (point){x2 + w, -y2, mCharacterInfo[*p].tx + mCharacterInfo[*p].bw / mAtlasWidth, 0};
				coords[n++] = (point){x2, -y2 - h, mCharacterInfo[*p].tx, mCharacterInfo[*p].bh / mAtlasHeight};
				coords[n++] = (point){x2 + w, -y2 - h, mCharacterInfo[*p].tx + mCharacterInfo[*p].bw / mAtlasWidth, mCharacterInfo[*p].bh / mAtlasHeight};
			}

			LOG_TRACE("Font: Buffering {}bytes ({}points) triangle points ({} tris)", sizeof(coords), sizeof(coords)/sizeof(point), n);

			glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords[0], GL_DYNAMIC_DRAW);
			GLCheckError();
			glDrawArrays(GL_TRIANGLES, 0, n);
			GLCheckError();
			glBindVertexArray(0);
        }
        dreamElseLockFailed
	}

	void FontRuntime::setFontFile(File* file)
	{
		if(dreamTryLock())
		{
			dreamLock();
			mFontFile = file;
		} dreamElseLockFailed
	}

	File* FontRuntime::getFontFile()
	const
	{
		return mFontFile;
	}

	FontCharacterInfo* FontRuntime::getCharacterInfo()
	{
		return &mCharacterInfo[0];
	}

	void FontRuntime::pushInstance(EntityRuntime* entityRuntime)
	{
		if(dreamTryLock())
		{
			dreamLock();
			LOG_TRACE("FontRuntime: {}", __FUNCTION__);
			auto itr = std::find(mInstances.begin(), mInstances.end(),entityRuntime);

			if (itr == mInstances.end())
			{
				mInstances.push_back(entityRuntime);
			}
		} dreamElseLockFailed
	}

	void FontRuntime::popInstance(EntityRuntime* entityRuntime)
	{
		if(dreamTryLock())
		{
			dreamLock();
			LOG_TRACE("FontRuntime: {}", __FUNCTION__);
			auto itr = std::find(mInstances.begin(), mInstances.end(), entityRuntime);

			if (itr != mInstances.end())
			{
				mInstances.erase(itr);
			}
		} dreamElseLockFailed
	}

	vector<EntityRuntime*>& FontRuntime::getInstanceVector()
	{
		if(dreamTryLock())
		{
			dreamLock();
			return mInstances;
		} dreamElseLockFailed
	}

	void FontRuntime::setVao(GLuint vao)
	{
		if(dreamTryLock())
		{
			dreamLock();
			mVao = vao;
		} dreamElseLockFailed
	}
	void FontRuntime::setVbo(GLuint vbo)
	{
		if(dreamTryLock())
		{
			dreamLock();
			mVbo = vbo;
		} dreamElseLockFailed
	}

	void FontRuntime::setAtlasTexture(GLuint atlasTexture)
	{
		if(dreamTryLock())
		{
			dreamLock();
			mAtlasTexture = atlasTexture;
		} dreamElseLockFailed
	}

	void FontRuntime::setAtlasWidth(unsigned int atlasWidth)
	{
		if(dreamTryLock())
		{
			dreamLock();
			mAtlasWidth = atlasWidth;
		} dreamElseLockFailed
	}

	void FontRuntime::setAtlasHeight(unsigned int atlasHeight)
	{
		if(dreamTryLock())
		{
			dreamLock();
			mAtlasHeight = atlasHeight;
		} dreamElseLockFailed
	}


	GLuint FontRuntime::getAtlasTexture() const
	{
		return mAtlasTexture;
	}

	unsigned int FontRuntime::getAtlasWidth() const
	{
		return mAtlasWidth;
	}

	unsigned int FontRuntime::getAtlasHeight() const
	{
		return mAtlasHeight;
	}

	bool FontRuntime::loadIntoGL()
	{
		if(dreamTryLock()) {
			dreamLock();
			// VAO
			glGenVertexArrays(1,&mVao);
			GLCheckError();
			glBindVertexArray(mVao);
			GLCheckError();

			// VBO
			glGenBuffers(1,&mVbo);
			GLCheckError();
			glBindBuffer(GL_ARRAY_BUFFER,mVbo);
			GLCheckError();

			// Vertex Position Attributes
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
			GLCheckError();
			glEnableVertexAttribArray(0);
			GLCheckError();

			// Unbind for now
			glBindVertexArray(0);
			GLCheckError();

			FT_Face face = NULL;

			FontCache* fontCache = getProjectRuntime()->getFontCache();
			FT_Library ft_lib = fontCache->getFreeTypeLibrary();

			StorageManager* sm = getProjectRuntime()->getStorageManager();

			// Already read the data in
			void* font_data = mFontFile->getBinaryData();
			size_t font_data_sz = mFontFile->getBinaryDataSize();

			if (!font_data || font_data_sz == 0)
			{
				LOG_ERROR("FontConstructionTask: Font file was not read successfully");
				sm->closeFile(mFontFile);
				mFontFile = nullptr;
				return false;
			}

			FT_Error ft_error = FT_New_Memory_Face(ft_lib,static_cast<FT_Byte*>(font_data),font_data_sz, 0, &face );

			if(ft_error)
			{
				LOG_ERROR("FontManager: Unable to create font face: {}", FontCache::getFreetypeErrorMessage(ft_error));
				return false;
			}

			assert(mSize > 0.f);

			FT_Set_Pixel_Sizes(face, 0, mSize);

			if(ft_error)
			{
				LOG_ERROR("FontManager: Unable to create font face: {}", FontCache::getFreetypeErrorMessage(ft_error));
				return false;
			}

			FT_GlyphSlot g = face->glyph;
			unsigned int w = 0;
			unsigned int h = 0;

			for(int i = 32; i < 128; i++)
			{
				if(FT_Load_Char(face, i, FT_LOAD_RENDER))
				{
					LOG_ERROR("Font: Loading character %c failed!\n", i);
					continue;
				}

				w += g->bitmap.width;
				h = max(h, g->bitmap.rows);
			}

			/* you might as well save this value as it is needed later on */
			mAtlasWidth = w;
			mAtlasHeight = h;

			glGenTextures(1, &mAtlasTexture);
			GLCheckError();

			glActiveTexture(GL_TEXTURE0);
			GLCheckError();

			glBindTexture(GL_TEXTURE_2D, mAtlasTexture);
			GLCheckError();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			GLCheckError();

	#if defined (GL_ES_VERSION_3_0)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);
			GLCheckError();
	#else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
			GLCheckError();
	#endif

			/* Clamping to edges is important to prevent artifacts when scaling */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			GLCheckError();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			GLCheckError();

			/* Linear filtering usually looks best for text */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			GLCheckError();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			GLCheckError();

			// Begin loading glyphs

			int x = 0;

			for(int i = 32; i < 128; i++)
			{
				if(FT_Load_Char(face, i, FT_LOAD_RENDER))
				{
					continue;
				}

	#if defined (GL_ES_VERSION_3_0)
				glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
	#else
				glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
				GLCheckError();
	#endif

				mCharacterInfo[i].ax = g->advance.x >> 6;
				mCharacterInfo[i].ay = g->advance.y >> 6;
				mCharacterInfo[i].bw = g->bitmap.width;
				mCharacterInfo[i].bh = g->bitmap.rows;
				mCharacterInfo[i].bl = g->bitmap_left;
				mCharacterInfo[i].bt = g->bitmap_top;
				mCharacterInfo[i].tx = (float)x / w;

				x += g->bitmap.width;
			}

			FT_Done_Face(face);
			sm->closeFile(mFontFile);
			return true;

		} dreamElseLockFailed
	}
}
