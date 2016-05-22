App.controller("ProjectAssetList",
["$scope","$state","ProjectService","UIService","UtilService","ApiService",
function($scope,$state,ProjectService,UIService,UtilService,ApiService) {

  if (ProjectService.isProjectOpen()) {
    $scope.project = ProjectService.getProject();
    UIService.setBreadcrumbs([ProjectService.getName(),"Assets"]);
    UIService.update();
  } else {
    UIService.addAlert("No Project Open!","danger");
    $state.go("Home");
  }

  $scope.goToAssetEditor = function(uuid) {
    $state.go("ProjectAssetEditor",{asset: uuid});
  };

  $scope.getAssetTypes = function() {
    return ProjectService.getAssetTypes();
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
    console.log("Updating Asset Variables for",$scope.currentAsset);
    if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_AUDIO) {
      console.log("Checking for existing audio asset");
      if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_WAV) {
        ProjectService.assetHasAudioWav($scope.currentAsset.uuid,function(result){
          console.log("AudioWav Asset Exists",result);
          $scope.hasAudioWav = result;
        });
      } else if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_OGG) {
        ProjectService.assetHasAudioOgg($scope.currentAsset.uuid,function(result){
          console.log("AudioOgg Asset Exists",result);
          $scope.hasAudioOgg = result;
        });
      }
    } else if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_MODEL) {
      console.log("Checking for existing model asset");
      if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_MODEL_WAVEFRONT_OBJ) {
        ProjectService.assetHasModelObj($scope.currentAsset.uuid,function(result) {
          console.log("ModelObj Asset Exists",result);
          $scope.hasModelObj = result;
        });
        ProjectService.assetHasModelMtl($scope.currentAsset.uuid,function(result){
          console.log("ModelMtl Asset Exists",result);
          $scope.hasModelMtl = result;
        });
      }
    } else if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_SHADER) {
      console.log("Checking for existsing shader asset");
      ProjectService.assetHasVertexShader($scope.currentAsset.uuid,function(result){
        console.log("VertexShader asset exists",result);
        $scope.hasVertexShader = result;
      });
      ProjectService.assetHasFragmentShader($scope.currentAsset.uuid,function(result){
        console.log("FragmentShader exists",result);
        $scope.hasFragmentShader = result;
      })
    }
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
      ApiService.uploadAsset(path,data, function(success){
        if (success) {
          UIService.addAlert("Asset uploaded successfuly.","success");
        }
        else {
          UIService.addAlert("Error uploading asset.","danger");
        }
      });
    });
  };
}]);
