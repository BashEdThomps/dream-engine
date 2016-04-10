App.controller("ProjectResourceEditor",
    ["$scope","$state","$stateParams","ProjectService","UIService","UtilService","ApiService",
    function($scope,$state,$stateParams,ProjectService,UIService,UtilService,ApiService) {
        $scope.resourceUUID = $stateParams.resource;
        ProjectService.getResourceByUUID($scope.resourceUUID, function(resource) {
            if (resource === null) {
                UIService.addAlert("Error: Unable to find resource"+$scope.resourceUUID,"danger");
            } else {
                $scope.resource = resource;
                console.log("Editing Resource:",$scope.resource.name);
                UIService.setBreadcrumbs([ProjectService.getName(),"Resources",$scope.resource.name]);
                UIService.update();
            }

        });

        $scope.getResourecTypes = ProjectService.getResourecTypes;

        $scope.onResourceModelWavefrontUploadButtonClicked = function() {
            var objFile = document.getElementById('wf-obj-file');
            UtilService.readFileAsBinaryFromElement(objFile, function(data) {
                var path = ProjectService.getProjectUUID()+"/resource/models/"+$scope.resourceUUID+"/obj";
                ApiService.uploadResource(path,data,function(success){
                    if (success) {
                        UIService.addAlert("Resource uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading resource.","danger");
                    }
                });
            });

            var mtlFile = document.getElementById('wf-mtl-file');
            UtilService.readFileAsBinaryFromElement(mtlFile, function (data) {
                var path = ProjectService.getProjectUUID()+"/resource/models/"+$scope.resourceUUID+"/mtl";
                ApiService.uploadResource(path,data,function(success) {
                    if (success) {
                        UIService.addAlert("Resource uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading resource.","danger");
                    }
                });
            });
        };

        $scope.onResourceAudioWaveUploadButtonClicked = function() {
            var wavFile = document.getElementById('wav-file');
            UtilService.readFileAsBinaryFromElement(wavFile, function(data) {
                var path = ProjectService.getProjectUUID()+"/resource/audio/"+$scope.resourceUUID+"/wav";
                ApiService.uploadResource(path,data,function(success){
                    if (success) {
                        UIService.addAlert("Resource uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading resource.","danger");
                    }
                });
            });
        };

        $scope.onResourceAudioOggUploadButtonClicked = function() {
            var oggFile = document.getElementById('ogg-file');
            UtilService.readFileAsBinaryFromElement(oggFile, function(data) {
                var path = ProjectService.getProjectUUID()+"/resource/audio/"+$scope.resourceUUID+"/ogg";
                ApiService.uploadResource(path,data,
                    function(success){
                    if (success) {
                        UIService.addAlert("Resource uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading resource.","danger");
                    }
                });
            });
        };
    }
]);
