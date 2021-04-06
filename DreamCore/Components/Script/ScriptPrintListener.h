#pragma once

#include <string>

using std::string;

namespace octronic::dream
{
 class ScriptPrintListener
  {
  public:
    virtual ~ScriptPrintListener();
    virtual void onPrint(const string&) = 0;
  };
}
