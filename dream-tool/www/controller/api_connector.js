App.factory('ApiConnector',
    ['$http',
    function($http) {
      return {
        runDreamProject: function(file, callback) {
					$http({
						url: "/run",
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
