#pragma once

/**
 * \mainpage
 * Dream is a 3D Multimedia and Game Engine.
 *
 * \section Overview
 * * Platform Independant
 * * Written in C++
 * * Using OpenGL for Graphics
 * * Using OpenAL for Audio
 * * Using Bullet for Physics & Collision
 * * Using Lua for Scripting
 * * Project Editor Written with ImGui
 *
 * \section BuildTargets Build Targets
 *
 * <b>DreamCore</b>
 * <br>
 * The core library.
 *
 * <b>DreamTool</b>
 * <br>
 * A 'Creator' application that allows users to create and modify Dream Projects.
 *
 * <b>DreamGLFW</b>
 * <br>
 * A 'Consumer' application that can run Dream Projects on desktop systems
 * (Windows/macOS/Linux)
 *
 * <b>DreamAndroid</b>
 * <br>
 * A 'Consumer' application that can run Dream Projects on mobile
 * devices running the Android Operating System.
 *
 * <b>Documentation</b>
 * <br>
 * Generates this documentation set with Doxygen.
 *
 *
 * \section Architecture Architecture
 * This section will describe the key concepts used when working with Dream.
 *
 * \subsection Definition-vs-Runtime
 * Dream separates data into two domains.
 *
 * \subsubsection Definition Definition
 * Objects are used to describe the Project as it was saved. For example,
 * initial transforms, physics properties, visibility, etc.
 * * \link octronic::dream::Definition \endlink
 *
 * \subsubsection Runtime Runtime
 * Objects will change their properties as a Scene runs. This means that the
 * current state of the scene no longer matches it's initial state. Runtime
 * objects are used to store the current state of a scene, allowing variables to
 * be modified whilst Definition objects always retain the Scene's initial state.
 * * \link octronic::dream::Runtime \endlink
 *
 * \subsection Project Project
 * Project objects are the highest level of object in Dream. A Project consists
 * primarily of a ProjectDefinition and ProjectRuntime. A ProjectDefinition
 * defines a Project, it's Scenes, Entities and Assets. On disk a Project is
 * saved as a json file and an accompanying set of assets directories. These
 * directories store any data (Texture/Font/3D/Audio/etc.) required to run the
 * Project
 * * \link octronic::dream::Project \endlink
 * * \link octronic::dream::ProjectDefinition \endlink
 * * \link octronic::dream::ProjectRuntime \endlink
 *
 * \subsection Scenes Scenes
 * A Project contains one or more Scenes that are to be displayed. A Scene is composed of one
 * or more Entities. A Scene may for example represent a level, environment or stage within
 * your application.
 * * \link octronic::dream::SceneDefinition \endlink
 * * \link octronic::dream::SceneRuntime \endlink
 *
 * \subsection Entities Entities
 * Entities are used to construct a 'Scenegraph' that describes the relationship of objects
 * within the Scene. A Scene has a single 'Root' Entity with n-children. Entities are
 * arranged within a parent-child relationship tree, through which they can inherit their transformation.
 * Each Entity is assigned zero or more 'Assets Definitions' to instanciate at runtime.
 * * \link octronic::dream::EntityDefinition \endlink
 * * \link octronic::dream::EntityRuntime \endlink
 *
 * \subsection Assets Assets
 * A Project will contain numerous Assets. An Asset describes something that can
 * be added into a Scene, such as a 3D Model, Audio Clip, Spline Path, etc.
 * (full list below).
 * * \link octronic::dream::AssetDefinition \endlink
 * * \link octronic::dream::AssetRuntime \endlink
 *
 * \subsubsection AssetDefinitions Asset Definitions
 * AssetDefinition objects are used to define an Asset's initial properties.
 * They are defined once at the Project level and can be reused multiple times
 * within a Screen to create AssetRuntime objects. These AssetRuntimes are
 * then assigned to Entities within a Scene. AssetDefinition objects are
 * defined at the Project level so that they can be shared by Entities.
 *
 * These are the types of Asset available in Dream.
 * * <b>Animation</b>
 *      * \link octronic::dream::AnimationDefinition \endlink
 *      * \link octronic::dream::AnimationRuntime \endlink
 *
 * * <b>Audio</b>
 *      * \link octronic::dream::AudioDefinition \endlink
 *      * \link octronic::dream::AudioRuntime \endlink
 *
 * * <b>Font</b>
 *      * \link octronic::dream::FontDefinition \endlink
 *      * \link octronic::dream::FontRuntime \endlink
 *
 * * <b>Light</b>
 *      * \link octronic::dream::LightDefinition \endlink
 *      * \link octronic::dream::LightRuntime \endlink
 *
 * * <b>Model</b>
 *      * \link octronic::dream::ModelDefinition \endlink
 *      * \link octronic::dream::ModelRuntime \endlink
 *
 * * <b>Path</b>
 *      * \link octronic::dream::PathDefinition \endlink
 *      * \link octronic::dream::PathRuntime \endlink
 *
 * * <b>Physics Object</b>
 *      * \link octronic::dream::PhysicsObjectDefinition \endlink
 *      * \link octronic::dream::PhysicsObjectRuntime \endlink
 *
 * * <b>Script</b>
 *      * \link octronic::dream::ScriptDefinition \endlink
 *      * \link octronic::dream::ScriptRuntime \endlink
 *
 * * <b>Shader</b>
 *      * \link octronic::dream::ShaderDefinition \endlink
 *      * \link octronic::dream::ShaderRuntime \endlink
 *
 * * <b>Texture</b>
 *      * \link octronic::dream::TextureDefinition \endlink
 *      * \link octronic::dream::TextureRuntime \endlink
 *
 * \section BuildDependencies Build Dependencies
 * Dream uses the following Dependencies.
 * * [Assimp](https://github.com/assimp/assimp)
 * * [Bullet](https://github.com/bulletphysics/bullet3)
 * * [Freetype](https://www.freetype.org/)
 * * [GLAD](https://glad.dav1d.de/)
 * * [GLFW](https://www.glfw.org/)
 * * [GLM](https://github.com/g-truc/glm)
 * * [ImGui](https://github.com/ocornut/imgui)
 * * [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
 * * [ImTextEditor](https://github.com/BalazsJako/ImGuiColorTextEdit)
 * * [Lua](http://www.lua.org/)
 * * [NativeFileDialog](https://github.com/mlabbe/nativefiledialog)
 * * [Ogg](https://www.xiph.org/ogg/)
 * * [STB Image](https://github.com/nothings/stb)
 * * [SPDLOG](https://github.com/gabime/spdlog)
 * * [tinyspline](https://github.com/msteinbeck/tinyspline)
 * * [tweeny](https://github.com/mobius3/tweeny)
 * * [Vorbis](https://xiph.org/vorbis/)
 **/

