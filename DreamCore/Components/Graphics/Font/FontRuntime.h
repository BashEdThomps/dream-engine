#pragma once

#include "FontTasks.h"
#include "FontCharacterInfo.h"
#include "Components/SharedAssetRuntime.h"
#include "Common/GLHeader.h"
#include <memory>

using std::shared_ptr;

#define CHAR_INFO_SZ 128

namespace octronic::dream
{
    // Forward Declarations
    class File;
    class FontDefinition;
    class EntityRuntime;

	class FontRuntime : public SharedAssetRuntime
	{
	public:
		FontRuntime(FontDefinition* def, ProjectRuntime* e );
        ~FontRuntime();

		bool useDefinition() override;

        float getWidthOf(string s);
        void draw(EntityRuntime* fti);

		int getSize() const;
		void setSize(int size);

		void pushConstructionTask();

        void setFontFile(File* file);
        File* getFontFile() const;

        FontCharacterInfo* getCharacterInfo();

        void pushInstance(EntityRuntime*);
        void popInstance(EntityRuntime*);
        vector<EntityRuntime*>& getInstanceVector();

        void setVao(GLuint vao);
        void setVbo(GLuint vbo);
        void setAtlasTexture(GLuint atlasTexture);
        void setAtlasWidth(unsigned int atlasWidth);
        void setAtlasHeight(unsigned int atlasHeight);


        GLuint getAtlasTexture() const;
        unsigned int getAtlasWidth() const;
        unsigned int getAtlasHeight() const;

        bool loadIntoGL();

    private:
        int mSize;
        FontCharacterInfo mCharacterInfo[CHAR_INFO_SZ];
        GLuint mAtlasTexture;
		unsigned int mAtlasWidth;
		unsigned int mAtlasHeight;
		GLuint mVao;
		GLuint mVbo;
        File* mFontFile;
		FontConstructionTask mFontConstructionTask;
		shared_ptr<FontDestructionTask> mFontDestructionTask;
        vector<EntityRuntime*> mInstances;
	};
}
