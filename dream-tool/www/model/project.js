var fs    = require('fs');
var path  = require('path');
var targz = require('tar.gz');

var PROJECT_FILE = "project.json";
var RESOURCE_DIR = "resources";
var MODELS_DIR   = "models";
var AUDIO_DIR    = "audio";

var projectName = "DreamProject";

var setProjectName = function(name) {
    projectName = name;
};

var createProjectDirectoryStructure = function(projectName) {
    fs.mkdir(projectName);
    fs.mkdir(projectName+path.sep+RESOURCE_DIR);
    fs.mkdir(projectName+path.sep+RESOURCE_DIR+path.sep+MODELS_DIR);
    fs.mkdir(projectName+path.sep+RESOURCE_DIR+path.sep+AUDIO_DIR);
};

var compressProjectDirectory = function() {
    var read = targz().createReadStream(projectName);
    var write = fs.createWriteStream(projectName+'.tar.gz');
    read.pipe(write);
};

module.exports = this;
