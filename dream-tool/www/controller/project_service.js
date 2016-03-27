App.service('ProjectService',
    ["Blob","FileSaver", function(Blob,FileSaver) {

    this.project = null;

    this.getProject = function() {
        if (this.project === null) {
            this.initialise();
        }
        return this.project;
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
    this.getSoundEffectFromByName = function(scene, name, callback) {
        var retval = null;
        this.project.resources.audio.soundEffects.forEach(function(sfx){
            if (sfx.name == name) {
                retval = name;
            }
        });
        callback(retval);
    };


    this.pushScene = function(scene) {
        this.isModified = true;
        this.scenes.push(scene);
    };

    this.getResourecTypes = function() {
        return [
            "Model",
            "Animation",
            "Sound Effect",
            "Music"
        ];
    };

    // Create New Resources ----------------------------------------------------

    this.createScene = function() {
        return {
            name:"New Scene",
            objects:[this.createSceneObject()],
        };
    };

    this.createSceneObject = function() {
        return {
            name: "New Scene Object",
        };
    };

    this.createSoundEffectResource = function() {
        return {
            name: "New Sound Effect",
            path: "/path/to/soundeffect.wav",
            type: "Sound Effect"
        };
    };

    this.createMusicResource = function() {
        return {
            name: "New Music",
            path: "/path/to/music.wav",
            type: "Music"
        };
    };

    this.createModelResource = function() {
        return {
            name: "New Model Resource",
            path: "/path/to/resource",
            type: "Model",
        };
    };

    this.createAnimationResource = function() {
        return {
            name: "New Animation",
            type: "Animation",
            path: "In Project",
            keyframes: [],
        };
    };

    this.initialise = function() {
        this.project = {
            name: "New Project",
            author: "Unknown Author",
            description: "A new Dream project.",
            configuration: {
                animationEnabled: false,
                collisionEnabled: false,
                physicsEnabled:   false,
                audioEnabled:     false,
                inputEnabled:     false,
            },
            scenes: [
                this.createScene()
            ],
            resources: [
                this.createAnimationResource(),
                this.createModelResource(),
                this.createSoundEffectResource(),
                this.createMusicResource()
            ],
            isModified: false
        };
    };

    this.saveProject = function () {
        var blob = this.generateDownloadBlob();
        console.log("Saving:",blob);
        FileSaver.saveAs(blob, this.getName()+'.json');
    };

    this.isModified = function() {
        return this.project.isModified;
    };

    this.setProjectModified = function(modified) {
        this.project.isModified = modified;
    };

    this.generateDownloadBlob = function() {
        return new Blob([ JSON.stringify(this.project) ], { type : 'application/octet-stream' });
    };

    return this;
}]);
