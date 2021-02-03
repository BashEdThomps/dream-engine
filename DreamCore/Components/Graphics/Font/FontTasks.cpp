#include "FontRuntime.h"

#include "FontTasks.h"

namespace octronic::dream
{
    FontConstructionTask::FontConstructionTask(FontRuntime* rt)
        : GraphicsComponentTask("FontConstructionTask"),
        mFontRuntime(rt)
    {
    }

    void FontConstructionTask::execute()
    {
        LOG_TRACE("FontConstructionTask: {} Executing on Graphics thread",mFontRuntime->getNameAndUuidString());

		if (!mFontRuntime->loadIntoGL())
		{
			setState(TaskState::TASK_STATE_FAILED);
		}
        else
        {
			setState(TaskState::TASK_STATE_COMPLETED);
        }
    }

    FontDestructionTask::FontDestructionTask
    ()
        : GraphicsComponentDestructionTask("FontDestructionTask")
    {
    }

    void FontDestructionTask::execute()
    {
        LOG_TRACE("FontDestructionTask: with texture:{} vao:{} vbo:{}, Executing on Graphics thread",
                  mFontAtlasTexture, mFontVao, mFontVbo);

        glDeleteBuffers(1,&mFontVao);
        glDeleteBuffers(1,&mFontVbo);
        glDeleteTextures(1,&mFontAtlasTexture);

        setState(TaskState::TASK_STATE_COMPLETED);
    }

    void FontDestructionTask::setFontAtlasTexture(GLuint id)
    {
        mFontAtlasTexture = id;
    }

    void FontDestructionTask::setFontVao(GLuint id)
    {
        mFontVao = id;
    }

    void FontDestructionTask::setFontVbo(GLuint id)
    {
        mFontVbo = id;
    }
}
