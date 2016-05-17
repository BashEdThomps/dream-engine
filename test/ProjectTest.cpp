/*
* ProjectTest
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

#include "ProjectTest.h"

#include "../src/Project/Project.h"
#include "../src/Scene/Scene.h"
#include "../src/Asset/Asset.h"
#include "Asset/TestAssetImpl.h"

namespace Dream {
	namespace Test {
		ProjectTest::ProjectTest(void) : Dream::Unit::Unit("Dream::Project") {
		}

		ProjectTest::~ProjectTest(void) {
		}

		void ProjectTest::run(void) {
			header();
			testProjectCanStoreUUID();
			testProjectCanStoreName();
			testProjectCanStoreAuthor();
			testProjectCanStoreDescription();
			testProjectCanStoreMultipleAssets();
			testProjectCanStoreMultipleScenes();
		}

		void ProjectTest::testProjectCanStoreUUID() {
			Project p;
			std::string uuid = "12ab-34cd-56ef-78af";
			p.setUUID(uuid);
			assertTrue("Stores UUID",p.getUUID().compare(uuid) == 0);
		}

		void ProjectTest::testProjectCanStoreName() {
			Project p;
			std::string name = "ProjectName";
			p.setName(name);
			assertZero("Project name is stored", name.compare(p.getName()));
		}

		void ProjectTest::testProjectCanStoreAuthor(void) {
			Project p;
			std::string author = "Project Author";
			p.setAuthor(author);
			assertZero("Project author is stored",author.compare(p.getAuthor()));
		}

		void ProjectTest::testProjectCanStoreDescription(void) {
			Project p;
			std::string description = "Project Description";
			p.setDescription(description);
			assertZero("Project description is stored",description.compare(p.getDescription()));
		}

		void ProjectTest::testProjectCanStoreMultipleScenes() {
			Project p;
			Dream::Scene::Scene a,b,c;
			a.setName("A");
			b.setName("B");
			c.setName("C");

			p.addScene(&a);
			p.addScene(&b);
			p.addScene(&c);

			assertTrue("Project has 3 scenes",p.getNumberOfScenes() == 3);
		}

		void ProjectTest::testProjectCanStoreMultipleAssets() {
			Project p;

			Dream::Asset::Test::TestAssetImpl a,b,c;
			a.setName("A");
			b.setName("B");
			c.setName("C");

			p.addAsset(&a);
			p.addAsset(&b);
			p.addAsset(&c);

			assertEqual("Project has 3 assets", 3, p.getNumberOfAssets());
		}

		void ProjectTest::testProjectCanStoreDirectory() {
			std::string directory = "/home/ash/.dreamtool/0123-4567-89ab-cdef";
			Project p;
			p.setProjectPath(directory);
			assertZero("Project can store directory",p.getProjectPath().compare(directory));
		}
	} // End of Test
} // End of Dream
