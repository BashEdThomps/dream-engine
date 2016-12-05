var sys       = require('sys');
var exec      = require('child_process').exec;

module.exports.execDream = function(bin,projDir,uuid) {
	var fullExec = bin +" --project-dir "+projDir+" --uuid "+uuid;
	console.log("Executing:",fullExec);
	var child = exec(fullExec, function (error, stdout, stderr) {
  	sys.print('stdout: ' + stdout);
  	sys.print('stderr: ' + stderr);
  	if (error !== null) {
    	console.log('exec error: ' + error);
  	}
  });
	console.log("exec'd child:");
};

module.exports = this;
