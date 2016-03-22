App.factory('apiConnector',
    ['$http',
    function($http) {
      return {
        runFile: function(path, callback) {
            callback();
        },
        // File Management
        loadFile: function(path, callback) {
            callback();
        },
        saveFile: function(path, callback) {
            callback();
        },
        // Application
        getVersion: function(callback) {
            callback();
        },
      };
    }
]);
