#include "FontRuntime.h"

#include "FontTasks.h"

namespace octronic::dream
{
    FontLoadIntoGLTask::FontLoadIntoGLTask(ProjectRuntime* pr, FontRuntime* rt)
        : GraphicsTask(pr, "FontLoadIntoGLTask"),
        mFontRuntime(rt)
    {
    }

    void FontLoadIntoGLTask::execute()
    {
        LOG_TRACE("FontLoadIntoGLTask: {} Executing on Graphics thread",mFontRuntime->getNameAndUuidString());

		if (!mFontRuntime->loadIntoGL())
		{
			setState(TaskState::TASK_STATE_FAILED);
		}
        else
        {
			setState(TaskState::TASK_STATE_COMPLETED);
        }
    }

    FontRemoveFromGLTask::FontRemoveFromGLTask
    (ProjectRuntime* pr)
        : GraphicsDestructionTask(pr, "FontRemoveFromGLTask")
    {
    }

    void FontRemoveFromGLTask::execute()
    {
        LOG_TRACE("FontRemoveFromGLTask: with texture:{} vao:{} vbo:{}, Executing on Graphics thread",
                  mFontAtlasTexture, mFontVao, mFontVbo);

        glDeleteBuffers(1,&mFontVao);
        glDeleteBuffers(1,&mFontVbo);
        glDeleteTextures(1,&mFontAtlasTexture);

        setState(TaskState::TASK_STATE_COMPLETED);
    }

    void FontRemoveFromGLTask::setFontAtlasTexture(GLuint id)
    {
        mFontAtlasTexture = id;
    }

    void FontRemoveFromGLTask::setFontVao(GLuint id)
    {
        mFontVao = id;
    }

    void FontRemoveFromGLTask::setFontVbo(GLuint id)
    {
        mFontVbo = id;
    }
}
