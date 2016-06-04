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
var ANGULAR_AUDIO              = NODE_MODULES + "angular-audio";
var ANGULAR_COOKIES            = NODE_MODULES + "angular-cookies";
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
var D3                         = NODE_MODULES + "d3";
var FONT_AWESOME               = NODE_MODULES + "font-awesome";
var ACE_BUILDS                 = NODE_MODULES + "ace-builds";
var THREE_JS                   = NODE_MODULES + "three";

app.use(bodyParser({
	formLimit:"100mb",
	jsonLimit:"100mb"
}));

// Static Pages ----------------------------------------------------------------

app.use(koaStatic(WWW));
app.use(koaStatic(ANGULAR));
app.use(koaStatic(ANGULAR_AUDIO));
app.use(koaStatic(ANGULAR_COOKIES));
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
app.use(koaStatic(D3));
app.use(koaStatic(FONT_AWESOME));
app.use(koaStatic(ACE_BUILDS));
app.use(koaStatic(THREE_JS));

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

// POST to /uuid/asset/*
koaRouter.post("/:proj/asset/:dir/:rsc/:format", function* (next) {
	var assetPath = this.request.url;
	var reqData = this.request.body.data;
	this.status = 200;
	dreamDirectory.writeAsset(
		this.params.proj,
		this.params.dir,
		this.params.rsc,
		this.params.format,
		reqData
	);
	yield next;
});

// POST to /save/:project_id to save project json file
koaRouter.post("/save/:project_id",function* (next){
	var projData = this.request.body.project;
	console.log("Saving Project:",JSON.stringify(projData));
	dreamDirectory.writeProjectFile(this.params.project_id,projData);
	this.status = 200;
	yield next;
});

// GET /compress/:uuid to get a compressed project
koaRouter.get("/compress/:uuid", function *(next){
	var uuid = this.params.uuid;
	console.log("Compressing project",uuid);
	var req = this;
	var promise = new Promise ( function(resolve,reject) {
		dreamDirectory.compressProject(uuid,function(stream){
			req.body = stream;
		    return resolve();
		});
	});
	yield promise;
});

// GET /projectlist to list stored projects
koaRouter.get("/projectlist",function *(next) {
	this.status = 200;
	this.body = dreamDirectory.listProjects();
	yield next;
});

// GET /projectfile/:uuid to read a project file
koaRouter.get("/projectfile/:uuid",function* (next){
	this.status = 200;
	this.body = dreamDirectory.readProjectFile(this.params.uuid);
	yield next;
});

// GET Asset Exists
koaRouter.get("/asset/exists/:project/:type/:uuid/:format",function* (next) {
	this.status = 200;
	this.body = dreamDirectory.assetExists(
		this.params.project,
		this.params.uuid,
		this.params.type,
		this.params.format
	);
	yield next;
});

// DELETE Asset Data
koaRouter.delete("/asset/delete/:project/:type/:uuid",function* (next) {
	this.status = 200;
	this.body = dreamDirectory.deleteAssetData(
		this.params.project,
		this.params.uuid,
		this.params.type
	);
	yield next;
});

// GET /asset_exists/:uuid/type/format
koaRouter.get("/asset/:project/:type/:uuid/:format",function* (next) {
	this.status = 200;
	this.body = dreamDirectory.readAsset (
		this.params.project,
		this.params.uuid,
		this.params.type,
		this.params.format
	);
	yield next;
});

koaRouter.post("/exec", function* (next){
	this.status = 200;
	var exec = this.request.body.exec;
	var uuid = this.request.body.uuid;
	dreamExecutor.execDream(exec,dreamDirectory.getProjectDirectory(), uuid);
	yield next;
});

app.use(koaRouter.routes());
app.use(koaRouter.allowedMethods());

module.exports.listen = function() {
    console.log("DreamTool WWW is starting on port", config.port);
    app.listen(config.port);
};
