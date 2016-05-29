/*
 * Dream::Scripting::ChaiScripting
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ChaiScripting.h"

namespace Dream     {
namespace Plugins   {
namespace Scripting {
namespace Chai      {
	
	ChaiScripting::ChaiScripting(void) : ScriptingPluginInterface() {
		mChai = NULL;
	}
				
	ChaiScripting::~ChaiScripting(void) {}
				
	bool ChaiScripting::init() {
		if (mChai != NULL) {
			delete mChai;
		}
					
		mChai = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());
		return mChai != NULL;
	}
				
	chaiscript::ChaiScript* ChaiScripting::getChaiEngine() {
		return mChai;
	}

	void ChaiScripting::update(Dream::Scene::Scene* scene) {
		Dream::Scene::SceneObject* root = scene->getRootSceneObject();
		if (root->hasScriptAssetInstance()) try {
			std::cout << "ChaiScripting: Invoking RootSceneObject Update Script" << std::endl;
			Asset::Instances::Script::ChaiScript::ChaiScriptInstance* script;
			script = dynamic_cast<Asset::Instances::Script::ChaiScript::ChaiScriptInstance*>(
					root->getScriptAssetInstance()
			);
		} catch (std::exception ex) {
			std::cerr << "ChaiScripting: Could not cast Root Scene Object's Script AssetInstance to ChaiScriptInstance"
			          << std::endl
			          << ex.what()
			          << std::endl;
		}
	}
	
} // End of Chai
} // End of Scripting
} // End of Plugins
} // End of Dream
