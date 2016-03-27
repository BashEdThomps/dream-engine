var config    = require ('../config.json');
var koa       = require('koa');
var koaStatic = require('koa-static');
var app       = koa();
var sys       = require('sys');
var exec      = require('child_process').exec;
var parse     = require('co-body');

// Directory Constants
var WWW                        = "www";
var NODE_MODULES               = __dirname    + "/../node_modules/";
var ANGULAR                    = NODE_MODULES + "angular";
var ANGULAR_UI_ROUTER          = NODE_MODULES + "angular-ui-router/release";
var ANGULAR_UI_BOOTSTRAP       = NODE_MODULES + "angular-ui-bootstrap/dist";
var ANGULAR_BOOTSTRAP_NAV_TREE = NODE_MODULES + "angular-bootstrap-nav-tree/dist";
var ANGULAR_SCREENFUL          = NODE_MODULES + "angular-screenfull/dist";
var BLOB                       = NODE_MODULES + "Blob.js";
var BOOTSTRAP                  = NODE_MODULES + "bootstrap/dist";
var ANGULAR_FILE_SAVER         = NODE_MODULES + "angular-file-saver/dist";
var SCREENFUL                  = NODE_MODULES + "screenfull/dist";
var FILESAVER                  = NODE_MODULES + "FileSaver.js";

app.use(koaStatic(WWW));
app.use(koaStatic(ANGULAR));
app.use(koaStatic(ANGULAR_BOOTSTRAP_NAV_TREE));
app.use(koaStatic(ANGULAR_SCREENFUL));
app.use(koaStatic(ANGULAR_UI_BOOTSTRAP));
app.use(koaStatic(ANGULAR_UI_ROUTER));
app.use(koaStatic(ANGULAR_FILE_SAVER));
app.use(koaStatic(FILESAVER));
app.use(koaStatic(BLOB));
app.use(koaStatic(BOOTSTRAP));
app.use(koaStatic(SCREENFUL));

var runDream = function(project) {
	var child = exec(config.dream_bin + " \"" + JSON.stringify(project) +"\"", function (error, stdout, stderr) {
  	sys.print('stdout: ' + stdout);
  	sys.print('stderr: ' + stderr);
  	if (error !== null) {
    	console.log('exec error: ' + error);
  	}
  });
	console.log("exec'd child:");
};

app.use(function *(next){
	if ('POST' !== this.method) {
		return next;
	}

	var project = yield parse(this, {limit: '1024kb'});
	console.log("Executing Project:",project);
	runDream(project);
	this.body   = { message: "OK" };
	this.status =  200;
});

module.exports.listen = function() {
    console.log("DreamTool WWW is starting on port", config.port);
    app.listen(config.port);
};
