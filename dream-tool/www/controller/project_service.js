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
        this.isModified = true;
        this.project.name = name;
    };

    this.setAuthor = function(author) {
        this.isModified = true;
        this.project.author = author;
    };

    this.setDescription = function(description) {
        this.isModified = true;
        this.project.description = description;

    };

    this.pushScene = function(scene) {
        this.isModified = true;
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
        this.isModified = true;
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
        ];
    };

    this.addResourceLinkToSceneObject = function(sceneUUID,sceneObjectUUID,resourceUUID){
        var proj = this;
        console.log("Adding Resource Link",sceneObjectUUID,resourceUUID);
        this.getSceneByUUID(sceneUUID,function(scene){
            proj.getSceneObjectByUUID(scene,sceneObjectUUID,function(sceneObject){
                if (sceneObject.resourceLinks === undefined) {
                    sceneObject.resourceLinks = [];
                }
                if (sceneObject.resourceLinks.indexOf(sceneObjectUUID) < 0) {
                    sceneObject.resourceLinks.push(resourceUUID);
                    console.log(sceneObjectUUID,"contains links",sceneObject.resourceLinks);
                }
            });
        });
    };

    this.getSceneObjectByUUID = function(scene,sceneObjectUUID,callback) {
        scene.objects.forEach(function(sceneObject){
            if (sceneObject.uuid == sceneObjectUUID) {
                callback(sceneObject);
            }
        });
    };

    // Create New Resources ----------------------------------------------------

    this.createScene = function() {
        return {
            uuid: UtilService.generateUUID(),
            name:"New Scene",
            objects:[
                this.createSceneObject()
            ],
            animationEnabled : false,
            bulletEnabled    : false,
            audioEnabled     : false,
            inputEnabled     : false,
        };
    };

    this.createSceneObject = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "New Scene Object",
            resourceLinks: [],
        };
    };

    this.createResource = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "New Resource",
            type: "",
        };
    };

    this.initialise = function() {
        this.project = {
            uuid:        UtilService.generateUUID(),
            name:        "New Dream Project",
            author:      "",
            description: "",
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

    this.setSceneBulletEnabled = function(scene) {
        this.getSceneByUUID(scene.uuid,function(projScene){
            projScene.bulletEnabled = scene.bulletEnabled;
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

    return this;
}]);
