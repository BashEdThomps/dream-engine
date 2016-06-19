## Octronic::Dream

Dream is a Modular 3D Multimedia Engine.
* Written in C++
* Scriptable in Chaiscript.
* Modular architecture.

### Modularity

Dream allows you to implement modules for your favorite tech. A module may be an interface to a graphics library, input device, scripting runtime or other fancy piece of I/O tech. Those listed below are included as part of Dream.

* **Animation**
    * Dream (Linear Interpolated KeyFrame Animation)
* **Audio**
    * OpenAL
* **Graphics**
    * OpenGL (GLFW)
    * Vulkan (Future)
* **Input**
    * From GLFW
* **Physics**
    * Bullet2
* **Scripting**
    * ChaiScript

### Build Dependencies
* ALUT
* Assimp
* Bullet2
* ChaiScript
* OggVorbis
* OpenAL
* OpenGL
  * GLEW
  * GLFW
  * GLM
* SOIL

### Project Files

Dream defines it's own project structure with the following key attributes

* JSON Project definition file
* Accompanying structured assets directory

A Dream project contains two types of entity

#### Assets
An Asset describes something that can be held in a Scene, for example

* Animations
* Audio Files
* Lights
* Models
* Physics Objects
* Scripts
* Shaders

Assets come in two parts, AssetDefinitions and AssetInstances.

Asset definitions define the asset's properties. Asset Instances are assigned
to the SceneObject instances in the Scene's Scenegraph.

#### Scenes
A Scene is composed of a Scenegraph, a data structure that stores SceneObjects
and their relationships. All scenes start with a root SceneObject, which has
more SceneObject children and AssetInstances associated with it.

### Octronic::Dream::DreamTool
DreamTool is the project editor developed for Dream. DreamTool is a web application
that is designed to be 100% independent from the Dream engine application. Dream
projects created with the tool are stored by default in ~/.dreamtool under their
randomly generated UUIDs. DreamTool has the following attributes;

* Web Based
* Hosted Locally or SaaSy
* Implemented with NodeJS, Koa and AngularUI Bootstrap
* As well as loads of Angular Plug-Ins (Thanks to their respective devs :)
* Looks Pretty
