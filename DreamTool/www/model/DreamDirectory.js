var fs    = require('fs');
var path  = require('path');
var targz = require('tar.gz');

var DREAM_TOOL_HOME_DIR = ".dreamtool";
var ASSET_DIR           = "assets";
var MODELS_DIR          = "model";
var AUDIO_DIR           = "audio";
var ANIMATION_DIR       = "animation";
var SCRIPT_DIR          = "script";
var SHADER_DIR          = "shader";

var AUDIO_WAV_FMT = "wav";
var AUDIO_OGG_FMT = "ogg";

var MODEL_OBJ_FMT = "obj";
var MODEL_MTL_FMT = "mtl";

var SHADER_VERT_FMT = "vertex";
var SHADER_FRAG_FMT = "fragment";

var JSON_EXTENSION = ".json";
var TAR_GZ_EXTENSION = ".tar.gz";

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

    var assetPrefix = projectPrefix + ASSET_DIR;
    fs.mkdirSync(assetPrefix);
    console.log("mkdir",assetPrefix,"ok");

    var models = assetPrefix + path.sep + MODELS_DIR;
    fs.mkdirSync(models);
    console.log("mkdir",models,"ok");

    var audio = assetPrefix + path.sep + AUDIO_DIR;
    fs.mkdirSync(audio);
    console.log("mkdir",audio,"ok");

    var anim = assetPrefix + path.sep + ANIMATION_DIR;
    fs.mkdirSync(anim);
    console.log("mkdir",anim,"ok");

    var script = assetPrefix + path.sep + SCRIPT_DIR;
    fs.mkdirSync(script);
    console.log("mkdir",script,"ok");

    var shader = assetPrefix + path.sep + SHADER_DIR;
    fs.mkdirSync(shader);
    console.log("mkdir",shader,"ok");
    yield next;
};

module.exports.compressProject = function (projectUUID,callback) {
    console.log("Compressing project",projectUUID);
    var srcPath = dreamDirectory+path.sep+projectUUID+path.sep;
    var archPath = dreamDirectory+path.sep+projectUUID+TAR_GZ_EXTENSION;
    var read = targz().createReadStream(srcPath);
    var write = fs.createWriteStream(archPath);

    read.on('close',function(){
        callback(fs.createReadStream(archPath));
    });

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

var getAssetDirectoryByFormat = function(format) {
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

module.exports.writeProjectFile = function(proj,data) {
    var projPath =
        dreamDirectory+path.sep+
        proj+path.sep+
        proj+".json";

    fs.writeFileSync(projPath,JSON.stringify(data));
    console.log("Project file written to",projPath);
};

module.exports.writeAsset = function(proj, dir, rsc, format, data, next) {
    console.log("  Project is:", proj);
    console.log("Directory is:", dir);
    console.log(" Asset is:", rsc);
    console.log("   Format is:", format);

    var projPath     = dreamDirectory  + path.sep + proj;
    var dirPath      = projPath + path.sep + ASSET_DIR + path.sep + dir;
    var rscPath      = dirPath  + path.sep + rsc;
    var absolutePath = rscPath  + path.sep + format;

    console.log("Writing Asset to",absolutePath);

    try {
        fs.mkdirSync(rscPath);
        console.log("Created directory",rscPath);
    } catch (e) {
        console.log(rscPath,"exists");
    }

    try {
        var writeData = null;

        if (dir === SCRIPT_DIR || dir === SHADER_DIR) {
            writeData = data;
            console.log("Data identified as NOT b64 buffer");
        } else {
            writeData = new Buffer(data.split(",")[1],'base64');
            console.log("Data identified as b64 buffer");
        }

        if (writeData !== null) {
            fs.writeFileSync(absolutePath,writeData);
            console.log("Successfuly written", absolutePath);
        }
    } catch (e) {
        console.log("Could not write file to",absolutePath);
        return;
    }
    return;
};

module.exports.readProjectFile = function(uuid) {
    var retval = fs.readFileSync(getProjectFilePathFromUUID(uuid));
    return retval;
};

var getProjectFilePathFromUUID = function(uuid) {
    return dreamDirectory+path.sep+uuid+path.sep+uuid+JSON_EXTENSION;
};

var isValidProjectFolder = function(projFolder) {
    return (
      projFolder.indexOf(TAR_GZ_EXTENSION) < 0 &&
      projFolder[0] != "."
  );
};

module.exports.listProjects = function () {
    var retval = [];
    var uuidList = fs.readdirSync(dreamDirectory);
    uuidList.forEach(function(projFolder) {
        if (isValidProjectFolder(projFolder)) {
            var name = "Untitled Project";
            try {
                name = require(getProjectFilePathFromUUID(projFolder)).name || name;
            } catch (ex) {
                console.log("exception while reading project dir list",ex);
            }
            retval.push({
                uuid: projFolder,
                name: name
            });
        }
    });
    return retval;
};

module.exports.assetExists = function(project,uuid,type,format) {
    var retval = false;
    var absAssetPath =
        dreamDirectory + path.sep +
        project        + path.sep +
        ASSET_DIR      + path.sep +
        type           + path.sep +
        uuid           + path.sep +
        format;
    console.log("Checking for",absAssetPath);
    retval = fs.existsSync(absAssetPath);
    return retval;
};

module.exports.readAsset = function(project,uuid,type,format) {
    var retval = null;
    var absAssetPath =
        dreamDirectory + path.sep +
        project        + path.sep +
        ASSET_DIR      + path.sep +
        type           + path.sep +
        uuid           + path.sep +
        format;
    console.log("Checking for",absAssetPath);
    retval = fs.readFileSync(absAssetPath);
    return retval;
};

module.exports.getProjectDirectory = function() {
    return dreamDirectory;
};

createDreamToolDirInHome();
