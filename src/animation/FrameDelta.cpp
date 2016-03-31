#include <iostream>

#include "FrameDelta.h"

namespace Dream {
	namespace Animation {
		FrameDelta::FrameDelta(int drawableId, int operation) {
		   mOperation            = operation;
		   mDrawableID           = drawableId;

		   mOrbitRadius          = 0;

		   mPositionDelta [DA_X] = 0;
		   mPositionDelta [DA_Y] = 0;
		   mPositionDelta [DA_Z] = 0;

		   mRotationDelta [DA_X] = 0;
		   mRotationDelta [DA_Y] = 0;
		   mRotationDelta [DA_Z] = 0;
		}

		FrameDelta::~FrameDelta() {

		}

		FrameDelta* FrameDelta::computeFrameDelta(
		    FrameDelta* start,
		    FrameDelta* end,
		    int steps,
		    int stepIndex) {
			FrameDelta* d = new FrameDelta(start->getDrawableID(), start->getOperation());
			float posDelta[3];
			float rotDelta[3];

			computeDeltaVector(&posDelta[0], start->getPositionDelta(), end->getPositionDelta());
			computeDeltaVector(&rotDelta[0], start->getRotationDelta(), end->getRotationDelta());

			switch (d->getOperation()) {
			    default:
			    case DA_OP_LINEAR:
				posDelta [DA_X] /= steps;
				posDelta [DA_Y] /= steps;
				posDelta [DA_Z] /= steps;
				break;
			    case DA_OP_ORBIT:
				break;
			    case DA_OP_BEZIER:
				break;
			}

			rotDelta[DA_X] /= steps;
			rotDelta[DA_Y] /= steps;
			rotDelta[DA_Z] /= steps;

			d->setPositionDelta(posDelta);
			d->setRotationDelta(rotDelta);

			return d;
		    }

		void FrameDelta::computeDeltaVector(float* buffer, float* a, float* b) {
		    buffer[DA_X] = a[DA_X] - b[DA_X];
		    buffer[DA_Y] = a[DA_Y] - b[DA_Y];
		    buffer[DA_Z] = a[DA_Z] - b[DA_Z];
		}

		void FrameDelta::printDebug() {
		    std::cout << "Delta for ID: " << mDrawableID << std::endl;
		    std::cout <<"\tPOS: X: " << mPositionDelta[DA_X] << " Y: " << mPositionDelta[DA_Y] << " Z: " << mRotationDelta[DA_Z] << std::endl;
		    std::cout <<"\tROT: X: " << mRotationDelta[DA_X] << " Y: " << mRotationDelta[DA_Y] << " Z: " << mRotationDelta[DA_Z] << std::endl;
		}

		int FrameDelta::getDrawableID() {
			return mDrawableID;
		}

		//! TODO - Document
		float* FrameDelta::getPositionDelta() {
			return mRotationDelta;
		}
		//! TODO - Document
		float* FrameDelta::getRotationDelta() {
			return mRotationDelta;
		}

		int FrameDelta::getOperation() {
			return mOperation;
		}

		void FrameDelta::setPositionDelta(float* pos) {
			mPositionDelta[DA_X] = pos[DA_X];
			mPositionDelta[DA_Y] = pos[DA_Y];
			mPositionDelta[DA_Z] = pos[DA_Z];
		}

		void FrameDelta::setRotationDelta(float* rot) {
			mRotationDelta[DA_X] = rot[DA_X];
			mRotationDelta[DA_Y] = rot[DA_Y];
			mRotationDelta[DA_Z] = rot[DA_Z];
		}
	}
}
