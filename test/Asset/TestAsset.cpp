/*
* TestAsset
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

#include "TestAsset.h"

namespace Dream {
namespace Asset {
namespace Test  {
	
	TestAsset::TestAsset(void) : Dream::Unit::Unit("Dream::Asset::Asset") {}
	TestAsset::~TestAsset(void) {}

	void TestAsset::run() {
		header();
		testAssetCanStoreUUID();
		testAssetCanStoreName();
	}

	void TestAsset::testAssetCanStoreUUID() {
		Dream::Asset::AssetDefinition r;
		std::string uuid = "1234-abcd-3382-efda";
		r.setUUID(uuid);
		assertZero("Asset can store UUID",uuid.compare(r.getUUID()));
	}

	void TestAsset::testAssetCanStoreName() {
		Dream::Asset::AssetDefinition r;
		std::string name = "TestAsset";
		r.setName(name);
		assertZero("Asset can store name",name.compare(r.getName()));
	}
	
} // End of Test
} // End of Asset
} // End of Dream
