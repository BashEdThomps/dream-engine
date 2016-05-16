//
//  DreamAnimation.hpp
//  Dream
//
//  Created by Ashley Thompson on 16/05/2016.
//  Copyright © 2016 Octronic. All rights reserved.
//

#ifndef DreamAnimation_hpp
#define DreamAnimation_hpp

#include "../Interfaces/AnimationInterface.h"

namespace Dream {
namespace Plugins {
namespace Animation {
	
	class DreamAnimation : public AnimationInterface {
	public :
		DreamAnimation();
		~DreamAnimation();
		bool init();
		void update(Dream::Scene::Scene*);
	};
	
} // End of Animation
} // End of Plugins
} // End of Dream

#endif /* DreamAnimation_hpp */
