App.controller("ProjectAssetEditor",
["$scope","$state","$stateParams","$window","ProjectService","UIService","UtilService","ApiService",
function($scope,$state,$stateParams,$window,ProjectService,UIService,UtilService,ApiService) {
  $scope.assetUUID = $stateParams.asset;
  if (ProjectService.isProjectOpen()) {
    $scope.project = ProjectService.getProject();
    $scope.getResourecTypes = ProjectService.getResourecTypes;
    ProjectService.getAssetByUUID($scope.assetUUID, function(asset) {
      if (asset === null) {
        UIService.addAlert("Error: Unable to find asset"+$scope.assetUUID,"danger");
      } else {
        $scope.asset = asset;
        console.log("Editing Asset:",$scope.asset.name);
        UIService.setBreadcrumbs([ProjectService.getName(),"Assets",$scope.asset.name]);
        UIService.update();
        switch ($scope.asset.type) {
          case "Script":
          $window.setTimeout(function() {
            console.log("setting up script editor");
            var editor = $scope.getAceEditor();
            editor.setTheme("ace/theme/vibrant_ink");
            editor.getSession().setMode("ace/mode/javascript");
            editor.setShowPrintMargin(false);
            editor.setFontSize(14);
            editor.setKeyboardHandler('ace/keyboard/vim');
            var editorElement = $scope.getAceEditorElement();
            editorElement.style.position = "relative";
            editorElement.style.height   = "600px";
            editorElement.style.bottom   = "0px";
            editorElement.style.left     = "0px";
            editorElement.style.right    = "0px";
            console.log("using",editor,editorElement);
            $scope.getScriptContent();
          },100);
          break;
        }
      }
    });
  } else {
    $state.go("Home");
  }

  $scope.getScriptContent = function() {
    ApiService.readAsset(ProjectService.getProjectUUID(),$scope.asset.type.toLowerCase(),$scope.asset.uuid,"js",function(data)
    {
      $scope.getAceEditor().setValue(data);
    });
  };

  $scope.getAceEditorElement = function() {
    return $window.document.getElementById("editor");
  };

  $scope.getAceEditor = function() {
    return $window.ace.edit("editor");
  };

  $scope.onScriptEditorSaveClicked = function() {
    var ace = $scope.getAceEditor();
    var script = ace.getValue();
    console.log("Saving script",script);
    ProjectService.saveScriptAsset($scope.asset,script,function(success) {
      if (success) {
        UIService.addAlert("Saved Script","success");
      }
      else {
        UIService.addAlert("Error Saving Script","danger");
      }
    });
  };

  $scope.onScriptEditorVimModeClicked = function() {
    var ace = $scope.getAceEditor();
    var handler = ace.getKeyboardHandler();
    UIService.addAlert("Vim Mode: "+handler,"success");
  };

}
]);
