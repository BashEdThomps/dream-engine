# Dream
Dream is a 3D Multimedia and Game Engine.

## Key Features
* Platform Independant
* Written in C++
* OpenGL Graphics
* OpenAL Audio
* Scriptable in Lua
* Uses Bullet Physics & Collision Libraries
* Included Project Editor Written in Qt

## Architecture
The next sections give an architectural overview of the Dream engine.

### Project
Projects are the highest level of object in Dream. Projects are composed of a .json project definition file and an accompanying assets directory.

### Scenes
A Project contains one or more Scenes that are to be displayed. A Scene is composed of one or more SceneObjects.

### SceneObjects
SceneObjects are used to construct a 'Scenegraph' data tree that describes the relationship of objects in the Scene. SceneObjects are assigned parent-child relationships from which they inherit their transformation. Each SceneObject is assigned zero or more Assets to instanciate at runtime.

### Assets
A Project also contains Asset objects. An Asset describes something that can be assigned to a SceneObejct and placed in a Scene at runtime. AssetDefinitions are used to define an Asset's properties once and used multiple times to create Asset instances. AssetDefinitions are defined at the Project level so that they can be shared by many Scenes and SceneObjects. There are several types of AssetInstance available.

* Animation
* Audio
* Font
* Light
* Model
* Physics Object
* Script
* Shader
* Sprite

# DreamTool
DreamTool is the real-time project editor for Dream. It is cross-platrofm, written in Qt.
