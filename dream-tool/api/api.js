var config    = require ('../config.json');
var koa       = require('koa');
var app       = koa();

module.exports.listen = function() {
    console.log("DreamTool API is starting on port", config.api.port);
    app.listen(config.api.port);
};
