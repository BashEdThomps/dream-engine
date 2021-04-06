#include "DefaultPrintListener.h"

namespace octronic::dream::glfw
{
  DefaultPrintListener::DefaultPrintListener
  ()
  {}

  DefaultPrintListener::~DefaultPrintListener
  ()
  {}

  void
  DefaultPrintListener::onPrint
  (const string& str)
  {
    LOG_ERROR("LUA PrintListener: {}", str);
  }
}
