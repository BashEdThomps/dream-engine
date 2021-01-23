/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>
#include <DreamCore.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


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
    /*
    auto android_logger = spdlog::android_logger_mt("android", "Dream");
    spdlog::set_default_logger(android_logger);

    LOG_LEVEL(LOG_LEVEL_DEBUG);

    LOG_DEBUG("Version:    {}", GetGLString(GL_VERSION));
    LOG_DEBUG("Vendor:     {}", GetGLString(GL_VENDOR));
    LOG_DEBUG("Renderer:   {}", GetGLString(GL_RENDERER));
    LOG_DEBUG("Extensions: {}", GetGLString(GL_EXTENSIONS));

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3."))
    {
        LOG_DEBUG("CanDashJNI: Using OpenGL ES 3");
        ShaderManager::SetGLVersion(Constants::SHADER_GL_VERSION_GLES3);
    }
    else if (strstr(versionStr, "OpenGL ES 2."))
    {
        LOG_DEBUG("CanDashJNI: Using OpenGL ES 2");
        ShaderManager::SetGLVersion(Constants::SHADER_GL_VERSION_GLES2);
    }
    else
    {
        LOG_ERROR("CanDashJNI: Unsupported OpenGL ES version");
    }

    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    AndroidFileManager* fm = new AndroidFileManager(mgr);

    // Core Starts ---------------------------------------------------------------------------------
     */
}

JNIEXPORT void JNICALL
Java_com_octronic_dream_DreamJNI_resize(JNIEnv* env, jobject obj, jint width, jint height)
{
    //LOG_TRACE("Main: Window resized to {}x{}", width, height);
}

JNIEXPORT void JNICALL
Java_com_octronic_dream_DreamJNI_step(JNIEnv* env, jobject obj)
{
    //LOG_TRACE("DreamJNI: step");
}

