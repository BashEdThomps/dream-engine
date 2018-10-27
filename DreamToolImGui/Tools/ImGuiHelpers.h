#pragma once
#include <vector>
#include <string>
#include "../deps/ImGui/imgui.h"

using std::vector;
using std::string;

namespace DreamTool
{
	bool
	StringCombo 
	(const char* label, int* current_item, const vector<string>& items, int items_count, int height_in_items = -1);
}
