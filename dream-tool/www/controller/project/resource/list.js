App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService","UtilService","ApiService",
    function($scope,$state,ProjectService,UIService,UtilService,ApiService) {

        if (ProjectService.isProjectOpen()) {
            $scope.project = ProjectService.getProject();
            UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);
            UIService.update();
        } else {
            $state.go("Home");
        }

        $scope.goToResourceEditor = function(uuid) {
            $state.go("ProjectResourceEditor",{resource: uuid});
        };

        $scope.getResourecTypes = function() {
            return ProjectService.getResourecTypes();
        };

        $scope.onNewResourceButtonClicked = function() {
            $scope.currentResource = ProjectService.createResource();
            ProjectService.pushResource($scope.currentResource);
            UIService.addTreeProjectResource(UIService.createTreeProjectResource($scope.currentResource));
            UIService.update();
        };

        $scope.onRemoveResourceButtonClicked = function(uuid) {
            ProjectService.getResourceByUUID(uuid,function(resource){
                if (resource !== null) {
                    ProjectService.removeResource(resource);
                    UIService.removeTreeProjectResourceByUUID(uuid);
                } else {
                    UIService.addAlert("Error: Could not remove resource!","danger");
                }
            });
        };

        $scope.onResourceSelected = function(uuid) {
            console.log("Selected Resource",uuid);
            ProjectService.getResourceByUUID(uuid,function(resource){
                $scope.currentResource = resource;
                $scope.updateResourceUIVariables();
            });
        };

        $scope.updateResourceUIVariables = function() {
            console.log("Updating resource variables");
            ProjectService.resourceHasModelObj($scope.currentResource.uuid,function(result) {
                console.log("ModelObj Resource Exists",result);
                $scope.hasModelObj = result;
            });

            ProjectService.resourceHasModelMtl($scope.currentResource.uuid,function(result){
                console.log("ModelMtl Resource Exists",result);
                $scope.hasModelMtl = result;
            });
        };

        $scope.modified = function() {
            console.log("Resource List Item Modified");
            ProjectService.updateResource($scope.currentResource);
            UIService.updateResource($scope.currentResource);
        };

        $scope.onResourceModelWavefrontUploadButtonClicked = function() {
            var objFile = document.getElementById('wf-obj-file');
            UtilService.readFileAsBinaryFromElement(objFile, function(data) {
                var path = ProjectService.getProjectUUID()+"/resource/model/"+$scope.currentResource.uuid+"/obj";
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
                var path = ProjectService.getProjectUUID()+"/resource/model/"+$scope.currentResource.uuid+"/mtl";
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
                var path = ProjectService.getProjectUUID()+"/resource/audio/"+$scope.currentResource.uuid+"/wav";
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
                var path = ProjectService.getProjectUUID()+"/resource/audio/"+$scope.currentResource.uuid+"/ogg";
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
