App.factory('apiConnector',
    ['$http',
    function($http) {
			var HOST = "127.0.0.1";
			var PORT = 4001;
			var BASE = "http://"+HOST+":"+PORT+"/"; 
      return {
        runDreamProject: function(file, callback) {
					$http({
						url: BASE + "run",
							method: "POST",
							data: file
					}, function successCallback() {
						callback(true);
					}, function errorCallback() {
						callback(false);
					});
        },
      };
    }
]);
