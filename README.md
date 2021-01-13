# Dream

( Data Rules Everything Around Me ) 

# What?

Dream is a Demo and Game Engine.

## Key Features
* Platform Independant
* Written in C++
* OpenGL Graphics
* OpenAL Audio
* Freetype Font Engine
* Bullet Physics & Collision
* Lua Scripting
* Project Editor Written with ImGui

## Architecture
This section will describe the key concepts used when working with Dream.

### Project
Projects are the highest level of object in Dream.  A Project consists of a json 
file and an assets directory structure. The Project holds definitions of Scenes, 
Entities and AssetDefinitions.

### Assets
A Project will contain several 'Asset' objects. An Asset describes something 
that can be added into a Scene, such as a 3D model, Audio Clip, Spline Path, etc. 
(full list below).

#### Asset Definitions
Asset Definitions are used to define an Asset's properties at start-up.
They are defined once at the Project level and can be reused multiple times 
within your Screen to create Asset Instances. These Asset Instances are then 
assigned to Entities within a scene. Asset Definitions are defined at the Project 
level so that they can be shared by many Scenes and Entities. There are several 
types of Asset available in Dream.

* Animation
* Audio
* Font
* Light
* Model
* Object Emitter
* Path
* Physics Object
* Script
* Shader
* Sprite
* Texture

### Scenes
A Project contains one or more Scenes that are to be displayed. A Scene is 
composed of one or more Entities. A Scene may for example represent a level, 
environment or stage within your application.

### Entities
Entities are used to construct a 'Scenegraph' that describes the relationship of 
objects within the Scene. A Scene has a single 'Root' Entity with 0-n children. 
Entities are arranged within a parent-child relationship tree, through which they 
can inherit their initial transformation. Each Entity is assigned zero or more 
'Assets Definitions' to instanciate at startup.

## DreamTool
DreamTool is the Project editor for Dream. It is cross-platrofm, written with 
Dear ImGui. It allows you to build and test Scenes that can later be run using 
a standalone Dream engine frontend (DreamGLFW/DreamAndroid). Both DreamTool and 
Frontend applications use the same engine codebase 'DreamCore' so runtime 
results will be identical.

## Build Dependencies
Dream uses the following libraries.

* [Assimp](https://github.com/assimp/assimp)
* [Bullet](https://github.com/bulletphysics/bullet3)
* [Freetype](https://www.freetype.org/)
* [GLAD](https://glad.dav1d.de/)
* [GLFW](https://www.glfw.org/)
* [GLM](https://github.com/g-truc/glm)
* [ImGui](https://github.com/ocornut/imgui)
* [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo)
* [ImTextEditor](https://github.com/BalazsJako/ImGuiColorTextEdit)
* [Lua](http://www.lua.org/)
* [NativeFileDialog](https://github.com/mlabbe/nativefiledialog)
* [Ogg](https://www.xiph.org/ogg/)
* [SOIL](https://github.com/littlstar/soil)
* [SPDLOG](https://github.com/gabime/spdlog)
* [tinyspline](https://github.com/msteinbeck/tinyspline)
* [tweeny](https://github.com/mobius3/tweeny)
* [Vorbis](https://xiph.org/vorbis/)
