## Dream

Dream is a modular and utilitarian 3D Engine. If you don't need it, don't add it
to your project! This allows for better conservation of runtime resources.

### Modularity

Dream works by allowing you to implement your favorite interfaces. Where an
interface may be to a graphics library, input device or other fancy piece of I/O
tech. Those listed below are included as part of Dream. There is just one rule I
ask that you follow when implementing an extension... '''use open software and
formats!'''

* '''AudioInterface'''
    * OpenAL
* '''GraphicsInterface'''
    * OpenGL
    * Vulkan (soon)
* '''PhysicsInterface'''
    * Bullet2
    * Bullet3 (soon)
* '''ScriptingInterface'''
    * v8 (Google's JavaScript Engine)

### Dream Project Information
* JSON Project Structure.
* Scriptable in JavaScript
* Supported Audio Formats
    * Ogg
    * Wav
* Supported Model Formats
    * Wavefront OBJ

### DreamTool - Project Editor
* Web Based
* Cross Platform
* Implemented with NodeJS
* Koa
* AngularUI Bootstrap
* And Loads of Angular Plug-Ins.
