var sys       = require('sys');
var exec      = require('child_process').exec;

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

module.exports = this;
