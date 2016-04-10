App.service('ProjectService',
    ["Blob","FileSaver","UtilService",
     function(Blob, FileSaver, UtilService) {

    this.project = null;

    this.getProject = function() {
        if (this.project === null) {
            this.initialise();
        }
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

    this.getConfigurationAnimationEnabled = function() {
        return this.project.configuration.animationEnabled;
    };

    this.setConfigurationAnimationEnabled = function(enabled) {
        this.isModified = true;
        this.project.configuration.animationEnabled = enabled;
    };

    this.getConfigurationCollisionEnabled = function() {
        return this.project.configuration.collisionEnabled;
    };

    this.setConfigurationCollisionEnabled = function(enabled) {
        this.isModified = true;
        this.project.configuration.collisionEnabled = enabled;
    };

    this.getConfigurationPhysicsEnabled = function() {
        return this.project.configuration.physicsEnabled;
    };

    this.setConfigurationPhysicsEnabled = function(enabled) {
        this.isModified = true;
        this.project.configuration.physicsEnabled = enabled;
    };

    this.getConfigurationInputEnabled = function() {
        return this.project.configuration.inputEnabled;
    };

    this.setConfigurationInputEnabled = function(enabled) {
        this.isModified = true;
        this.project.configuration.inputEnabled = enabled;
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
            "Model",
            "Animation",
            "Audio"
        ];
    };

    // Create New Resources ----------------------------------------------------

    this.createScene = function() {
        return {
            uuid: UtilService.generateUUID(),
            name:"",
            objects:[
                this.createSceneObject()
            ],
            animationEnabled: false,
            collisionEnabled: false,
            physicsEnabled:   false,
            audioEnabled:     false,
            inputEnabled:     false,
        };
    };

    this.createSceneObject = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "",
        };
    };

    this.createResource = function() {
        return {
            uuid: UtilService.generateUUID(),
            name: "",
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

    this.saveProject = function () {
        var blob = this.generateDownloadBlob();
        console.log("Saving:",blob);
        FileSaver.saveAs(blob, this.getName()+'.json');
    };

    this.openProject = function(project) {
        this.project = project;
        this.project.isModified = false;
    };

    this.isModified = function() {
        return this.project.isModified;
    };

    this.setProjectModified = function(modified) {
        this.project.isModified = modified;
    };

    this.generateDownloadBlob = function() {
        delete this.project.isModified;
        return new Blob([ JSON.stringify(this.project) ], { type : 'application/octet-stream' });
    };

    return this;
}]);
