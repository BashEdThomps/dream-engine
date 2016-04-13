var fs    = require('fs');
var path  = require('path');
var targz = require('tar.gz');

var DREAM_TOOL_HOME_DIR = ".dreamtool";
var PROJECT_FILE        = "project.json";
var RESOURCE_DIR        = "resources";
var MODELS_DIR          = "models";
var AUDIO_DIR           = "audio";
var ANIMATION_DIR       = "animation";

var AUDIO_WAV_FMT = "wav";
var AUDIO_OGG_FMT = "ogg";

var MODEL_OBJ_FMT = "obj";
var MODEL_MTL_FMT = "mtl";

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

module.exports.compressProject = function *(projectUUID, koaObj, next) {
    console.log("Compressing project",projectUUID);
    var read = targz().createReadStream(dreamDirectory+path.sep+projectUUID);
    read.pipe(koaObj.body);
    yield next;
    //var write = fs.createWriteStream(dreamDirectory+path.sep+projectName+'.tar.gz');
    //yield read.pipe(write);
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

var getResourceDirectoryByFormat = function(format) {
    switch (format) {
        case AUDIO_WAV_FMT:
        case AUDIO_OGG_FMT:
            return AUDIO_DIR;
        case MODEL_OBJ_FMT:
        case MODEL_MTL_FMT:
            return MODEL_DIR;
        default:
            return UNKNOWN_FMT;
    }
    return UNKNOWN_FMT;
};

module.exports.writeProjectFile = function* (proj,data,next) {
    var projPath =
        dreamDirectory+path.sep+
        proj+path.sep+
        proj+".json";

    fs.writeFileSync(projPath,JSON.stringify(data));
    console.log("Project file written to",projPath);
    yield next;
};

module.exports.writeResource = function* (proj, dir, rsc, format, data, next) {
    console.log("Project is",   proj);
    console.log("Directory is", dir);
    console.log("Resource is",  rsc);
    console.log("Format is ",   format);

    var projPath     = dreamDirectory  + path.sep + proj;
    var dirPath      = projPath + path.sep + RESOURCE_DIR + path.sep + dir;
    var rscPath      = dirPath  + path.sep + rsc;
    var absolutePath = rscPath  + path.sep + format;

    console.log("Writing Resource to",absolutePath);

    try {
        fs.mkdirSync(rscPath);
    } catch (e) {
        console.log(rscPath,"exists");
    }

    try {
        fs.writeFileSync(absolutePath,new Buffer(data.split(",")[1],'base64'));
    } catch (e) {
        console.log("Could not write file to",absolutePath);
    }
    console.log("Successfuly written", absolutePath);
    yield next;
};

module.exports.readProjectFile = function(uuid) {
    var retval = require(getProjectFilePathFromUUID(uuid));
    return retval;
};

var getProjectFilePathFromUUID = function(uuid) {
    return dreamDirectory+path.sep+uuid+path.sep+uuid+".json";
};

module.exports.listProjects = function () {
    var retval = [];
    var uuidList = fs.readdirSync(dreamDirectory);
    uuidList.forEach(function(projFolder) {
        var name = "untitled";
        try {
            name = require(getProjectFilePathFromUUID(projFolder)).name || "Untitled";
        } catch (ex) {
            console.log("exception while reading project dir list",ex);
        }
        retval.push({
            uuid: projFolder,
            name: name
        });
    });
    return retval;
};

createDreamToolDirInHome();
