#include "FontRuntime.h"

#include "Common/Logger.h"
#include "FontDefinition.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Components/Graphics/GraphicsComponent.h"

using std::make_shared;

/**
 * https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02
 */

namespace octronic::dream
{
	FontRuntime::FontRuntime(FontDefinition* fd, ProjectRuntime* er)
        : SharedAssetRuntime(fd, er),
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
        mFontDestructionTask->setState(TaskState::TASK_STATE_QUEUED);
        mProjectRuntime->getGraphicsComponent()->pushDestructionTask(mFontDestructionTask);
    }

    void FontRuntime::pushConstructionTask()
    {
		LOG_TRACE("FontRuntime: {}", __FUNCTION__);
        mProjectRuntime->getGraphicsComponent()->pushTask(&mFontConstructionTask);
    }

	bool FontRuntime::useDefinition()
	{
		LOG_TRACE("FontRuntime: {}", __FUNCTION__);
        FontDefinition* fontDef = static_cast<FontDefinition*>(mDefinition);

        mSize = fontDef->getSize();

        return true;
	}

    int FontRuntime::getSize() const
    {
        return mSize;
    }

    void FontRuntime::setSize(int size)
    {
        mSize = size;
    }

    float FontRuntime::getWidthOf(string s)
    {
		LOG_TRACE("FontRuntime: {} {}", __FUNCTION__,s);
        float width = 0.f;
        for (string::iterator it = s.begin(); it != s.end(); it++)
        {
            width += mCharacterInfo[*it].ax;
        }
        return width/64.f;
    }

    void FontRuntime::draw(EntityRuntime* entity)
    {
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


    void FontRuntime::setFontFile(File* file)
    {
        mFontFile = file;
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
		LOG_TRACE("FontRuntime: {}", __FUNCTION__);
        auto itr = std::find(mInstances.begin(), mInstances.end(),entityRuntime);

        if (itr == mInstances.end())
        {
            mInstances.push_back(entityRuntime);
        }
    }

    void FontRuntime::popInstance(EntityRuntime* entityRuntime)
    {
		LOG_TRACE("FontRuntime: {}", __FUNCTION__);
        auto itr = std::find(mInstances.begin(), mInstances.end(), entityRuntime);

        if (itr != mInstances.end())
        {
            mInstances.erase(itr);
        }
    }

    vector<EntityRuntime*>& FontRuntime::getInstanceVector()
    {
        return mInstances;
    }

    void FontRuntime::setVao(GLuint vao)
    {
        mVao = vao;
    }
	void FontRuntime::setVbo(GLuint vbo)
    {
       mVbo = vbo;
    }

	void FontRuntime::setAtlasTexture(GLuint atlasTexture)
    {
       mAtlasTexture = atlasTexture;
    }

	void FontRuntime::setAtlasWidth(unsigned int atlasWidth)
    {
        mAtlasWidth = atlasWidth;
    }

	void FontRuntime::setAtlasHeight(unsigned int atlasHeight)
    {
       mAtlasHeight = atlasHeight;
    }
}
