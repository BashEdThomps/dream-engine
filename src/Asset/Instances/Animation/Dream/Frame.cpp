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
		sceneObject->setTranslation(mTranslation);
		sceneObject->setRotation(mRotation);
		sceneObject->setScale(mScale);
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
