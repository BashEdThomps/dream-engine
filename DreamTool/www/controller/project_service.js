App.service('ProjectService',
    ["Blob","FileSaver","UtilService","ApiService",
     function(Blob, FileSaver, UtilService, ApiService) {

    // Plugin Constants
    this.PLUGIN_ANIMATION_DREAM  = "dreamAnim";
    this.PLUGIN_AUDIO_OPENAL     = "openAL";
    this.PLUGIN_INPUT_GLFW       = "glfwInput"
    this.PLUGIN_SCRIPTING_CHAI   = "chai";
    this.PLUGIN_PHYSICS_BULLET2  = "bullet2";
    this.PLUGIN_VIDEO_OPENGL     = "openGL";

    // Asset Constants
    this.ASSET_TYPE_NAME_ANIMATION    = "Animation";
    this.ASSET_TYPE_ANIMATION         = "animation";
    this.ASSET_FORMAT_NAME_DREAM      = "Dream Animation";
    this.ASSET_FORMAT_ANIMATION_DREAM = "dream";

    this.ASSET_TYPE_NAME_AUDIO  = "Audio";
    this.ASSET_TYPE_AUDIO       = "audio";
    this.ASSET_FORMAT_NAME_OGG  = "Ogg Vorbis";
    this.ASSET_FORMAT_AUDIO_OGG = "ogg";
    this.ASSET_FORMAT_NAME_WAV  = "Wave File";
    this.ASSET_FORMAT_AUDIO_WAV = "wav";

    this.ASSET_TYPE_LIGHT = "light";
    this.ASSET_TYPE_NAME_LIGHT = "Light";

    this.ASSET_TYPE_NAME_PHYSICS_OBJECT = "Physics Object";
    this.ASSET_TYPE_PHYSICS_OBJECT      = "physicsObject";

    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_SPHERE = "Shpere";
    this.ASSET_FORMAT_PHYSICS_OBJECT_SPHERE = "btShpereShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_BOX = "Box";
    this.ASSET_FORMAT_PHYSICS_OBJECT_BOX = "btBoxShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CYLINDER = "Cylinder";
    this.ASSET_FORMAT_PHYSICS_OBJECT_CYLINDER = "btCylinderShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CAPSULE = "Capsule";
    this.ASSET_FORMAT_PHYSICS_OBJECT_CAPSULE = "btCapsuleShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONE = "Cone";
    this.ASSET_FORMAT_PHYSICS_OBJECT_CONE = "btConeShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_MULTI_SPHERE = "Multi Sphere";
    this.ASSET_FORMAT_PHYSICS_OBJECT_MULTI_SPHERE = "btMultiSphereShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONVEX_HULL = "Convex Hull";
    this.ASSET_FORMAT_PHYSICS_OBJECT_CONVEX_HULL = "btConvexHullShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONVEX_TRIANGLE_MESH = "Convex Triangle Mesh";
    this.ASSET_FORMAT_PHYSICS_OBJECT_CONVEX_TRIANGLE_MESH = "btConvexTriangleMeshShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONVEX_BVH_TRIANGLE_MESH = "Bvh Triangle Mesh";
    this.ASSET_FORMAT_PHYSICS_OBJECT_CONVEX_BVH_TRIANGLE_MESH = "btBvhTriangleMeshShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_HEIGHTFIELD_TERRAIN = "Heightfield Terrain";
    this.ASSET_FORMAT_PHYSICS_OBJECT_HEIGHTFIELD_TERRAIN = "btHeightfieldTerrainShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_STATIC_PLANE = "Static Plane";
    this.ASSET_FORMAT_PHYSICS_OBJECT_STATIC_PLANE = "btStaticPlaneShape";
    this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_COMPOUND = "Compound";
    this.ASSET_FORMAT_PHYSICS_OBJECT_COMPOUND = "btCompoundShape";

    this.ASSET_TYPE_NAME_MODEL     = "Model";
    this.ASSET_TYPE_MODEL          = "model";
    this.ASSET_FORMAT_NAME_ASSIMP  = "Assimp";
    this.ASSET_FORMAT_MODEL_ASSIMP = "assimp";

    this.ASSET_TYPE_NAME_SCRIPT   = "Script";
    this.ASSET_TYPE_SCRIPT        = "script";
    this.ASSET_FORMAT_NAME_CHAI   = "ChaiScript";
    this.ASSET_FORMAT_SCRIPT_CHAI = "chai";

    this.ASSET_TYPE_NAME_SHADER       = "Shader";
    this.ASSET_TYPE_SHADER            = "shader";
    this.ASSET_FORMAT_SHADER_VERTEX   = "vertex";
    this.ASSET_FORMAT_SHADER_FRAGMENT = "fragment";

    this.ASSET_TYPES = [
      // Animation
      {
        name: this.ASSET_TYPE_NAME_ANIMATION,
        type: this.ASSET_TYPE_ANIMATION,
        formats: [
          {
            name:   this.ASSET_FORMAT_NAME_DREAM,
            format: this.ASSET_FORMAT_ANIMATION_DREAM
          }
        ]
      },
      // Audio
      {
        name: this.ASSET_TYPE_NAME_AUDIO,
        type: this.ASSET_TYPE_AUDIO,
        formats: [
          {
            name:   this.ASSET_FORMAT_NAME_OGG,
            format: this.ASSET_FORMAT_AUDIO_OGG
          },
          {
            name:   this.ASSET_FORMAT_NAME_WAV,
            format: this.ASSET_FORMAT_AUDIO_WAV
          }
        ]
      },
      // Light
      {
        name:    this.ASSET_TYPE_NAME_LIGHT,
        type:    this.ASSET_TYPE_LIGHT,
        formats: []
      },
      // Physics Object
      {
        name: this.ASSET_TYPE_NAME_PHYSICS_OBJECT,
        type: this.ASSET_TYPE_PHYSICS_OBJECT,
        formats: [
          {
            name:   this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_SPHERE,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_SPHERE
          },{
            name:   this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_BOX,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_BOX
          },{
            name:   this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CYLINDER,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_CYLINDER
          },{
            name:   this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CAPSULE,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_CAPSULE
          },{
            name:   this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONE,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_CONE
          },{
            name:   this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_MULTI_SPHERE,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_MULTI_SPHERE
          },{
            name: this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONVEX_HULL,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_CONVEX_HULL
          },{
            name: this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONVEX_TRIANGLE_MESH,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_CONVEX_TRIANGLE_MESH
          },{
            name: this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_CONVEX_BVH_TRIANGLE_MESH,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_CONVEX_BVH_TRIANGLE_MESH
          },{
            name: this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_HEIGHTFIELD_TERRAIN,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_HEIGHTFIELD_TERRAIN
          },{
            name: this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_STATIC_PLANE,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_STATIC_PLANE
          },{
            name: this.ASSET_FORMAT_NAME_PHYSICS_OBJECT_COMPOUND,
            format: this.ASSET_FORMAT_PHYSICS_OBJECT_COMPOUND
          }
        ]
      },
      // Model
      {
        name: this.ASSET_TYPE_NAME_MODEL,
        type: this.ASSET_TYPE_MODEL,
        formats: [
          {
            name: this.ASSET_FORMAT_NAME_ASSIMP,
            format: this.ASSET_FORMAT_MODEL_ASSIMP
          }
        ]
      },
      // Shader
      {
        name: this.ASSET_TYPE_NAME_SHADER,
        type: this.ASSET_TYPE_SHADER,
        formats: []
      },
      // Script
      {
        name: this.ASSET_TYPE_NAME_SCRIPT,
        type: this.ASSET_TYPE_SCRIPT,
        formats: [
          {
            name:   this.ASSET_FORMAT_NAME_CHAI,
            format: this.ASSET_FORMAT_SCRIPT_CHAI,
          }
        ]
      }
    ];

    this.getFormatsByAssetType = function(assetType,callback) {
      var retval = [];
      this.ASSET_TYPES.forEach(function(type) {
        if (type.name == assetType) {
          retval = type.formats;
        }
      });
      callback(retval);
    };

    this.getAssetTypes = function(callback) {
      var retval = [];
      this.ASSET_TYPES.forEach(function(assetType) {
        retval.push(assetType.type);
      });
      callback(retval);
    };

    this.getAssetTypesObject = function(callback) {
      callback(this.ASSET_TYPES);
    };

    this.getAssetTypeNames = function(callback) {
      var retval = [];
      this.ASSET_TYPES.forEach(function(assetType) {
        retval.push(assetType.name);
      });
      callback(retval);
    };

    this.getAnimationAssetFormats = function(callback) {
      var ps = this;
      this.ASSET_TYPES.forEach(function(assetType){
        if (assetType.type == ps.ASSET_TYPE_ANIMATION) {
          callback(assetType.formats);
        }
      });
    };

    this.getAudioAssetFormats = function(callback) {
      var ps = this;
      this.ASSET_TYPES.forEach(function(assetType){
        if (assetType.type == ps.ASSET_TYPE_AUDIO) {
          callback(assetType.formats);
        }
      });
    };

    this.getPhysicsObjectAssetFormats = function(callback) {
      var ps = this;
      this.ASSET_TYPES.forEach(function(assetType){
        if (assetType.type == ps.ASSET_TYPE_PHYSICS_OBJECT) {
          callback(assetType.formats);
        }
      });
    };

    this.getModelAssetFormats = function(callback) {
      var ps = this;
      this.ASSET_TYPES.forEach(function(assetType){
        if (assetType.type == ps.ASSET_TYPE_MODEL) {
          callback(assetType.formats);
        }
      });
    };

    this.getScriptAssetFormats = function(callback) {
      var ps = this;
      this.ASSET_TYPES.forEach(function(assetType){
        if (assetType.type == ps.ASSET_TYPE_SCRIPT) {
          callback(assetType.formats);
        }
      });
    };

    // Functions && Variables --------------------------------------------------

    this.project = null;

    this.isProjectOpen = function() {
        return this.project !== null;
    };

    this.getProject = function() {
        return this.project;
    };

    this.getProjectUUID = function() {
        return this.project.uuid;
    };

    this.getName = function() {
        return this.project.name;
    };

    this.setName = function(name) {
        this.project.isModified = true;
        this.project.name = name;
    };

    this.setAuthor = function(author) {
        this.project.isModified = true;
        this.project.author = author;
    };

    this.setDescription = function(description) {
        this.project.isModified = true;
        this.project.description = description;

    };

    this.setStartupScene = function(startupScene) {
      this.project.isModified = true;
      this.project.startupScene = startupScene;
    }

    this.updateProject = function(project) {
        this.setName        (project.name);
        this.setAuthor      (project.author);
        this.setDescription (project.description);

        this.setOpenAL  (project.openAL);
        this.setChai     (project.chai);
        this.setBullet2 (project.bullet2);
        this.setOpenGL  (project.openGL);

        this.setStartupScene(project.startupScene);
    };

    this.getSceneList = function(callback) {
        var retval = [];
        this.project.scenes.forEach(function(nextScene){
          retval.push({
            uuid: nextScene.uuid,
            name: nextScene.name});
        });
        callback(retval);
    };

    this.pushScene = function(scene) {
        this.project.isModified = true;
        this.project.scenes.push(scene);
    };

    this.getSceneByUUID = function(name, callback) {
        var retval = null;
        this.project.scenes.forEach(function (scene){
            if (scene.uuid === name) {
                retval = scene;
            }
        });
        callback(retval);
    };

    this.removeScene = function(scene) {
        var index = this.project.scenes.indexOf(scene);
        this.project.scenes.splice(index,1);
    };

    this.pushAsset = function(asset) {
        this.project.isModified = true;
        this.project.assets.push(asset);
    };

    this.getAssetByUUID = function(uuid, callback) {
        var retval = null;

        if (this.project === null) {
            callback(retval);
            return;
        }

        this.project.assets.forEach(function (rsc) {
            if (rsc.uuid === uuid) {
                retval = rsc;
            }
        });
        callback(retval);
    };

    this.removeAsset = function(asset) {
        var index = this.project.assets.indexOf(asset);
        this.project.assets.splice(index,1);
    };


    this.addAssetInstanceToSceneObject = function(sceneUUID,sceneObjectUUID,assetUUID,callback){
        var proj = this;
        console.log("Adding Asset Instance",sceneObjectUUID,"to",assetUUID);
        this.getSceneByUUID(sceneUUID,function(scene){
            proj.getSceneObjectByUUID(scene,sceneObjectUUID,function(sceneObject){
                if (sceneObject.assetInstances === undefined) {
                    sceneObject.assetInstances = [];
                }
                if (sceneObject.assetInstances.indexOf(assetUUID) < 0) {
                    sceneObject.assetInstances.push(assetUUID);
                    callback(true);
                } else {
                    console.log(sceneObjectUUID,"allready has instance to",assetUUID);
                    callback(false);
                }
            });
        });
    };

    this.removeAssetInstanceFromSceneObject = function(sceneUUID,sceneObjectUUID,assetUUID){
        var proj = this;
        console.log("Removing Asset Instance",assetUUID,"from",sceneObjectUUID);
        this.getSceneByUUID(sceneUUID, function(scene){
            proj.getSceneObjectByUUID(scene,sceneObjectUUID,function(sceneObject){
                var indexOf = sceneObject.assetInstances.indexOf(assetUUID);
                if (indexOf > -1) {
                    sceneObject.assetInstances.splice(indexOf,1);
                } else {
                    console.log(sceneObjectUUID,": unable to remove instance of",assetUUID);
                }
            });
        });
    };

    this.getSceneObjectByUUID = function(scene,uuid,callback) {
      var ps = this;
      scene.objects.forEach(function(sceneObject) {
        ps.lookForSceneObject(sceneObject,uuid,callback);
      });
    };

    this.lookForSceneObject = function(sceneObject, uuid, callback) {
      var ps = this;
      if (sceneObject.uuid == uuid) {
        callback(sceneObject);
      } else {
        if (sceneObject.children !== undefined) {
          sceneObject.children.forEach(function(so) {
            ps.lookForSceneObject(so,uuid,callback);
          });
        }
      }
    };

    this.createScene = function() {
        return {
            uuid: UtilService.generateUUID(),
            name:"New Scene",
            objects:[
                this.createSceneObject()
            ],
            scriptingEnabled : false,
            animationEnabled : false,
            physicsEnabled   : false,
            audioEnabled     : false,
            inputEnabled     : false,
        };
    };

    this.createSceneObject = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "New Scene Object",
            assetInstances: [],
            children: [],
            translation: {
              x: 0,
              y: 0,
              z: 0
            },
            rotation: {
              x: 0,
              y: 0,
              z: 0
            },
            scale: {
              x: 1,
              y: 1,
              z: 1
            }
        };
    };

    this.createAsset = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "New Asset",
            type: "",
        };
    };

    this.setDreamAnimation = function(dreamAnim) {
      this.project.dreamAnimation = dreamAnim;
    };

    this.setGLFWInput = function(glfw) {
      this.project.glfwInput = glfw;
    };

    this.setChai = function(chai) {
        this.project.chai = chai;
    };

    this.setOpenAL = function(openAL) {
        this.project.openAL = openAL;
    };

    this.setBullet2 = function(b2) {
        this.project.bullet2 = b2;
    };

    this.setOpenGL = function(openGL) {
        this.project.openGL = openGL;
    };

    this.setSelectedPlugins = function(selectedPlugins) {
      this.project.isModified = true;
      // Animation
      switch (selectedPlugins.animationPlugin) {
        case this.PLUGIN_ANIMATION_DREAM:
          this.project.dreamAnim = true;
          break;
      }
      // Audio
      switch (selectedPlugins.audioPlugin) {
        case this.PLUGIN_AUDIO_OPENAL:
          this.project.openAL = true;
          break;
      }
      // Input
      switch (selectedPlugins.inputPlugin) {
        case this.PLUGIN_INPUT_GLFW:
          this.project.glfwInput = true;
          break;
      }
      // Scripting
      switch (selectedPlugins.scriptingPlugin) {
        case this.PLUGIN_SCRIPTING_CHAI:
          this.project.chai = true;
          break;
        default:
          this.project.chai = false;
          break;
      }
      // Physics
      switch (selectedPlugins.physicsPlugin) {
        case this.PLUGIN_PHYSICS_BULLET2:
          this.project.bullet2 = true;
          break;
        default:
          this.project.bullet2 = false;
          break;
      }
      // Video
      switch (selectedPlugins.videoPlugin) {
          case this.PLUGIN_VIDEO_OPENGL:
            this.project.openGL = true;
            break;
      }
    };

    this.getSelectedPlugins = function(callback) {
        var retval = {};

        // Animation
        if (this.project.dreamAnim) {
          retval.animationPlugin = this.PLUGIN_ANIMATION_DREAM;
        }
        // Audio
        if (this.project.openAL) {
          retval.audioPlugin = this.PLUGIN_AUDIO_OPENAL;
        }
        // Input
        if (this.project.glfwInput) {
          retval.inputPlugin = this.PLUGIN_INPUT_GLFW;
        }
        // Physics
        if (this.project.bullet2) {
            retval.physicsPlugin = this.PLUGIN_PHYSICS_BULLET2;
        }
        // Scripting
        if (this.project.chai) {
          retval.scriptingPlugin = this.PLUGIN_SCRIPTING_CHAI;
        }
        // Video
        if (this.project.openGL) {
          retval.videoPlugin = this.PLUGIN_VIDEO_OPENGL;
        } else if (this.project.vulkan) {
          retval.videoPlugin = this.PLUGIN_VIDEO_VULKAN;
        }

        callback(retval);
    };

    this.initialise = function() {
        this.project = {
            uuid:        UtilService.generateUUID(),
            name:        "New Dream Project",
            author:      "",
            description: "",
            scenes:      [],
            assets:      [],
            isModified:  false
        };
    };

    this.openProject = function(uuid, callback) {
        var svc = this;
        ApiService.readProjectFile(uuid, function(data){
            if (data === null) {
                callback(false);
            } else {
                svc.project = data;
                console.log("Opened project",svc.project);
                svc.project.isModified = false;
                callback(true);
            }
        });
    };

    this.isModified = function() {
        if (this.project !== null) return this.project.isModified;
        return false;
    };

    this.setProjectModified = function(modified) {
        this.project.isModified = modified;
    };

    this.generateDownloadBlob = function() {
        delete this.project.isModified;
        return new Blob([ JSON.stringify(this.project) ], { type : 'application/octet-stream' });
    };

    this.updateAsset = function(asset) {
        this.setAssetName(asset);
        this.setAssetType(asset);
    };

    this.setAssetName = function(asset) {
        this.getAssetByUUID(asset.uuid,function(rsc){
            rsc.name = asset.name;
        });
    };

    this.setAssetType = function(asset) {
        this.getAssetByUUID(asset.uuid,function(rsc){
            rsc.type = asset.type;
        });
    };

    this.updateScene = function(scene) {
      this.setSceneScriptingEnabled (scene);
      this.setSceneAnimationEnabled (scene);
      this.setSceneInputEnabled     (scene);
      this.setSceneAudioEnabled     (scene);
      this.setSceneName             (scene);
    };

    this.setSceneScriptingEnabled = function(scene) {
      this.getSceneByUUID(scene.uuid,function(projScene) {
          projScene.scriptingEnabled = scene.scriptingEnabled;
      });
    };

    this.setSceneAnimationEnabled = function(scene) {
        this.getSceneByUUID(scene.uuid,function(projScene){
            projScene.animationEnabled = scene.animationEnabled;
        });
    };

    this.setSceneInputEnabled = function(scene) {
        this.getSceneByUUID(scene.uuid,function(projScene){
            projScene.inputEnabled = scene.inputEnabled;
        });
    };

    this.setSceneAudioEnabled = function(scene){
        this.getSceneByUUID(scene.uuid,function(projScene){
            projScene.audioEnabled = scene.audioEnabled;
        });
    };

    this.setScenePhysicsEnabled = function(scene) {
        this.getSceneByUUID(scene.uuid,function(projScene){
            projScene.physicsEnabled = scene.physicsEnabled;
        });
    };

    this.setSceneName = function(scene) {
        this.getSceneByUUID(scene.uuid,function(projScene){
            projScene.name = scene.name;
        });
    };

    this.closeProject = function() {
        this.project = null;
    };

    this.getAssetList = function(callback) {
        callback(this.project.assets);
    };

    this.getScriptAssetList = function(callback) {
      var retval = [];
      var projSvc = this;
      this.project.assets.forEach(function(asset){
        if (asset.type == projSvc.ASSET_TYPE_SCRIPT) {
          retval.push(asset);
        }
      });
      callback(retval);
    };

    this.assetHasModelAssimp = function(uuid,callback){
        ApiService.assetExists(
            this.project.uuid,
            this.ASSET_TYPE_MODEL,
            uuid,
            this.ASSET_FORMAT_MODEL_ASSIMP,
            callback
        );
    };

    this.assetHasModelMtl = function(uuid,callback){
        ApiService.assetExists(
            this.project.uuid,
            this.ASSET_TYPE_MODEL,
            uuid,
            this.ASSET_FORMAT_MODEL_WAVEFRONT_MTL,
            callback
        );
    };

    this.assetHasVertexShader = function(uuid,callback){
        ApiService.assetExists(
            this.project.uuid,
            this.ASSET_TYPE_SHADER,
            uuid,
            this.ASSET_FORMAT_SHADER_VERTEX,
            callback
        );
    };

    this.assetHasFragmentShader = function(uuid,callback){
      ApiService.assetExists(
        this.project.uuid,
        this.ASSET_TYPE_SHADER,
        uuid,
        this.ASSET_FORMAT_SHADER_FRAGMENT,
        callback
      );
    };

    this.assetHasAudioWav = function(uuid,callback) {
      ApiService.assetExists(
        this.project.uuid,
        this.ASSET_TYPE_AUDIO,
        uuid,
        this.ASSET_FORMAT_AUDIO_WAV,
        callback
      );
    };

    this.assetHasAudioOgg = function(uuid,callback) {
      ApiService.assetExists(
        this.project.uuid,
        this.ASSET_TYPE_AUDIO,
        uuid,
        this.ASSET_FORMAT_AUDIO_OGG,
        callback
      );
    };

    this.saveScriptAsset = function(asset,script,callback) {
      ApiService.uploadAsset(
        "/"+this.project.uuid+"/asset/script/"+asset.uuid+"/"+asset.format,
        script,
        callback
      );
    };

    this.saveVertexShaderAsset = function(asset,content,callback) {
      ApiService.uploadAsset(
        "/" +
        this.project.uuid +
        "/asset/" +
        this.ASSET_TYPE_SHADER +
        "/" + asset.uuid +
        "/" + this.ASSET_FORMAT_SHADER_VERTEX,
        content,
        callback
      );
    };

    this.saveFragmentShaderAsset = function(asset,content,callback) {
      ApiService.uploadAsset(
        "/" +
        this.project.uuid +
        "/asset/" +
        this.ASSET_TYPE_SHADER +
        "/" + asset.uuid + "/" +
        this.ASSET_FORMAT_SHADER_FRAGMENT,
        content,
        callback
      );
    };

    this.readAsset = function(asset,callback) {
      ApiService.readAsset(this.project, asset,callback);
    };

    this.deleteAssetData = function(asset,callback) {
      ApiService.deleteAssetData(this.project.uuid,asset.type,asset.uuid,callback);
    };

    return this;
}]);
