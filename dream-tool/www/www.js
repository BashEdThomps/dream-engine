var config         = require ('../config.json');
var fs             = require('fs');
var koa            = require('koa');
var koaRouter      = require('koa-router')();
var koaStatic      = require('koa-static');
var bodyParser     = require('koa-bodyparser');
var app            = koa();
var dreamDirectory = require('./model/DreamDirectory');
var dreamExecutor  = require('./model/DreamExecutor');

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

// Static Pages ----------------------------------------------------------------

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

// Api -------------------------------------------------------------------------

// Log requet
app.use(function *(next){
	console.log(
		"API Request"+
		"\n\tURL:",this.request.url,
		"\n\tMethod:",this.request.method
	);
	yield next;
});

// POST to /create
koaRouter.post("/create",function* (next) {
	var uuid = this.request.body.uuid;
	this.status = 200;
	yield dreamDirectory.createProjectDirectory(uuid,next);
});

// POST to /uuid/resource/*
koaRouter.post("/:uuid/resource/", function* () {
	var resourcePath = this.request.url;
	var reqData = this.request.body;
	this.status = 200;
	yield dreamDirectory.writeResource(resourcePath,reqData,next);
});

app.use(koaRouter.routes());
app.use(koaRouter.allowedMethods());

module.exports.listen = function() {
    console.log("DreamTool WWW is starting on port", config.port);
    app.listen(config.port);
};
