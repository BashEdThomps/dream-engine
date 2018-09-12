# Dream
Dream is a 3D Multimedia and Game Engine.

## Key Features
* Platform Independant
* Written in C++
* OpenGL Graphics
* OpenAL Audio
* Bullet Physics & Collision
* Lua Scripting
* Gainput Input Handling
* Project Editor Written in Qt/C++

## Architecture

### Project
Projects are the highest level of object in Dream. 
A project is uses a .json file to describe it an an accompanying accompanying 
assets directory structure..

### Assets
A Project also contains 'Asset' objects. An Asset describes something that can 
be added  into a Scene such as a 3D model, spline or sound source.(full list below) 

#### AssetDefinitions 
AssetDefinitions are used to define an Asset's base properties. 
They are defined once at the Project level and can be used multiple times within
your Screen to create Assetnstances. These AssetInstances are then assigned to SceneObjects within a scene. AssetDefinitions are defined at the Project level so that they can be shared by many Scenes and SceneObjects. There are several types of AssetInstance available.

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

### Scenes
A Project contains one or more Scenes that are to be displayed. A Scene is composed of one or more SceneObjects.

### SceneObjects
SceneObjects are used to construct a 'Scenegraph' that describes the relationship of objects in the Scene. A scene has a single 'Root' SceneObject. SceneObjects are assigned arranged with a parent-child relationship, through which they can inherit their transformation. Each SceneObject is assigned zero or more 'Assets' to use at runtime.

## DreamTool

DreamTool is the project editor for Dream. It is cross-platrofm, written in Qt.
