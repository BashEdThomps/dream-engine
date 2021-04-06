#include "ImGuiWidget.h"

namespace octronic::dream::tool
{
  ImGuiWidget::ImGuiWidget
  (DreamToolContext& project, bool visible)
    : DreamToolWidget(project, visible)
  {
  }

  ImGuiWidget::~ImGuiWidget() {}

  bool
  ImGuiWidget::StringCombo
  (const char* label, int* current_item,
   const vector<string>& items, int height_in_items)
  {
    return ImGui::Combo
        (label, current_item,
         [](void* data, int idx, const char** out_text)
    { auto _data = ((const vector<string>*)data);
      *out_text = (*_data)[idx].c_str();
      return true; },
    (void*)&items, items.size(), height_in_items);
  }

  int
  ImGuiWidget::GetStringIndexInVector
  (const string& str, const vector<string>& vec)
  {
    size_t sz = vec.size();
    for (int i=0; i<sz; i++)
    {
      if (vec.at(i).compare(str) == 0)
      {
        return i;
      }
    }
    return -1;
  }
}
