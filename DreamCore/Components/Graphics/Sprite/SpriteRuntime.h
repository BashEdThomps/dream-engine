#pragma once

#include "SpriteTasks.h"
#include "Components/DiscreteAssetRuntime.h"
#include <memory>

using std::shared_ptr;

namespace octronic::dream
{
    // Forward Declarations
    class SpriteDefinition;
    class EntityRuntime;
	class TextureRuntime;

	class SpriteRuntime : public DiscreteAssetRuntime
	{
	public:
		SpriteRuntime(SpriteDefinition* def, EntityRuntime* e );
        ~SpriteRuntime();

		bool useDefinition() override;

		int getWidth() const;
		void setWidth(int size);

		int getHeight() const;
		void setHeight(int height);

		void pushConstructionTask();

		TextureRuntime* getTextureRuntime() const;

		void draw();

        void pushInstance();
        void popInstance();


    private:
        int mWidth;
        int mHeight;
        TextureRuntime* mTexture;
		SpriteConstructionTask mSpriteConstructionTask;
		shared_ptr<SpriteDestructionTask> mSpriteDestructionTask;
	};
}
