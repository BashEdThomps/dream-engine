App.service('ApiService',
    ['$http',
    function($http) {
        this.runDreamProject = function(projectName, callback) {
			$http({
				url: "/run",
				method: "POST",
				data: projectName
			}).then(
                function successCallback() {
				    callback(true);
			     }, function errorCallback() {
				    callback(false);
			    }
            );
        };

        this.uploadResource = function(path,data,callback) {
            $http({
                url    : path,
                data   : {data: data},
                method : "POST"
            }).then(
                function successCallback() {
                    callback(true);
                },function errorCallback() {
                    callback(false);
                }
            );
        };

        return this;
    }
]);
