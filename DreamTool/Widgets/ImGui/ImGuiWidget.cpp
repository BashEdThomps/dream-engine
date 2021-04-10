#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
  ImGuiWidget::ImGuiWidget
  (DreamToolContext& project, bool visible)
    : DreamToolWidget(project, visible)
  {
  }

  bool
  ImGuiWidget::StringCombo
  (const char* label, int* current_item, const vector<string>& items)
  {
    int height_in_items = -1;
    return ImGui::Combo(
          label, current_item,
          [](void* data, int idx, const char** out_text)
					{
					  auto _data = ((const vector<string>*)data);
					  *out_text = (*_data)[idx].c_str();
					  return true;
					},
					(void*)&items,
					items.size(),
					height_in_items
    );
  }

  int
  ImGuiWidget::GetStringIndexInVector
  (const string& str, const vector<string>& vec)
  {
    auto itr = find(vec.begin(), vec.end(), str);
    if (itr != vec.end())
    {
      return std::distance(vec.begin(),itr);
    }
    else
    {
      return -1;
    }
  }
}
