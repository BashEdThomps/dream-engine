var config    = require ('../config.json');
var koa       = require('koa');
var koaStatic = require('koa-static');
var bodyParser = require('koa-bodyparser');
var app       = koa();
var sys       = require('sys');
var exec      = require('child_process').exec;
var parse     = require('co-body');
var project   = require('./model/project');

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
var NG_TOAST                   = NODE_MODULES + "ng-toast/dist";
var ANGULAR_SANITIZE           = NODE_MODULES + "angular-sanitize";
var ANGULAR_ANIMATE            = NODE_MODULES + "angular-animate";

app.use(bodyParser({
	formLimit:"100mb",
	jsonLimit:"100mb"
}));
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
app.use(koaStatic(NG_TOAST));
app.use(koaStatic(ANGULAR_SANITIZE));
app.use(koaStatic(ANGULAR_ANIMATE));

// Api

app.use(function *(next) {
	console.log("API Request URL: ",this.request.url,"Method:",this.request.method);

	// POST to /resource/*
	if (this.request.url.indexOf('resource') > -1 && this.request.method == "POST")
	{
		var resourcePath = this.method.url;
		this.status = 200;
		//console.log("Body:",this.request.body);
		this.body = "Success";
	} else {
		this.status = 404;
		this.body = "Unable to handle this request";
	}
	yield next;
});

var runDream = function(project) {
	var child = exec(config.dream_bin+" "+project+".tar.gz", function (error, stdout, stderr) {
  	sys.print('stdout: ' + stdout);
  	sys.print('stderr: ' + stderr);
  	if (error !== null) {
    	console.log('exec error: ' + error);
  	}
  });
	console.log("exec'd child:");
};

module.exports.listen = function() {
    console.log("DreamTool WWW is starting on port", config.port);
    app.listen(config.port);
};
