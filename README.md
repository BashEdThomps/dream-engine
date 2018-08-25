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
This section give an architectural overview of the engine.

### Project
Projects are the highest level of object in Dream. A project is composed of a .json project definition file and an accompanying assets directory.

### Scenes
A Project contains one or more Scenes that are to be displayed. A Scene is composed of one or more SceneObjects.

### SceneObjects
SceneObjects are used to construct a 'Scenegraph' that describes the relationship of objects in the Scene. A scene has a single 'Root' SceneObject. SceneObjects are assigned arranged with a parent-child relationship, through which they can inherit their transformation. Each SceneObject is assigned zero or more 'Assets' to use at runtime.

### Assets
A Project also contains 'Asset' objects. An Asset describes something that can be placed in a Scene such as a 3D model, spline or sound source. 'AssetDefinitions' are used to define an Asset's properties. They are defined once at the Project level and can be used multiple times to create Asset instances that are assigned to SceneObjects within a scene. AssetDefinitions are defined at the Project level so that they can be shared by many Scenes and SceneObjects. There are several types of AssetInstance available.

* Audio
* Font
* Light
* Model
* Particle Emitter
* Path
* Physics Object
* Script
* Shader
* Sprite

# DreamTool
DreamTool is the project editor for Dream. It is cross-platrofm, written in Qt.
