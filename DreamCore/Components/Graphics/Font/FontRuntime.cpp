#include "FontRuntime.h"

#include "FontDefinition.h"

#include "Common/Logger.h"
#include "Components/Graphics/GraphicsComponent.h"
#include "Storage/StorageManager.h"
#include "Project/ProjectDirectory.h"
#include "Storage/File.h"
#include "Scene/SceneRuntime.h"
#include "Entity/EntityRuntime.h"
#include "Project/ProjectRuntime.h"


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
  FontRuntime::FontRuntime(ProjectRuntime& pr,FontDefinition& fd)
    : SharedAssetRuntime(pr, fd),
      mSize(0),
      mAtlasTexture(0),
      mAtlasWidth(0),
      mAtlasHeight(0),
      mVao(0),
      mVbo(0)
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);
    memset(&mCharacterInfo, 0, sizeof(FontCharacterInfo)*CHAR_INFO_SZ);
    mFontLoadIntoGLTask   = make_shared<FontLoadIntoGLTask>(getProjectRuntime(), *this);
    mFontRemoveFromGLTask = make_shared<FontRemoveFromGLTask>(getProjectRuntime());
  }

  FontRuntime::~FontRuntime
  ()
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);
    mFontRemoveFromGLTask->setFontAtlasTexture(mAtlasTexture);
    mFontRemoveFromGLTask->setFontVao(mVao);
    mFontRemoveFromGLTask->setFontVbo(mVbo);

    auto& gfxComp = getProjectRuntime().getGraphicsComponent();
    auto& gfxDestructionQueue = gfxComp.getDestructionTaskQueue();
    gfxDestructionQueue.pushTask(mFontRemoveFromGLTask);
  }

  bool
  FontRuntime::loadFromDefinition
  ()
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);
    auto& fontDef = static_cast<FontDefinition&>(getDefinition());

    mSize = fontDef.getSize();

    auto& pd = getProjectRuntime().getProjectDirectory();
    string filename = pd.getAssetAbsolutePath(fontDef);
    // Open File
    auto& sm = getProjectRuntime().getStorageManager();
    auto& fontFile = sm.openFile(filename);

    if (!fontFile.exists())
    {
      LOG_ERROR("FontCache: Font file {} does not exist", filename);
      sm.closeFile(fontFile);
      mLoadError = true;
      return false;
    }

    // Read File
    LOG_DEBUG("FontCache: Loading font: {}",filename);

    if (!fontFile.readBinary())
    {
      LOG_ERROR("FontCache: Unable to read file data");
      sm.closeFile(fontFile);
      mLoadError = true;
      return false;
    }

    mFontData = fontFile.getBinaryData();

    // Cache FontRuntime
    mLoaded = true;
    return true;
  }

  int
  FontRuntime::getSize
  () const
  {
    return mSize;
  }

  void
  FontRuntime::setSize
  (int size)
  {
    mSize = size;
  }

  float
  FontRuntime::getWidthOf
  (string s)
  {
    LOG_TRACE("FontRuntime: {} {}", __FUNCTION__,s);
    float width = 0.f;
    for (string::iterator it = s.begin(); it != s.end(); it++)
    {
      width += mCharacterInfo[(size_t)*it].ax;
    }
    return width/64.f;

  }

  void FontRuntime::draw
  (EntityRuntime& entity)
  {
    LOG_TRACE("FontRuntime: {}", __FUNCTION__);

    float sx = entity.getFontScale();
    float sy = sx;
    Transform tx = entity.getTransform();
    vec3 translation = tx.getTranslation();
    float x = translation.x;
    float y = translation.y;
    string text = entity.getFontText();

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
      float x2 =  x + mCharacterInfo[(size_t)*p].bl * sx;
      float y2 = -y - mCharacterInfo[(size_t)*p].bt * sy;
      float w = mCharacterInfo[(size_t)*p].bw * sx;
      float h = mCharacterInfo[(size_t)*p].bh * sy;

      /* Advance the cursor to the start of the next character */
      x += mCharacterInfo[(size_t)*p].ax * sx;
      y += mCharacterInfo[(size_t)*p].ay * sy;

      /* Skip glyphs that have no pixels */
      if(!w || !h) continue;

      coords[n++] = {x2, -y2, mCharacterInfo[(size_t)*p].tx, 0};
      coords[n++] = {x2 + w, -y2, mCharacterInfo[(size_t)*p].tx + mCharacterInfo[(size_t)*p].bw / mAtlasWidth, 0};
      coords[n++] = {x2, -y2 - h, mCharacterInfo[(size_t)*p].tx, mCharacterInfo[(size_t)*p].bh / mAtlasHeight}; //remember: each glyph occupies a different amount of vertical space
      coords[n++] = {x2 + w, -y2, mCharacterInfo[(size_t)*p].tx + mCharacterInfo[(size_t)*p].bw / mAtlasWidth, 0};
      coords[n++] = {x2, -y2 - h, mCharacterInfo[(size_t)*p].tx, mCharacterInfo[(size_t)*p].bh / mAtlasHeight};
      coords[n++] = {x2 + w, -y2 - h, mCharacterInfo[(size_t)*p].tx + mCharacterInfo[(size_t)*p].bw / mAtlasWidth, mCharacterInfo[(size_t)*p].bh / mAtlasHeight};
    }

    LOG_TRACE("Font: Buffering {}bytes ({}points) triangle points ({} tris)", sizeof(coords), sizeof(coords)/sizeof(point), n);

    glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords[0], GL_DYNAMIC_DRAW);
    GLCheckError();
    glDrawArrays(GL_TRIANGLES, 0, n);
    GLCheckError();
    glBindVertexArray(0);
  }

  void
  FontRuntime::setFontData
  (const vector<uint8_t>& data)
  {
    mFontData = data;
  }

  vector<uint8_t>
  FontRuntime::getFontData
  ()
  {
    return mFontData;
  }

  void
  FontRuntime::setVao
  (GLuint vao)
  {
    mVao = vao;
  }

  void
  FontRuntime::setVbo
  (GLuint vbo)
  {
    mVbo = vbo;
  }

  void
  FontRuntime::setAtlasTexture
  (GLuint atlasTexture)
  {
    mAtlasTexture = atlasTexture;
  }

  void
  FontRuntime::setAtlasWidth
  (unsigned int atlasWidth)
  {
    mAtlasWidth = atlasWidth;
  }

  void
  FontRuntime::setAtlasHeight
  (unsigned int atlasHeight)
  {
    mAtlasHeight = atlasHeight;
  }

  GLuint
  FontRuntime::getAtlasTexture
  () const
  {
    return mAtlasTexture;
  }

  unsigned int
  FontRuntime::getAtlasWidth
  () const
  {
    return mAtlasWidth;
  }

  unsigned int
  FontRuntime::getAtlasHeight
  () const
  {
    return mAtlasHeight;
  }

  bool
  FontRuntime::loadIntoGL
  ()
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

    if(!mFontData.empty())
    {

      FT_Error ft_error = FT_New_Memory_Face(
            sFreeTypeLibrary,
            static_cast<FT_Byte*>(&mFontData[0]),
          mFontData.size(), 0, &face );

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
      return true;
    }
    return false;
  }

  void FontRuntime::pushTasks()
  {
    auto& taskQueue = getProjectRuntime().getTaskQueue();
    auto& gfxComp = getProjectRuntime().getGraphicsComponent();
    auto& gfxTaskQueue = gfxComp.getTaskQueue();

    if (!mLoaded && !mLoadError && mLoadFromDefinitionTask->hasState(TASK_STATE_QUEUED))
    {
      taskQueue.pushTask(mLoadFromDefinitionTask);
    }
    else if (mLoaded & mFontLoadIntoGLTask->hasState(TASK_STATE_QUEUED))
    {
      gfxTaskQueue.pushTask(mFontLoadIntoGLTask);
    }
    else
    {
      for(auto& entity : mInstances)
      {
        auto& sr = entity.get().getSceneRuntime();
        if (sr.hasState(SCENE_STATE_ACTIVE))
        {
          // Do entity specific tasks
        }
      }
    }
  }

  // Statics =================================================================

  bool
  FontRuntime::InitFreetypeLibrary
  ()
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

  void
  FontRuntime::CleanupFreetypeLibrary
  ()
  {
    FT_Done_FreeType(sFreeTypeLibrary);
  }

  FT_Library
  FontRuntime::GetFreeTypeLibrary
  ()
  {
    return sFreeTypeLibrary;
  }

  const char*
  FontRuntime::GetFreetypeErrorMessage
  (FT_Error err)
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

