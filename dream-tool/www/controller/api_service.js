App.service('ApiService',
    ['$http',
    function($http) {
        this.uploadResource = function(path,buffer,callback) {
            $http({
				url: "/upload/resource",
				method: "POST",
				data: {
                    path   : path,
                    buffer : buffer
                }
			}, function successCallback() {
				callback(true);
			}, function errorCallback() {
				callback(false);
			});
        };

        this.runDreamProject = function(projectName, callback) {
			$http({
				url: "/run",
				method: "POST",
				data: projectName
			}, function successCallback() {
				callback(true);
			}, function errorCallback() {
				callback(false);
			});
        };

        return this;
    }
]);
