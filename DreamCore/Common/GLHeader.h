#pragma once

#include "Logger.h"

#if defined(__APPLE__)
    #define GL_SILENCE_DEPRECATION
    #include "glad/glad.h"
    #include <OpenGL/gl.h>
#elif defined(__ANDROID__)
	#if __ANDROID_API__ >= 24
		#include <GLES3/gl32.h>
	#elif __ANDROID_API__ >= 21
		#include <GLES3/gl31.h>
	#elif __ANDROID_API >= 18
		#include <GLES3/gl3.h>
	#else
		#define GL_GLEXT_PROTOTYPES
		#include <GLES2/gl2.h>
		#include <GLES2/gl2ext.h>
		#include <EGL/egl.h>
	#endif
#elif defined(__linux__)
    #include "glad/glad.h"
    #include <GL/gl.h>
#elif defined(_WIN32)
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


#if defined(ENABLE_LOGGING)
	#include <string>
	using std::string;

	/**
	* @brief Used to check for OpenGL Runtime Errors. This will display the
	* file and line from which the error was detected. This function should
	* not be called directly. Instead use the macro GLCheckError.
	*
	* @return True if an error was detected.
	*/
	#define GLCheckError() _GLCheckError_(__FILE__, __LINE__)
	static bool _GLCheckError_(const string& file, int line)
	{
		GLenum errorCode = 0;
		bool wasError = false;
		do
		{
	#if defined(__ANDROID__)
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

    /**
      @brief Get Framebuffer Error string from the given enum
     * Using:
     *     https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glCheckFramebufferStatus.xhtml
     */
	#define GLGetFrameBufferError(x) _GLGetFrameBufferError_(x)
    static string _GLGetFrameBufferError_(GLuint error)
    {
        switch (error)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                return "GL_FRAMEBUFFER_UNDEFINED";
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT :
                return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			case GL_FRAMEBUFFER_UNSUPPORTED:
                return "GL_FRAMEBUFFER_UNSUPPORTED";
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
            default:
                return "Unknown Error";
        }
    }

#else
	#define GLCheckError()
	#define GLGetFrameBufferError()
#endif
