var fs    = require('fs');
var path  = require('path');
var targz = require('tar.gz');

var DREAM_TOOL_HOME_DIR = ".dreamtool";
var PROJECT_FILE        = "project.json";
var RESOURCE_DIR        = "resources";
var MODELS_DIR          = "models";
var AUDIO_DIR           = "audio";
var ANIMATION_DIR       = "animation";

var getUserHome = function() {
  return process.env[(process.platform == 'win32') ? 'USERPROFILE' : 'HOME'];
};

var USER_HOME           = getUserHome();
var dreamDirectory      = USER_HOME+path.sep+DREAM_TOOL_HOME_DIR;

var dreamDirectoryExists = function(callback) {
    fs.exists(dreamDirectory,callback);
};

var projectDirectoryExists = function(projectUUID,callback) {
    var projectPrefix = dreamDirectory + path.sep + projectUUID;
    fs.exists(projectPrefix,callback);
};

module.exports.createProjectDirectory = function *(projectUUID, next) {
    var projectPrefix =  dreamDirectory +path.sep+ projectUUID + path.sep;
    console.log("Creating project directory structure for",projectPrefix);
    fs.mkdirSync(projectPrefix);
    console.log("mkdir",projectPrefix,"ok");

    var resourcePrefix = projectPrefix + RESOURCE_DIR;
    fs.mkdirSync(resourcePrefix);
    console.log("mkdir",resourcePrefix,"ok");

    var models = resourcePrefix + path.sep + MODELS_DIR;
    fs.mkdirSync(models);
    console.log("mkdir",models,"ok");

    var audio = resourcePrefix + path.sep + AUDIO_DIR;
    fs.mkdirSync(audio);
    console.log("mkdir",audio,"ok");

    var anim = resourcePrefix + path.sep + ANIMATION_DIR;
    fs.mkdir(anim);
    console.log("mkdir",anim,"ok");
    yield next;
};

var compressProjectDirectory = function(projectUUID) {
    var read = targz().createReadStream(dreamDirectory+path.sep+projectUUID);
    var write = fs.createWriteStream(dreamDirectory+path.sep+projectName+'.tar.gz');
    read.pipe(write);
};


var createDreamToolDirInHome = function(callback) {
	console.log("Opening DreamTool dir at",dreamDirectory);
	dreamDirectoryExists(function(exists){
		if (exists) {
			console.log(dreamDirectory,"exists");
            if (callback) callback(true);
		} else {
			console.log(dreamDirectory,"does not exist, creating...");
			fs.mkdir(dreamDirectory,function(err,folder) {
				if (err) {
					console.log("Error creating",dreamDirectory);
                    if (callback) callback(false);
				} else  {
					console.log(dreamDirectory,"created!");
                    if (callback) callback(true);
				}
			});
		}
	});
};

module.exports.writeResource = function* (rscPath, data, next) {
    var projectUUID = rscPath.substr(1,19);
    console.log("Project is",projectUUID);
    var absolutePath = dreamDirectory + rscPath;
    console.log("Writing Resource to",absolutePath);
    yield next;
};

createDreamToolDirInHome();
