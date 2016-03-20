var config    = require ('../config.json');
var koa       = require('koa');
var koaStatic = require('koa-static');
var app       = koa();

// Directory Constants
var WWW                  = "www";
var NODE_MODULES         = __dirname    + "/../node_modules/";

var ANGULAR                    = NODE_MODULES + "angular";
var ANGULAR_UI_ROUTER          = NODE_MODULES + "angular-ui-router/release";
var ANGULAR_UI_BOOTSTRAP       = NODE_MODULES + "angular-ui-bootstrap/dist";
var BOOTSTRAP                  = NODE_MODULES + "bootstrap/dist";
var ANGULAR_BOOTSTRAP_NAV_TREE = NODE_MODULES + "angular-bootstrap-nav-tree/dist";

app.use(koaStatic(WWW));
app.use(koaStatic(ANGULAR));
app.use(koaStatic(ANGULAR_UI_BOOTSTRAP));
app.use(koaStatic(ANGULAR_UI_ROUTER));
app.use(koaStatic(BOOTSTRAP));
app.use(koaStatic(ANGULAR_BOOTSTRAP_NAV_TREE));

module.exports.listen = function() {
    console.log("DreamTool WWW is starting on port", config.www.port);
    app.listen(config.www.port);
};
