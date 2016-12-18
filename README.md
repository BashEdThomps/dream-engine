## Dream

Dream is a Modular 3D Multimedia Engine.
* Written in C++
* Scriptable in Lua.
The proceeding sections give an architectural overview of the Dream engine.

### Project
Projects are the highest level of object in Dream. Projects are composed of a .json project definition
file and an accompanying assets directory.

#### Scenes
A Project contains one or more Scenes that are to be displayed. A Scene is composed of a SceneObjects
that have parent-child relationships and are arranged into a scene graph data structure.

#### Asset Instances and Definitions
A Project also contains AssetDefinition objects. An AssetDefinition describes something that can be
assigned to a SceneObejct and placed in a Scene at runtime. Asset Definitions define an asset's
properties and are designed to be defined once and used multiple times to create AssetInstances.
AssetInstances are inflated from AssetDefinition which can be thought of as blueprints. Instances are
then assigned to a SceneObject within the Scene. There are several types of AssetInstance available.

* Animation
* Audio
* Lights
* Models
* Physics Objects
* Scripts
* Shaders
* Sprites
* Fonts

### DreamTool

DreamTool is the Project editor for Dream. It is a web application that is designed to be 100%
independent from the DreamEngine application. Projects that are created with the tool are stored by
default in ~/.dreamtool under their randomly generated UUIDs.

* Web Based
* Hosted Locally or SaaSy
* Implemented with NodeJS, Koa and AngularUI Bootstrap
* As well as loads of Angular Plug-Ins (with thanks to their respective devs :)
