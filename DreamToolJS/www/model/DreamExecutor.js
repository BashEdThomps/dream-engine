var spawn = require('child_process').spawn;

module.exports.execDream = function(bin,projDir,uuid) {
	var argv = ["--project-dir",projDir,"--uuid",uuid];
  var dreamProcess = spawn(bin,argv);

  dreamProcess.stdout.on('data', (data) => {
    console.log('Dream(stdout):',data.toString());
  });

  dreamProcess.stderr.on('data', (data) => {
    console.log('Dream(stderr):',data.toString());
  });

  dreamProcess.on('close', (code) => {
    if (code !== null) {
      console.log('Dream: exited with code',code.toString());
    } else {
      console.log('Dream: exited with no error code');
    }
  });
};

module.exports = this;
