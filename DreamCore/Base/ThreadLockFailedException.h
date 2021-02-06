#pragma once

#include <exception>

using std::exception;

namespace octronic::dream
{
	class ThreadLockFailedException : public exception
	{
	public:
		ThreadLockFailedException();
		~ThreadLockFailedException();
	};
}

