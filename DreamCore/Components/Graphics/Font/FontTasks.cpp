#include "FontRuntime.h"
#include "FontTasks.h"
#include "FontCache.h"
#include "Project/ProjectRuntime.h"
#include "Components/Storage/File.h"
#include "Components/Storage/StorageManager.h"

using std::max;

namespace octronic::dream
{
    FontConstructionTask::FontConstructionTask(FontRuntime* rt)
        : GraphicsComponentTask("FontConstructionTask"),
        mFontRuntime(rt)
    {
    }

    void FontConstructionTask::execute()
    {
        LOG_TRACE("FontConstructionTask: {} Executing on Graphics thread",mFontRuntime->getNameAndUuidString());

        GLuint vao;
        GLuint vbo;
        GLuint atlasTexture;
        unsigned int atlasWidth;
        unsigned int atlasHeight;

        // VAO
        glGenVertexArrays(1,&vao);
        GLCheckError();
        glBindVertexArray(vao);
        GLCheckError();

        // VBO
        glGenBuffers(1,&vbo);
        GLCheckError();
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
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

        FontCache* fontCache = mFontRuntime->getProjectRuntime()->getFontCache();
        FT_Library ft_lib = fontCache->getFreeTypeLibrary();

        StorageManager* sm = mFontRuntime->getProjectRuntime()->getStorageManager();
        File* font_file = mFontRuntime->getFontFile();

        // Already read the data in
        void* font_data = font_file->getBinaryData();
        size_t font_data_sz = font_file->getBinaryDataSize();

        if (!font_data || font_data_sz == 0)
        {
            LOG_ERROR("FontConstructionTask: Font file was not read successfully");
            sm->closeFile(font_file);
            mFontRuntime->setFontFile(nullptr);
            return;
        }

        FT_Error ft_error = FT_New_Memory_Face(ft_lib,static_cast<FT_Byte*>(font_data),font_data_sz, 0, &face );

        if(ft_error)
        {
            LOG_ERROR("FontManager: Unable to create font face: {}", FontCache::getFreetypeErrorMessage(ft_error));
            return;
        }

        assert(mFontRuntime->getSize() > 0.f);

        FT_Set_Pixel_Sizes( face, 0, mFontRuntime->getSize() );

        if(ft_error)
        {
            LOG_ERROR("FontManager: Unable to create font face: {}", FontCache::getFreetypeErrorMessage(ft_error));
            return ;
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
        atlasWidth = w;
        atlasHeight = h;

        glGenTextures(1, &atlasTexture);
        GLCheckError();

        glActiveTexture(GL_TEXTURE0);
        GLCheckError();

        glBindTexture(GL_TEXTURE_2D, atlasTexture);
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


        int x = 0;

        auto characterInfo = mFontRuntime->getCharacterInfo();

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

            characterInfo[i].ax = g->advance.x >> 6;
            characterInfo[i].ay = g->advance.y >> 6;
            characterInfo[i].bw = g->bitmap.width;
            characterInfo[i].bh = g->bitmap.rows;
            characterInfo[i].bl = g->bitmap_left;
            characterInfo[i].bt = g->bitmap_top;
            characterInfo[i].tx = (float)x / w;

            x += g->bitmap.width;
        }

        FT_Done_Face(face);
        sm->closeFile(font_file);
        mFontRuntime->setFontFile(nullptr);
        mFontRuntime->setVao(vao);
        mFontRuntime->setVbo(vbo);
        mFontRuntime->setAtlasTexture(atlasTexture);
        mFontRuntime->setAtlasWidth(atlasWidth);
        mFontRuntime->setAtlasHeight(atlasHeight);

        setState(TaskState::TASK_STATE_COMPLETED);
    }

    FontDestructionTask::FontDestructionTask
    ()
        : GraphicsComponentDestructionTask("FontDestructionTask")
    {
    }

    void FontDestructionTask::execute()
    {
        LOG_TRACE("FontDestructionTask: with texture:{} vao:{} vbo:{}, Executing on Graphics thread",
                  mFontAtlasTexture, mFontVao, mFontVbo);

        glDeleteBuffers(1,&mFontVao);
        glDeleteBuffers(1,&mFontVbo);
        glDeleteTextures(1,&mFontAtlasTexture);

        setState(TaskState::TASK_STATE_COMPLETED);
    }

    void FontDestructionTask::setFontAtlasTexture(GLuint id)
    {
        mFontAtlasTexture = id;
    }

    void FontDestructionTask::setFontVao(GLuint id)
    {
        mFontVao = id;
    }

    void FontDestructionTask::setFontVbo(GLuint id)
    {
        mFontVbo = id;
    }
}
