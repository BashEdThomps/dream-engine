#pragma once

#include "FontTasks.h"
#include "FontCharacterInfo.h"
#include "Components/SharedAssetRuntime.h"
#include "Common/GLHeader.h"

#include <memory>

using std::shared_ptr;

#define CHAR_INFO_SZ 128

typedef struct FT_LibraryRec_  *FT_Library;
typedef int FT_Error;

namespace octronic::dream
{
  // Forward Declarations
  class File;
  class FontDefinition;
  class EntityRuntime;

  class FontRuntime : public SharedAssetRuntime
  {
  public:
    FontRuntime(const weak_ptr<ProjectRuntime>& e,
                const weak_ptr<FontDefinition>& def);
    ~FontRuntime();

    bool loadFromDefinition() override;
    bool init() override;
    bool loadIntoGL();
    void pushTasks() override;

    void draw(const weak_ptr<EntityRuntime>& fti);

    float getWidthOf(string s);

    int getSize() const;
    void setSize(int size);

    void setFontFile(const weak_ptr<File>& file);
    weak_ptr<File> getFontFile() const;

    void setVao(GLuint vao);
    void setVbo(GLuint vbo);
    void setAtlasTexture(GLuint atlasTexture);
    void setAtlasWidth(unsigned int atlasWidth);
    void setAtlasHeight(unsigned int atlasHeight);

    GLuint getAtlasTexture() const;
    unsigned int getAtlasWidth() const;
    unsigned int getAtlasHeight() const;



    static bool InitFreetypeLibrary();
    static void CleanupFreetypeLibrary();
    static FT_Library GetFreeTypeLibrary();
    static const char* GetFreetypeErrorMessage(FT_Error err);

  private:
    static FT_Library sFreeTypeLibrary;

  private:
    int mSize;
    FontCharacterInfo mCharacterInfo[CHAR_INFO_SZ];
    GLuint mAtlasTexture;
    unsigned int mAtlasWidth;
    unsigned int mAtlasHeight;
    GLuint mVao;
    GLuint mVbo;
    weak_ptr<File> mFontFile;
    shared_ptr<FontLoadIntoGLTask> mFontLoadIntoGLTask;
    shared_ptr<FontRemoveFromGLTask> mFontRemoveFromGLTask;
  };
}
