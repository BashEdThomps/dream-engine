App.service('ProjectService', function() {
    this.project = {};

    this.getName = function() {
        return this.project.name;
    };

    this.setName = function(name) {
        this.project.name = name;
    };

    this.getConfigurationAnimationEnabled = function() {
        return this.project.configuration.animationEnabled;
    };

    this.setConfigurationAnimationEnabled = function(enabled) {
        this.project.configuration.animationEnabled = enabled;
    };

    this.getConfigurationCollisionEnabled = function() {
        return this.project.configuration.collisionEnabled;
    };

    this.setConfigurationCollisionEnabled = function(enabled) {
        this.project.configuration.collisionEnabled = enabled;
    };

    this.getConfigurationPhysicsEnabled = function() {
        return this.project.configuration.physicsEnabled;
    };

    this.setConfigurationPhysicsEnabled = function(enabled) {
        this.project.configuration.physicsEnabled = enabled;
    };

    this.getConfigurationInputEnabled = function() {
        return this.project.configuration.inputEnabled;
    };

    this.setConfigurationInputEnabled = function(enabled) {
        this.project.configuration.inputEnabled = enabled;
    };

    this.createSoundEffect = function(scene,name) {
        project.resources.audio.soundEffects.push({
            name:name
        });
    };

    this.getSoundEffectFromByName = function(scene, name, callback) {
        var retval = null;
        project.resources.audio.soundEffects.forEach(function(sfx){
            if (sfx.name == name) {
                retval = name;
            }
        });
        callback(retval);
    };

    this.createScene = function() {
        return {
            name:"New Scene",

        };
    };

    this.getResourecTypes = function() {
        return [
            "Texture",
            "Shader",
            "VertexBuffer",
            "Audio"
        ];
    };

    this.createResource = function() {
        return {
            name: "New Resource",
            path: "/path/to/resource",
            type: "Texture",
        };
    };

    this.initialise = function() {
        this.project = {
            name: "Untitled Project",
            author: "Unknown Author",
            configuration: {
                animationEnabled: false,
                collisionEnabled: false,
                physicsEnabled:   false,
                audioEnabled:     false,
                inputEnabled:     false,
            },
            scenes: [],
            resources: [ this.createResource()

            ],
            isModified: false
        };
    };

    return this;
});
