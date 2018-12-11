#pragma once

/**
 * \mainpage
 * Dream is a 3D Multimedia and Game Engine.
 *
 * \section Key Features
 * * Platform Independant
 * * Written in C++
 * * OpenGL Graphics
 * * OpenAL Audio
 * * Bullet Physics & Collision
 * * Lua Scripting
 * * Project Editor Written with ImGui
 *
 * \section Architecture
 * This section will describe the key concepts used when working with Dream.
 *
 * \subsection Definition-vs-Runtime
 * Dream separates data into two domains.
 *
 * \subsubsection Definition
 * Objects are used to describe the Project as it was saved. For example,
 * initial transforms, physics properties, visibility, etc.
 * * \link Definition \endlink
 *
 * \subsubsection Runtime
 * Objects will change their properties as a Scene runs. This means that the
 * current state of the scene no longer matches it's initial state. Runtime
 * objects are used to store the current state of a scene, allowing variables to
 * be modified whilst Definition objects always retain the Scene's initial state.
 * * \link Dream::Runtime \endlink
 *
 * \subsection Project
 * Project objects are the highest level of object in Dream. A Project consists
 * primarily of a ProjectDefinition and ProjectRuntime. A ProjectDefinition
 * defines a Project, it's Scenes, SceneObjects and Assets. On disk a Project is
 * saved as a json file and an accompanying set of assets directories. These
 * directories store any data (Texture/Font/3D/Audio/etc.) required to run the
 * Project
 * * \link Dream::Project \endlink
 * * \link Dream::ProjectDefinition \endlink
 * * \link Dream::ProjectRuntime \endlink
 *
 * \subsection Scenes
 * A Project contains one or more Scenes that are to be displayed. A Scene is composed of one
 * or more SceneObjects. A Scene may for example represent a level, environment or stage within
 * your application.
 * * \link Dream::SceneDefinition \endlink
 * * \link Dream::SceneRuntime \endlink
 *
 * \subsection SceneObjects
 * Scene Objects are used to construct a 'Scenegraph' that describes the relationship of objects
 * within the Scene. A Scene has a single 'Root' object with n-children. Scene Objects are
 * arranged within a parent-child relationship tree, through which they can inherit their transformation.
 * Each Scene Object is assigned zero or more 'Assets Definitions' to instanciate at runtime.
 * * \link Dream::SceneObjectDefinition \endlink
 * * \link Dream::SceneObjectRuntime \endlink
 *
 * \subsection Assets
 * A Project will contain numerous Assets. An Asset describes something that can
 * be added into a Scene, such as a 3D Model, Audio Clip, Spline Path, etc.
 * (full list below).
 * * \link Dream::AssetDefinition \endlink
 * * \link Dream::AssetInstance \endlink
 *
 * \subsubsection Asset Definitions
 * AssetDefinition objects are used to define an Asset's initial properties.
 * They are defined once at the Project level and can be reused multiple times
 * within a Screen to create AssetInstance objects. These AssetInstances are
 * then assigned to SceneObjects within a Scene. AssetDefinition objects are
 * defined at the Project level so that they can be shared by SceneObjects.
 *
 * These are the types of Asset available in Dream.
 * * Animation
 *      * \link Dream::AnimationDefinition \endlink
 *      * \link Dream::AnimationInstance \endlink
 *
 * * Audio
 *      * \link Dream::AudioDefinition \endlink
 *      * \link Dream::AudioInstance \endlink
 *
 * * Font (A non-instanced Asset. Used by NanoVG)
 *      * \link Dream::FontDefinition \endlink
 *
 * * Light
 *      * \link Dream::LightDefinition \endlink
 *      * \link Dream::LightInstance \endlink
 *
 * * Model
 *      * \link Dream::ModelDefinition \endlink
 *      * \link Dream::ModelInstance \endlink
 *
 * * Particle Emitter
 *      * \link Dream::ParticleEmitterDefinition \endlink
 *      * \link Dream::ParticleEmitterInstance \endlink
 *
 * * Path
 *      * \link Dream::PathDefinition \endlink
 *      * \link Dream::PathInstance \endlink
 *
 * * Physics Object
 *      * \link Dream::PhysicsObjectDefinition \endlink
 *      * \link Dream::PhysicsObjectInstance \endlink
 *
 * * Script
 *      * \link Dream::ScriptDefinition \endlink
 *      * \link Dream::ScriptInstance \endlink
 *
 * * Shader
 *      * \link Dream::ShaderDefinition \endlink
 *      * \link Dream::ShaderInstance \endlink
 *
 * * Texture
 *      * \link Dream::TextureDefinition \endlink
 *      * \link Dream::TextureInstance \endlink
 *
 * \section Build Dependencies
 * Dream uses the following libraries.
 * * ALUT
 * * Assimp
 * * Bullet
 * * GLM
 * * GLEW
 * * GLFW
 * * Nlohmann JSON
 * * Lua 5.3
 * * OggVorbis
 * * OpenAL
 * * OpenGL
 * * SOIL
 * * SPDLOG
 * * TinySpline
 */
