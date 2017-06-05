var fs    = require('fs');
var path  = require('path');

var WWW_DIR           = "www"
var TEMPLATES_DIR     = "templates";
var SHADER_DIR        = "shader";
var SCRIPTS_DIR       = "script";
var WWW_TEMPLATES_DIR = WWW_DIR+path.sep+TEMPLATES_DIR;

module.exports.readTemplate = function(type,name,file) {
    var retval = null;
    var absAssetPath =
        TEMPLATES_DIR + path.sep +
        type          + path.sep +
        name          + path.sep +
        format;
    console.log("Checking for",absAssetPath);
    retval = fs.readFileSync(absAssetPath);
    return retval;
};

module.exports.listTemplates = function() {
    var retval = [];
    var typesList = fs.readdirSync(WWW_TEMPLATES_DIR);
    typesList.forEach(function(type) {
        var nextDir = WWW_TEMPLATES_DIR+path.sep+type;
        if (fs.lstatSync(nextDir).isDirectory()) {
            var names = fs.readdirSync(nextDir);
            var namesReturned = [];
            names.forEach(function(name) {
                var nextName = WWW_TEMPLATES_DIR+path.sep+type+path.sep+name;
                    if (fs.lstatSync(nextName).isDirectory()) {
                        namesReturned.push(name);
                    }
            });
            var typeDir = {type: type, names: namesReturned};
            retval.push(typeDir);
        }
    });
    return retval;
};
