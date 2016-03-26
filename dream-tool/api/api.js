var config    = require ('../config.json');
var koa       = require('koa');
var app       = koa();
var sys       = require('sys');
var exec      = require('child_process').exec;
var parse     = require('co-body');

module.exports.listen = function() {
    console.log("DreamTool API is starting on port", config.api.port);
    app.listen(config.api.port);
};

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

