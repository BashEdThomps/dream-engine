App.controller("home",["$scope","ProjectService","UIService",
function($scope,ProjectService,UIService) {
  UIService.setBreadcrumbs(["Home"]);
  // Help Pages --------------------------------------------------------------

  $scope.showProject = true;

  $scope.onProjectClicked = function() {
    $scope.hideAllHelp();
    $scope.showProject = true;
  };

  $scope.onProjectConfigurationSelected = function() {
    $scope.hideAllHelp();
    $scope.showProjectConfiguration = true;
  };

  $scope.onProjectSceneClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectScene = true;
  };

  $scope.onProjectSceneSceneObjectClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectSceneSceneObject = true;
  };

  // Assets ----------------------------------------------------------------------

  $scope.onProjectAssetsClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssets = true;
  };

  $scope.onProjectAssetsAnimationClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssetsAnimation = true;
  };

  $scope.onProjectAssetsAudioClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssetsAudio = true;
  };

  $scope.onProjectAssetsModelClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssetsModel = true;
  };


  $scope.onProjectAssetsLightClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssetsLight = true;
  };


  $scope.onProjectAssetsPhysicsObjectClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssetsPhysicsObject = true;
  };


  $scope.onProjectAssetsShaderClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssetsShader = true;
  };


  $scope.onProjectAssetsScriptClicked = function() {
    $scope.hideAllHelp();
    $scope.showProjectAssetsScript = true;
  };

  // Hide All --------------------------------------------------------------------

  $scope.hideAllHelp = function() {
    $scope.showProject                    = false;
    $scope.showProjectConfiguration       = false;
    $scope.showProjectScene               = false;
    $scope.showProjectSceneSceneObject    = false;
    $scope.showProjectAssets              = false;
    $scope.showProjectAssetsAnimation     = false;
    $scope.showProjectAssetsAudio         = false;
    $scope.showProjectAssetsLight         = false;
    $scope.showProjectAssetsModel         = false;
    $scope.showProjectAssetsPhysicsObject = false;
    $scope.showProjectAssetsShader        = false;
    $scope.showProjectAssetsScript        = false;
  };

  // Tree Data

  $scope.helpTreeData = [{
      label: "Project", onSelect: $scope.onProjectClicked,
      children:
      [
        {
          label: "Scene", onSelect: $scope.onProjectSceneClicked,
          children:
          [
            { label: "Scene Object", onSelect: $scope.onProjectSceneSceneObjectClicked }
          ]
        },
        {
          label: "Assets", onSelect: $scope.onProjectAssetsClicked,
          children :
          [
            { label: "Animation",      onSelect: $scope.onProjectAssetsAnimationClicked     },
            { label: "Audio",          onSelect: $scope.onProjectAssetsAudioClicked         },
            { label: "Light",          onSelect: $scope.onProjectAssetsLightClicked         },
            { label: "Model",          onSelect: $scope.onProjectAssetsModelClicked         },
            { label: "Physics Object", onSelect: $scope.onProjectAssetsPhysicsObjectClicked },
            { label: "Shader",         onSelect: $scope.onProjectAssetsShaderClicked        },
            { label: "Script",         onSelect: $scope.onProjectAssetsScriptClicked        }
          ]
        }
      ],
    }
  ];
}]);
