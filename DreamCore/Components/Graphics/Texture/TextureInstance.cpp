#include "TextureInstance.h"
#include "TextureDefinition.h"
#include "../../../Scene/SceneObject/SceneObjectRuntime.h"

namespace Dream
{
    TextureInstance::TextureInstance
    (TextureDefinition* def)
        : IAssetInstance (def,nullptr),
          mGLID(0),
          mPath(""),
          mWidth(0),
          mHeight(0),
          mChannels(0),
          mImage(nullptr)
    {

    }

    TextureInstance::~TextureInstance
    ()
    {
        glDeleteTextures(1,&mGLID);
        checkGLError();
    }

    string TextureInstance::getPath() const
    {
        return mPath;
    }

    void TextureInstance::setPath(const string& path)
    {
        mPath = path;
    }

    int TextureInstance::getWidth() const
    {
        return mWidth;
    }

    void TextureInstance::setWidth(int width)
    {
        mWidth = width;
    }

    int TextureInstance::getHeight() const
    {
        return mHeight;
    }

    void TextureInstance::setHeight(int height)
    {
        mHeight = height;
    }

    int TextureInstance::getChannels() const
    {
        return mChannels;
    }

    void TextureInstance::setChannels(int channels)
    {
        mChannels = channels;
    }

    unsigned char* TextureInstance::getImage() const
    {
        return mImage;
    }

    void TextureInstance::setImage(unsigned char* image)
    {
        mImage = image;
    }

    GLuint TextureInstance::getGLID() const
    {
        return mGLID;
    }

    void TextureInstance::setGLID(const GLuint& gLID)
    {
        mGLID = gLID;
    }

    bool TextureInstance::operator==(const TextureInstance& other)
    {
        return this->mGLID == other.mGLID && this->mPath.compare(other.mPath) == 0;
    }


    bool
    TextureInstance::load
    (string projectPath)
    {
        return false;
    }

    void
    TextureInstance::loadExtraAttributes
    (nlohmann::json)
    {
    }

}
