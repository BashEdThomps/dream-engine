App.service('ProjectService',
    ["Blob","FileSaver","UtilService","ApiService",
     function(Blob, FileSaver, UtilService, ApiService) {

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

    this.updateProject = function(project) {
        this.setName        (project.name);
        this.setAuthor      (project.author);
        this.setDescription (project.description);
        this.setOpenAL      (project.openAL);
        this.setChai        (project.chai);
        this.setV8          (project.v8);
        this.setBullet2     (project.bullet2);
        this.setBullet3     (project.bullet3);
        this.setOpenGL      (project.openGL);
        this.setVulkan      (project.vulkan);
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

    this.pushResource = function(resource) {
        this.project.isModified = true;
        this.project.resources.push(resource);
    };

    this.getResourceByUUID = function(uuid, callback) {
        var retval = null;

        if (this.project === null) {
            callback(retval);
            return;
        }

        this.project.resources.forEach(function (rsc) {
            if (rsc.uuid === uuid) {
                retval = rsc;
            }
        });
        callback(retval);
    };

    this.removeResource = function(resource) {
        var index = this.project.resources.indexOf(resource);
        this.project.resources.splice(index,1);
    };

    this.getResourecTypes = function() {
        return [
            "Animation",
            "Audio",
            "Collision Shape",
            "Model",
            "Shader",
            "Script"
        ];
    };

    this.addResourceInstanceToSceneObject = function(sceneUUID,sceneObjectUUID,resourceUUID){
        var proj = this;
        console.log("Adding Resource Instance",sceneObjectUUID,"to",resourceUUID);
        this.getSceneByUUID(sceneUUID,function(scene){
            proj.getSceneObjectByUUID(scene,sceneObjectUUID,function(sceneObject){
                if (sceneObject.resourceInstances === undefined) {
                    sceneObject.resourceInstances = [];
                }
                if (sceneObject.resourceInstances.indexOf(resourceUUID) < 0) {
                    sceneObject.resourceInstances.push(resourceUUID);
                } else {
                    console.log(sceneObjectUUID,"allready has instance to",resourceUUID);
                }
            });
        });
    };

    this.removeResourceInstanceFromSceneObject = function(sceneUUID,sceneObjectUUID,resourceUUID){
        var proj = this;
        console.log("Removing Resource Instance",resourceUUID,"from",sceneObjectUUID);
        this.getSceneByUUID(sceneUUID, function(scene){
            proj.getSceneObjectByUUID(scene,sceneObjectUUID,function(sceneObject){
                var indexOf = sceneObject.resourceInstances.indexOf(resourceUUID);
                if (indexOf > -1) {
                    sceneObject.resourceInstances.splice(indexOf,1);
                } else {
                    console.log(sceneObjectUUID,": unable to remove instance of",resourceUUID);
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

    // Create New Resources ----------------------------------------------------

    this.createScene = function() {
        return {
            uuid: UtilService.generateUUID(),
            name:"New Scene",
            objects:[
                this.createSceneObject()
            ],
            scriptingEnabled : false,
            animationEnabled : false,
            physicsEnabled    : false,
            audioEnabled     : false,
            inputEnabled     : false,
        };
    };

    this.createSceneObject = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "New Scene Object",
            resourceInstances: [],
            children: []
        };
    };

    this.createResource = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "New Resource",
            type: "",
        };
    };

    this.setChai = function(chai) {
        this.project.chai = chai;
    };

    this.setV8 = function(v8) {
        this.project.v8 = v8;
    };;

    this.setOpenAL = function(openAL) {
        this.project.openAL = openAL;
    };

    this.setBullet2 = function(b2) {
        this.project.bullet2 = b2;
    };

    this.setBullet3 = function(b3) {
        this.project.bullet3 = b3;
    };

    this.setOpenGL = function(openGL) {
        this.project.openGL = openGL;
    };

    this.setVulkan = function(vulkan) {
        this.project.vulkan = vulkan;
    };

    this.initialise = function() {
        this.project = {
            uuid:        UtilService.generateUUID(),
            name:        "New Dream Project",
            author:      "",
            description: "",
            openAL : false,
            bullet2: false,
            bullet3: false,
            openGL: false,
            vulkan: false,
            scenes: [],
            resources: [],
            isModified: false
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

    this.updateResource = function(resource) {
        this.setResourceName(resource);
        this.setResourceType(resource);
    };

    this.setResourceName = function(resource) {
        this.getResourceByUUID(resource.uuid,function(rsc){
            rsc.name = resource.name;
        });
    };

    this.setResourceType = function(resource) {
        this.getResourceByUUID(resource.uuid,function(rsc){
            rsc.type = resource.type;
        });
    };

    this.updateScene = function(scene) {
      this.setSceneScriptingEnabled(scene);
      this.setSceneAnimationEnabled(scene);
      this.setSceneInputEnabled(scene);
      this.setSceneAudioEnabled(scene);
      this.setSceneName(scene);
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

    this.getResourceList = function(callback) {
        callback(this.project.resources);
    };

    this.resourceHasModelObj = function(uuid,callback){
        ApiService.resourceExists(
            this.project.uuid,
            "model",
            uuid,
            "obj",
            callback
        );
    };

    this.resourceHasModelMtl = function(uuid,callback){
        ApiService.resourceExists(
            this.project.uuid,
            "model",
            uuid,
            "mtl",
            callback
        );
    };

    this.saveScriptResource = function(resource,script,callback) {
        ApiService.uploadResource(
            "/"+this.project.uuid+"/resource/script/"+resource.uuid+"/"+"js",
            script,
            callback
        );
    };

    this.readResource = function(resource,callback) {
        ApiService.readResource(this.project, resource,callback);
    };

    return this;
}]);
