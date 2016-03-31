#ifndef DREAM_H
#define DREAM_H

#include "Scene.h"

namespace Dream {
	class Dream {
	protected:
		Scene *mScene;
	public:
		Dream(void);
		~Dream(void);
		Scene* getScene(void);
		void setScene(Scene*);
	};
}


#endif // DREAM_H
