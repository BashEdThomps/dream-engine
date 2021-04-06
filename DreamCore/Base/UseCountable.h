#pragma once

namespace octronic::dream
{
  class UseCountable
  {
  public:
    UseCountable();

    UseCountable(const UseCountable&) = delete;
    UseCountable& operator=(const UseCountable&) = delete;

	  UseCountable(UseCountable&&) = default;
    UseCountable& operator=(UseCountable&&) = default;

    bool hasUsers() const;
    bool hasNoUsers() const;
    unsigned long getUseCount() const;
    void incrementUseCount();
    void decrementUseCount();
  private:
    unsigned long mUseCount;
  };
}

