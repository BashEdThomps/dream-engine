#pragma once

#include "Logger.h"

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include "glad/glad.h"
    #include <OpenGL/gl.h>
#endif

#ifdef __linux__
    #include "glad/glad.h"
    #include <GL/gl.h>
#endif

#ifdef _WIN32
    #ifndef NOMINMAX
        #define NOMINMAX // prevent windows redefining min/max
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>
    #include "glad/glad.h"
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


#ifdef ENABLE_LOGGING
	#include <string>
	using std::string;

	#define GLCheckError() _GLCheckError_(__FILE__, __LINE__)
	/**
	* @brief Used to check for OpenGL Runtime Errors. This will display the
	* file and line from which the error was detected. This function should
	* not be called directly. Instead use the macro GLCheckError.
	*
	* @return True if an error was detected.
	*/
	static bool _GLCheckError_(const string& file, int line)
	{
		GLenum errorCode = 0;
		bool wasError = false;
		do
		{
	#ifdef __ANDROID__
			errorCode = glGetError();
	#else
		  errorCode = glad_glGetError();
	#endif
		  if (errorCode!=0)
		  {
			  LOG_ERROR("GLHeader: OpenGL Error Check {}:{} ", file, line);
			  switch (errorCode)
			  {
				  case GL_NO_ERROR:
					  LOG_ERROR("\tGL_NO_ERROR");
					  break;
				  case GL_INVALID_ENUM:
					  LOG_ERROR("\tGL_INVALID_ENUM" );
					  break;
				  case GL_INVALID_VALUE:
					  LOG_ERROR("\tGL_INVALID_VALUE");
					  break;
				  case GL_INVALID_OPERATION:
					  LOG_ERROR("\tGL_INVALID_OPERATION");
					  break;
				  case GL_OUT_OF_MEMORY:
					  LOG_ERROR("\tGL_OUT_OF_MEMORY");
					  break;
				  default:
					  LOG_ERROR("\t Unknown Error");
					  break;
			  }
			  LOG_ERROR("\tCode: {}" , errorCode );
			  wasError = true;
			  assert(!wasError);
		  }
		}
		while(errorCode != 0);
		return wasError;
	}
#else
	#define GLCheckError()
#endif


