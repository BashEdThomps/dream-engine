App.controller("ProjectAssetList",
    ["$scope","$state","ProjectService","UIService","UtilService","ApiService",
    function($scope,$state,ProjectService,UIService,UtilService,ApiService) {

        if (ProjectService.isProjectOpen()) {
            $scope.project = ProjectService.getProject();
            UIService.setBreadcrumbs([ProjectService.getName(),"Assets"]);
            UIService.update();
        } else {
            $state.go("Home");
        }

        $scope.goToAssetEditor = function(uuid) {
            $state.go("ProjectAssetEditor",{asset: uuid});
        };

        $scope.getResourecTypes = function() {
            return ProjectService.getResourecTypes();
        };

        $scope.onNewAssetButtonClicked = function() {
            $scope.currentAsset = ProjectService.createAsset();
            ProjectService.pushAsset($scope.currentAsset);
            UIService.addTreeProjectAsset(UIService.createTreeProjectAsset($scope.currentAsset));
            UIService.update();
        };

        $scope.onRemoveAssetButtonClicked = function(uuid) {
            ProjectService.getAssetByUUID(uuid,function(asset){
                if (asset !== null) {
                    ProjectService.removeAsset(asset);
                    UIService.removeTreeProjectAssetByUUID(uuid);
                } else {
                    UIService.addAlert("Error: Could not remove asset!","danger");
                }
            });
        };

        $scope.onAssetSelected = function(uuid) {
            console.log("Selected Asset",uuid);
            ProjectService.getAssetByUUID(uuid,function(asset){
                $scope.currentAsset = asset;
                $scope.updateAssetUIVariables();
            });
        };

        $scope.updateAssetUIVariables = function() {
            console.log("Updating asset variables");
            ProjectService.assetHasModelObj($scope.currentAsset.uuid,function(result) {
                console.log("ModelObj Asset Exists",result);
                $scope.hasModelObj = result;
            });

            ProjectService.assetHasModelMtl($scope.currentAsset.uuid,function(result){
                console.log("ModelMtl Asset Exists",result);
                $scope.hasModelMtl = result;
            });
        };

        $scope.modified = function() {
            console.log("Asset List Item Modified");
            ProjectService.updateAsset($scope.currentAsset);
            UIService.updateAsset($scope.currentAsset);
        };

        $scope.onAssetModelWavefrontUploadButtonClicked = function() {
            var objFile = document.getElementById('wf-obj-file');
            UtilService.readFileAsBinaryFromElement(objFile, function(data) {
                var path = ProjectService.getProjectUUID()+"/asset/model/"+$scope.currentAsset.uuid+"/obj";
                ApiService.uploadAsset(path,data,function(success){
                    if (success) {
                        UIService.addAlert("Asset uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading asset.","danger");
                    }
                });
            });

            var mtlFile = document.getElementById('wf-mtl-file');
            UtilService.readFileAsBinaryFromElement(mtlFile, function (data) {
                var path = ProjectService.getProjectUUID()+"/asset/model/"+$scope.currentAsset.uuid+"/mtl";
                ApiService.uploadAsset(path,data,function(success) {
                    if (success) {
                        UIService.addAlert("Asset uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading asset.","danger");
                    }
                });
            });
        };

        $scope.onAssetAudioWaveUploadButtonClicked = function() {
            var wavFile = document.getElementById('wav-file');
            UtilService.readFileAsBinaryFromElement(wavFile, function(data) {
                var path = ProjectService.getProjectUUID()+"/asset/audio/"+$scope.currentAsset.uuid+"/wav";
                ApiService.uploadAsset(path,data,function(success){
                    if (success) {
                        UIService.addAlert("Asset uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading asset.","danger");
                    }
                });
            });
        };

        $scope.onAssetAudioOggUploadButtonClicked = function() {
            var oggFile = document.getElementById('ogg-file');
            UtilService.readFileAsBinaryFromElement(oggFile, function(data) {
                var path = ProjectService.getProjectUUID()+"/asset/audio/"+$scope.currentAsset.uuid+"/ogg";
                ApiService.uploadAsset(path,data,
                    function(success){
                    if (success) {
                        UIService.addAlert("Asset uploaded successfuly.","success");
                    }
                    else {
                        UIService.addAlert("Error uploading asset.","danger");
                    }
                });
            });
        };

    }
]);
