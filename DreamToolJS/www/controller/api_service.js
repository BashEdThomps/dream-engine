App.service('ApiService',
    ['$http',
        function($http) {
            this.execDream = function(exec, uuid, callback) {
                $http({
                    url: "/exec",
                    method: "POST",
                    data: {
                        exec: exec,
                        uuid: uuid
                    }
                }).then(
                    function successCallback() {
                        callback(true);
                    }, function errorCallback() {
                        callback(false);
                    }
                );
            };

            this.uploadAsset = function(path,data,callback) {
                $http({
                    url    : path,
                    data   : { data: data },
                    method : "POST"
                }).then(
                    function successCallback() {
                        callback(true);
                    },function errorCallback() {
                        callback(false);
                    }
                );
            };

            this.assetExists = function(project,type,uuid,format,callback) {
                $http({
                    url    : "/asset/exists/"+project+"/"+type+"/"+uuid+"/"+format,
                    method : "get",
                }).then(function success(resp){
                    callback(JSON.parse(resp.data));
                },function failure(){
                    callback(false);
                });
            };

            this.generateAssetURL = function(project,type,uuid,format) {
                return "/asset/"+project+"/"+type+"/"+uuid+"/"+format;
            };

            this.readAsset = function(project,type,uuid,format,callback) {
                var api=this;
                $http({
                    url    : this.generateAssetURL(project,type,uuid,format),
                    method : "get",
                }).then(function success(resp){
                    callback(resp.data);
                },function failure(){
                    api.getAssetDefault(type,format,function(defaultData) {
                        callback(defaultData);
                    });
                });
            };

            this.readAssetAsBlob = function(project,type,uuid,format,callback) {
                var api=this;
                $http({
                    url    : this.generateAssetURL(project,type,uuid,format),
                    method : "get",
                    responseType: "blob"
                })
                    .then(function success(resp){
                        callback(resp.data);
                    },
                        function failure(){
                            api.getAssetDefault(type,format,function(defaultData) {
                                callback(defaultData);
                            });
                        });
            };

            this.getTemplateList = function(callback) {
                $http({
                    url : "/templates",
                    method: "get"
                }).then(
                    function success(resp) {
                        callback(resp.data);
                    },
                    function error() {
                        callback(null);
                    }
                );
            };


            this.getTemplateData = function(type,name,file,callback) {
                $http({
                    url: "/templates/"+type+"/"+name+"/"+file,
                    method: "get"
                }).then(function success(resp) {
                    callback(resp.data);
                },function error() {
                    callback("");
                });
            };

            this.createProjectDirectory = function(projectUUID,callback) {
                $http({
                    url    : '/create',
                    data   : { uuid : projectUUID },
                    method : 'POST'
                }).then(function success() {
                    callback(true);
                }, function error() {
                    callback(false);
                });
            };

            this.saveProjectFile = function(projObj,callback) {
                var api = this;
                this.projectDirectoryExists(projObj,function(exists){
                    if (exists) {
                        api.saveProjectAction(projObj,function(success3) {
                            console.log("Project Saved",projObj);
                            callback(success3);
                        });
                    } else {
                        api.createProjectDirectory(projObj.uuid,function(success2) {
                            console.log("Created project directory for",projObj.uuid);
                            api.saveProjectAction(projObj,function(success2){
                                console.log("Project Saved",projObj);
                                callback(success2);
                            });
                        });
                    }
                });
            };

            this.projectDirectoryExists = function(projObj, callback) {
                this.readProjectList(function(projList){
                    var retval = false;
                    projList.forEach(function(proj){
                        if (proj.uuid === projObj.uuid) {
                            retval = true;
                            return;
                        }
                    });
                    callback(retval);
                });
            };

            this.saveProjectAction = function(projObj,callback) {
                $http({
                    url    : '/save/'+projObj.uuid,
                    method : 'POST',
                    data   : { project : projObj }
                }).then(function success() {
                    callback(true);
                }, function failure() {
                    callback(false);
                });
            };

            this.saveProjectTarball = function(projObj,callback) {
                console.log("Saveing project as tarball...");
                $http({
                    url    : '/compress/'+projObj.uuid,
                    method : 'GET',
                }).then(function success(resp){
                    console.log("compression response",resp);
                    callback(true,resp);
                },function failure(resp){
                    callback(false,resp);
                });
            };

            this.readProjectList = function (callback) {
                $http({
                    url    : "/projectlist",
                    method : 'GET',
                }).then(function success(resp) {
                    callback(resp.data);
                },function error(resp){
                    callback(null);
                });
            };

            this.readProjectFile = function(project,callback){
                $http({
                    method: 'GET',
                    url: "/projectfile/"+project
                }).then(function success(resp) {
                    callback(resp.data);
                },function failure(resp) {
                    callback(null);
                });
            };

            this.deleteAssetData = function(project,type,uuid,callback) {
                $http({
                    method:"DELETE",
                    url: "/asset/delete/"+project+"/"+type+"/"+uuid
                }
                ).then(function success(response) {
                    callback(true);
                },function error(response) {
                    console.error(response);
                    callback(false);
                });
            }

            return this;
        }
    ]);
