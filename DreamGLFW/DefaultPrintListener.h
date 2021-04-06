#pragma once

#include <DreamCore.h>

using octronic::dream::ScriptPrintListener;

namespace octronic::dream::glfw
{
  class DefaultPrintListener : public ScriptPrintListener
  {
  public:
    DefaultPrintListener();
    ~DefaultPrintListener();
    void onPrint(const string& str) override;
  };
}

