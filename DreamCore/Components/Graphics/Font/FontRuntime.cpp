#include "FontRuntime.h"

#include "FontDefinition.h"

#include "Common/Logger.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Storage/StorageManager.h"
#include "Storage/ProjectDirectory.h"
#include "Storage/File.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"
#include "Project/Project.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using std::make_shared;
using std::static_pointer_cast;
using std::max;

/**
 * https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_02
 */
namespace octronic::dream
{
  FontRuntime::FontRuntime(
      const weak_ptr<ProjectRuntime>& pr,
      const weak_ptr<FontDefinition>& fd)
    : SharedAssetRuntime(pr, fd),
      mAtlasTexture(0),
      mAtlasWidth(0),
      mAtlasHeight(0),
      mSize(0),
      mVao(0),
      mVbo(0)
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);
    memset(&mCharacterInfo, 0, sizeof(FontCharacterInfo)*CHAR_INFO_SZ);
  }

  FontRuntime::~FontRuntime()
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);
    mFontRemoveFromGLTask->setFontAtlasTexture(mAtlasTexture);
    mFontRemoveFromGLTask->setFontVao(mVao);
    mFontRemoveFromGLTask->setFontVbo(mVbo);

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto gfxComp = prLock->getGraphicsComponent().lock())
      {
        if (auto gfxDestructionQueue = gfxComp->getDestructionTaskQueue().lock())
        {
          gfxDestructionQueue->pushTask(mFontRemoveFromGLTask);
        }
      }
    }
  }

  bool FontRuntime::init()
  {
    mFontLoadIntoGLTask   = make_shared<FontLoadIntoGLTask>(mProjectRuntime, static_pointer_cast<FontRuntime>(shared_from_this()));
    mFontRemoveFromGLTask = make_shared<FontRemoveFromGLTask>(mProjectRuntime);
    return true;
  }

  bool FontRuntime::loadFromDefinition()
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);
    if (auto defLock = mDefinition.lock())
    {
      auto fontDef = static_pointer_cast<FontDefinition>(defLock);

      mSize = fontDef->getSize();

      if (auto prLock = mProjectRuntime.lock())
      {
        if (auto pdLock = prLock->getProjectDirectory().lock())
        {
          string filename = pdLock->getAssetAbsolutePath(fontDef);
          // Open File
          if (auto smLock = prLock->getStorageManager().lock())
          {
            if (auto fontFile = smLock->openFile(filename).lock())
            {
              if (!fontFile->exists())
              {
                LOG_ERROR("FontCache: Font file {} does not exist", filename);
                smLock->closeFile(fontFile);
                mLoadError = true;
                return false;
              }

              // Read File
              LOG_DEBUG("FontCache: Loading font: {}",filename);

              if (!fontFile->readBinary())
              {
                LOG_ERROR("FontCache: Unable to read file data");
                smLock->closeFile(fontFile);
                fontFile = nullptr;
                mLoadError = true;
                return false;
              }

              vector<uint8_t> buffer = fontFile->getBinaryData();

              // Set Runtime Parameters
              setFontFile(fontFile);

              // Cache FontRuntime
              fontFile = nullptr;
              mLoaded = true;
              return true;
            }
          }
        }
      }
    }
    return false;
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

  void FontRuntime::draw(const weak_ptr<EntityRuntime>& entity)
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);

    if (auto entityLock = entity.lock())
    {
      float sx = entityLock->getFontScale();
      float sy = sx;
      Transform tx = entityLock->getTransform();
      vec3 translation = tx.getTranslation();
      float x = translation.x;
      float y = translation.y;
      string text = entityLock->getFontText();

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

        coords[n++] = {x2, -y2, mCharacterInfo[*p].tx, 0};
        coords[n++] = {x2 + w, -y2, mCharacterInfo[*p].tx + mCharacterInfo[*p].bw / mAtlasWidth, 0};
        coords[n++] = {x2, -y2 - h, mCharacterInfo[*p].tx, mCharacterInfo[*p].bh / mAtlasHeight}; //remember: each glyph occupies a different amount of vertical space
        coords[n++] = {x2 + w, -y2, mCharacterInfo[*p].tx + mCharacterInfo[*p].bw / mAtlasWidth, 0};
        coords[n++] = {x2, -y2 - h, mCharacterInfo[*p].tx, mCharacterInfo[*p].bh / mAtlasHeight};
        coords[n++] = {x2 + w, -y2 - h, mCharacterInfo[*p].tx + mCharacterInfo[*p].bw / mAtlasWidth, mCharacterInfo[*p].bh / mAtlasHeight};
      }

      LOG_TRACE("Font: Buffering {}bytes ({}points) triangle points ({} tris)", sizeof(coords), sizeof(coords)/sizeof(point), n);

      glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords[0], GL_DYNAMIC_DRAW);
      GLCheckError();
      glDrawArrays(GL_TRIANGLES, 0, n);
      GLCheckError();
      glBindVertexArray(0);
    }
  }

  void FontRuntime::setFontFile(const weak_ptr<File>& file)
  {
    mFontFile = file;
  }

  weak_ptr<File> FontRuntime::getFontFile()
  const
  {
    return mFontFile;
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

    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto smLock = prLock->getStorageManager().lock())
      {
        if(auto fileLock = mFontFile.lock())
        {
          // Already read the data in
          vector<uint8_t> font_data = fileLock->getBinaryData();

          if (font_data.size() == 0)
          {
            LOG_ERROR("FontLoadIntoGLTask Font file was not read successfully");
            smLock->closeFile(mFontFile);
            assert(false);
            return false;
          }

          FT_Error ft_error = FT_New_Memory_Face(
                sFreeTypeLibrary,
                static_cast<FT_Byte*>(&font_data[0]),
              font_data.size(), 0, &face );

          if(ft_error)
          {
            LOG_ERROR("FontManager: Unable to create font face: {}", GetFreetypeErrorMessage(ft_error));
            assert(false);
            return false;
          }

          assert(mSize > 0.f);

          FT_Set_Pixel_Sizes(face, 0, mSize);

          if(ft_error)
          {
            LOG_ERROR("FontManager: Unable to create font face: {}", GetFreetypeErrorMessage(ft_error));
            assert(false);
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
          smLock->closeFile(mFontFile);
          return true;
        }
      }
    }
    return false;
  }

  void FontRuntime::pushTasks()
  {
    if (auto prLock = mProjectRuntime.lock())
    {
      if (auto taskQueue = prLock->getTaskQueue().lock())
      {
        if (auto gfxCompLock = prLock->getGraphicsComponent().lock())
        {
          if (auto gfxTaskQueue = gfxCompLock->getTaskQueue().lock())
          {
            if (!mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
            {
              taskQueue->pushTask(mLoadFromDefinitionTask);
            }
            else if (mLoaded & mFontLoadIntoGLTask->hasState(TASK_STATE_QUEUED))
            {
              gfxTaskQueue->pushTask(mFontLoadIntoGLTask);
            }
            else
            {
              for(auto entity : mInstances)
              {
                if (auto entityLock = entity.lock())
                {
                  if (auto srLock = entityLock->getSceneRuntime().lock())
                  {
                    if (srLock->hasState(SCENE_STATE_ACTIVE))
                    {
                      // Do entity specific tasks
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // Statics =================================================================

  bool FontRuntime::InitFreetypeLibrary()
  {
    if (sFreeTypeLibrary != nullptr) return false;

    LOG_TRACE("FontCache: {}",__FUNCTION__);
    FT_Error ft_error = FT_Init_FreeType(&sFreeTypeLibrary);
    if(ft_error)
    {
      LOG_ERROR("FontManager: Unable to initialise FreeType Library: {}",ft_error);
      return false;
    }
    return true;

  }

  void FontRuntime::CleanupFreetypeLibrary()
  {
    FT_Done_FreeType(sFreeTypeLibrary);
  }

  FT_Library FontRuntime::GetFreeTypeLibrary()
  {
    return sFreeTypeLibrary;
  }

  const char* FontRuntime::GetFreetypeErrorMessage(FT_Error err)
  {
    LOG_TRACE("FontManager: {}",__FUNCTION__);
#undef FTERRORS_H_
#define FT_ERRORDEF( e, v, s )  case e: return s;
#define FT_ERROR_START_LIST     switch (err) {
#define FT_ERROR_END_LIST       }
#include FT_ERRORS_H
    return "(Unknown error)";
  }

  FT_Library FontRuntime::sFreeTypeLibrary;
}

