#include <iostream>
#include "Frame.h"
#include "../../../../Util/String.h"

namespace Dream     {
namespace Asset     {
namespace Instances {
namespace Animation {
namespace Dream     {
	
	Frame::Frame()  {
		mTranslation = std::vector<float>(3);
		mRotation    = std::vector<float>(3);
		mScale       = std::vector<float>(3);
	}
	
	Frame::~Frame() {}
	
	void Frame::applyToSceneObject(Scene::SceneObject* sceneObject) {
		std::vector<float> soTranslation = sceneObject->getTranslation();
		soTranslation[0] = mTranslation[0];
		soTranslation[1] = mTranslation[1];
		soTranslation[2] = mTranslation[2];
		
		std::vector<float> soRotation = sceneObject->getRotation();
		soRotation[0] = mRotation[0];
		soRotation[1] = mRotation[1];
		soRotation[2] = mRotation[2];
		
		std::vector<float> soScale = sceneObject->getScale();
		soScale[0] = mScale[0];
		soScale[1] = mScale[1];
		soScale[2] = mScale[2];
	}
	
	std::vector<float> Frame::getTranslation() {
		return mTranslation;
	}
	
	std::vector<float> Frame::getRotation() {
		return mRotation;
	}
	
	std::vector<float> Frame::getScale() {
		return mScale;
	}
	
	void Frame::setRotation(std::vector<float> rotation) {
		mRotation = rotation;
	}
	
	void Frame::setScale(std::vector<float> scale) {
		mScale = scale;
	}
	
	void Frame::setTranslation(std::vector<float> translation) {
		mTranslation = translation;
	}
	
	void Frame::showStatus() {
		std::cout << "Frame:" << std::endl
		<< "\tTranslation: "  << Util::String::floatVectorToString(mTranslation) << std::endl
		<< "\t   Rotation: "  << Util::String::floatVectorToString(mRotation)    << std::endl
		<< "\t      Scale: "  << Util::String::floatVectorToString(mScale)       << std::endl;
	}
	

} // End of Dream
} // End of Animation
} // End of Instances
} // End of Asset
} // End of Dream
