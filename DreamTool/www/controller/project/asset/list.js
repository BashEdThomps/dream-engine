App.controller("ProjectAssetList",
["$scope","$state","ProjectService","UIService","UtilService","ApiService",
function($scope,$state,ProjectService,UIService,UtilService,ApiService) {

  $scope.assetFormats = {};

  $scope.doOnLoad = function() {
    if (ProjectService.isProjectOpen()) {
      $scope.project = ProjectService.getProject();
      UIService.setBreadcrumbs([ProjectService.getName(),"Assets"]);
      UIService.update();
    } else {
      UIService.addAlert("No Project Open!","danger");
      $state.go("Home");
    }

    ProjectService.getAssetTypesObject(function(assetTypesObj) {
      $scope.assetTypesObject = assetTypesObj;
    });

    ProjectService.getFontAssetFormats (function(fontFormats) {
        $scope.assetFormats.font = fontFormats;
    });

    ProjectService.getAudioAssetFormats (function(audioFormats) {
        $scope.assetFormats.audio = audioFormats;
    });

    ProjectService.getAnimationAssetFormats(function(animFormats){
      $scope.assetFormats.animation = animFormats;
    })

    ProjectService.getPhysicsObjectAssetFormats(function(colShapeFormats){
      $scope.assetFormats.physicsObject = colShapeFormats;
    });

    ProjectService.getModelAssetFormats(function(modelFormats) {
      $scope.assetFormats.model = modelFormats;
    });

    ProjectService.getScriptAssetFormats(function(scriptFormats){
        $scope.assetFormats.script = scriptFormats;
    });

    console.log("Asset Formats:",$scope.assetFormats)
  };

  $scope.isAssetTypeAnimation = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_ANIMATION;
  };


  $scope.isAssetTypeFont = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_FONT;
  };

  $scope.isAssetTypeAudio = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_AUDIO;
  };

  $scope.isAssetTypePhysicsObject = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_PHYSICS_OBJECT;
  };

  $scope.isAssetTypeModel = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_MODEL;
  };

  $scope.isAssetTypeScript= function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_SCRIPT;
  };

  $scope.isAssetTypeShader = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_SHADER;
  };

  $scope.isAudioAssetFormatOgg = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_OGG;
  };

  $scope.isAudioAssetFormatWav = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_WAV;
  };

  $scope.isModelAssetFormatAssimp = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_MODEL_ASSIMP;
  };

  $scope.isScriptAssetFormatChai = function() {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_SCRIPT_CHAI;
  };

  $scope.isPhysicsObjectAssetFormatSphere = function() {
      if (!$scope.currentAsset) return false;
      return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_SPHERE;
  };

  $scope.isPhysicsObjectAssetFormatStaticPlane = function() {
      if (!$scope.currentAsset) return false;
      return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_STATIC_PLANE;
  };

  $scope.goToAssetEditor = function(uuid) {
    $state.go("ProjectAssetEditor",{asset: uuid});
    return;
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
      if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_MODEL_ASSIMP) {
        ProjectService.assetHasModelAssimp($scope.currentAsset.uuid,function(result) {
          console.log("Model Assimp Asset Exists",result);
          $scope.hasModelAssimp = result;
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

  $scope.onAssetModelAssimpUploadButtonClicked = function() {
    UIService.addAlert("Uploading Asset File...","info");
    var assimpFile = document.getElementById('wf-assimp-file');
    UtilService.readFileAsBinaryFromElement(assimpFile, function(data) {
      var path = ProjectService.getProjectUUID()+"/asset/model/"+$scope.currentAsset.uuid+"/assimp";
      ApiService.uploadAsset(path,data,function(success){
        if (success) {
          UIService.addAlert("Model Asset Uploaded Successfuly!","success");
          $scope.updateAssetUIVariables();
        }
        else {
          UIService.addAlert("Error uploading asset.","danger");
        }
      });
    });
  };

  $scope.onAssetAudioOggUploadButtonClicked = function() {
    UIService.addAlert("Uploading Asset File...","info");
    var oggFile = document.getElementById('ogg-file');
    UtilService.readFileAsBinaryFromElement(oggFile, function(data) {
      var path = ProjectService.getProjectUUID()+"/asset/"+$scope.currentAsset.type+"/"+$scope.currentAsset.uuid+"/"+$scope.currentAsset.format;
      ApiService.uploadAsset(path,data, function(success){
        if (success) {
          UIService.addAlert("Asset uploaded successfuly.","success");
          $scope.updateAssetUIVariables();
        }
        else {
          UIService.addAlert("Error uploading asset.","danger");
        }
      });
    });
  };

  $scope.onAssetAudioWavUploadButtonClicked = function() {
    UIService.addAlert("Uploading Asset File...","info");
    var wavFile = document.getElementById('wav-file');
    UtilService.readFileAsBinaryFromElement(wavFile, function(data) {
      var path = ProjectService.getProjectUUID()+"/asset/"+$scope.currentAsset.type+"/"+$scope.currentAsset.uuid+"/"+$scope.currentAsset.format;
      ApiService.uploadAsset(path,data, function(success){
        if (success) {
          UIService.addAlert("Asset uploaded successfuly.","success");
          $scope.updateAssetUIVariables();
        }
        else {
          UIService.addAlert("Error uploading asset.","danger");
        }
      });
    });
  };

  $scope.onDeleteAssetDataButtonClicked = function() {
    console.log("Deleting asset data for",$scope.currentAsset.name,"("+$scope.currentAsset.uuid+")");
    ProjectService.deleteAssetData($scope.currentAsset,function(success) {
      if (success){
        UIService.addAlert("Asset Data Deleted", "success");
        $scope.updateAssetUIVariables();
      } else {
        UIService.addAlert("Unable to Remove Asset Data", "succes");
      }
    });
  }

  $scope.doOnLoad();
}]);
