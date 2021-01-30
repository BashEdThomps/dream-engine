#include <jni.h>
#include <DreamCore.h>
#include "droid/AndroidStorageManager.h"
#include "droid/AndroidWindowComponent.h"
#include "droid/AndroidAudioComponent.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

using octronic::dream::android::AndroidAudioComponent;
using octronic::dream::android::AndroidWindowComponent;
using octronic::dream::android::AndroidStorageManager;
using octronic::dream::GraphicsComponent;
using octronic::dream::Project;
using octronic::dream::ProjectDefinition;
using octronic::dream::ProjectRuntime;
using octronic::dream::SceneDefinition;
using octronic::dream::SceneRuntime;
using octronic::dream::ProjectDirectory;
using octronic::dream::Directory;
using octronic::dream::ScriptPrintListener;
using octronic::dream::ScriptComponent;

// Script Print Listener ===========================================================================

class DefaultPrintListener : public ScriptPrintListener
{
public:
    DefaultPrintListener() {}
    ~DefaultPrintListener() {}

    void onPrint(const string& str) override
    {
        LOG_CRITICAL("LUA PrintListener: {}", str);
    }
};

DefaultPrintListener pl;

// Static Variables ================================================================================
AndroidWindowComponent  windowComponent;
AndroidAudioComponent   audioComponent;
AndroidStorageManager*  storageManager = nullptr;
Project*                project = nullptr;
ProjectDefinition*      projectDefinition = nullptr;
ProjectRuntime*         projectRuntime = nullptr;
SceneDefinition*        startupSceneDefinition = nullptr;
SceneRuntime*           activeSceneRuntime = nullptr;
ProjectDirectory*       projectDirectory = nullptr;

extern "C"
{
    JNIEXPORT void JNICALL Java_com_octronic_dream_DreamJNI_init(JNIEnv* env, jobject obj, jobject assetManager);
    JNIEXPORT void JNICALL Java_com_octronic_dream_DreamJNI_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_octronic_dream_DreamJNI_step(JNIEnv* env, jobject obj);
};

static const char* GetGLString(GLenum s)
{
    return (const char*)glGetString(s);
}

JNIEXPORT void JNICALL
Java_com_octronic_dream_DreamJNI_init(JNIEnv* env, jobject obj, jobject assetManager)
{
    auto android_logger = spdlog::android_logger_mt("android", "Dream");
    spdlog::set_default_logger(android_logger);
    android_logger->flush_on(LOG_LEVEL_TRACE);
    spdlog::set_pattern("[%H:%M:%S]%l: %v");

    LOG_LEVEL(LOG_LEVEL_TRACE);

    LOG_DEBUG("Version:    {}", GetGLString(GL_VERSION));
    LOG_DEBUG("Vendor:     {}", GetGLString(GL_VENDOR));
    LOG_DEBUG("Renderer:   {}", GetGLString(GL_RENDERER));
    LOG_DEBUG("Extensions: {}", GetGLString(GL_EXTENSIONS));

    ScriptComponent::AddPrintListener(&pl);
    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3."))
    {
        LOG_DEBUG("DreamJNI: Using OpenGL ES 3");
    }
    else if (strstr(versionStr, "OpenGL ES 2."))
    {
        LOG_DEBUG("DreamJNI: Using OpenGL ES 2");
    }
    else
    {
        LOG_ERROR("DreamJNI: Unsupported OpenGL ES version");
    }


    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);

    // Core Starts ---------------------------------------------------------------------------------
    storageManager = new AndroidStorageManager(mgr);

    LOG_INFO("DreamJNI: Starting...");

    if (!audioComponent.init())
    {
        LOG_ERROR("DreamJNI: Unable to init audio component");
        return;
    }

    if(!windowComponent.init())
    {
        LOG_ERROR("DreamJNI: Could not init window component");
        return;
    }

    string dir = "";
    projectDirectory = new ProjectDirectory(storageManager);
    Directory* d = storageManager->openDirectory(dir);

    LOG_DEBUG("DreamJNI: Opening project {}",dir);
    project = projectDirectory->openFromDirectory(d);
    if(!project)
    {
        LOG_ERROR("DreamJNI: ");
        return;
    }

    project->setAudioComponent(&audioComponent);
    project->setWindowComponent(&windowComponent);
    project->createProjectRuntime();

    projectRuntime = project->getRuntime();
    projectDefinition = project->getDefinition();

    if (!projectDefinition)
    {
        LOG_ERROR("DreamJNI: ");
        return;
    }

    startupSceneDefinition = nullptr;
    activeSceneRuntime = nullptr;
    startupSceneDefinition = projectDefinition->getStartupSceneDefinition();

    if (startupSceneDefinition && projectRuntime)
    {
        activeSceneRuntime = new SceneRuntime(startupSceneDefinition,projectRuntime);
        if(activeSceneRuntime->useDefinition())
        {
            projectRuntime->addSceneRuntime(activeSceneRuntime);
            projectRuntime->setSceneRuntimeAsActive(activeSceneRuntime->getUuid());
            LOG_ERROR("\n\nINIT COMPLETE - ACTIVE RUNTIME SET\n\n");
        }
        else
        {
            LOG_ERROR("DreamJNI: Unable to use startup scene runtime");
            delete activeSceneRuntime;
            activeSceneRuntime = nullptr;
        }
    }
    spdlog::default_logger()->flush();
}

JNIEXPORT void JNICALL
Java_com_octronic_dream_DreamJNI_resize(JNIEnv* env, jobject obj, jint width, jint height)
{
    LOG_TRACE("Main: Window resized to {}x{}", width, height);
    windowComponent.setWidth(width);
    windowComponent.setHeight(height);
    windowComponent.setWindowSizeChangedFlag(true);
    spdlog::default_logger()->flush();
}

JNIEXPORT void JNICALL
Java_com_octronic_dream_DreamJNI_step(JNIEnv* env, jobject obj)
{
    LOG_TRACE("DreamJNI: step");
    if (projectRuntime == nullptr)
    {
        LOG_ERROR("DreamJNI: No ProjectRuntime");
        return;
    }
    SceneRuntime* asr = projectRuntime->getActiveSceneRuntime();

    if(asr != nullptr)
    {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        projectRuntime->updateAll();
        windowComponent.swapBuffers();
    }
    else
    {
        glClearColor(1.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        LOG_ERROR("DreamJNI: No Active SceneRuntime");
    }
    spdlog::default_logger()->flush();
    //assert(0);
}

