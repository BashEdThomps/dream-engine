App.controller("ProjectAssetEditor",
  ["$scope","$state","$stateParams","$window","ProjectService","UIService","UtilService","ApiService",'ngAudio',
  function($scope,$state,$stateParams,$window,ProjectService,UIService,UtilService,ApiService,ngAudio) {

    var SCRIPT_EDITOR                  = "script-editor";
    var VERTEX_SHADER_EDITOR           = "vertex-shader-editor";
    var FRAGMENT_SHADER_EDITOR         = "fragment-shader-editor";

    $scope.assetUUID = $stateParams.asset;
    $scope.hasScriptEditor = false;
    $scope.hasVertexShaderEditor = false;
    $scope.hasFragmentShaderEditor = false;

    $scope.createScriptEditor = function() {
      if (!$scope.hasScriptEditor) {
        console.log("Creating New Script Editor");
        var editor = $scope.getScriptEditor();
        if (!editor){
          console.error("Could Not Find Script Editor");
          return;
        }
        editor.setTheme("ace/theme/vibrant_ink");
        editor.setShowPrintMargin(false);
        editor.setFontSize(14);
        editor.setKeyboardHandler('ace/keyboard/vim');
        if ($scope.asset.format == ProjectService.ASSET_FORMAT_CHAI) {
          console.log("Ace does not support Chai syntax highlighting");
        }
        var editorElement = $scope.getScriptEditorElement();
        if (!editorElement) {
          console.error("Could not find Editor Element");
          return;
        }
        editorElement.style.position = "relative";
        editorElement.style.height   = "80%";
        editorElement.style.bottom   = "0px";
        editorElement.style.left     = "0px";
        editorElement.style.right    = "0px";
        $scope.getScriptContent();
        $scope.hasScriptEditor = true;
      } else {
        console.log("Script Editor Exists");
      }
    };

    $scope.createVertexShaderEditor = function() {
      if (!$scope.hasVertexShaderEditor) {
        console.log("Creating New Vertex Shader Editor");
        var editor = $scope.getVertexShaderEditor();
        if (!editor){
          console.error("Could Not Find Vertex Shader Editor");
          return;
        }
        editor.setTheme("ace/theme/vibrant_ink");
        editor.setShowPrintMargin(false);
        editor.setFontSize(14);
        editor.setKeyboardHandler('ace/keyboard/vim');
        editor.getSession().setMode("ace/mode/glsl");

        var editorElement = $scope.getVertexShaderEditorElement();
        if (!editorElement) {
          console.error("Could not find Vertex Shader Editor Element");
          return;
        }
        editorElement.style.position = "relative";
        editorElement.style.height   = "50%";
        editorElement.style.bottom   = "0px";
        editorElement.style.left     = "0px";
        editorElement.style.right    = "0px";
        $scope.getVertexShaderContent();
        $scope.hasVertexShaderEditor = true;
      } else {
        console.log("Vertex editor exists");
      }
    };

    $scope.createFragmentShaderEditor = function() {
      if (!$scope.hasFragmentShaderEditor) {
        console.log("Creating New Fragment Shader Editor");
        var editor = $scope.getFragmentShaderEditor();
        if (!editor){
          console.error("Could Not Find Fragment Shader Editor");
          return;
        }
        editor.setTheme("ace/theme/vibrant_ink");
        editor.setShowPrintMargin(false);
        editor.setFontSize(14);
        editor.setKeyboardHandler('ace/keyboard/vim');
        editor.getSession().setMode("ace/mode/glsl");

        var editorElement = $scope.getFragmentShaderEditorElement();
        if (!editorElement) {
          console.error("Could not find Fragment Shader Editor Element");
          return;
        }
        editorElement.style.position = "relative";
        editorElement.style.height   = "50%";
        editorElement.style.bottom   = "0px";
        editorElement.style.left     = "0px";
        editorElement.style.right    = "0px";
        $scope.getFragmentShaderContent();
        $scope.hasFragmentShaderEditor = true;
      } else {
        console.log("Fragment Shader Editor Exists");
      }
    };

    $scope.getScriptContent = function() {
      ApiService.readAsset(
        ProjectService.getProjectUUID(),
        $scope.asset.type.toLowerCase(),
        $scope.asset.uuid,
        $scope.asset.format,
        function(data) {
          if (data) {
            $scope.getScriptEditor().setValue(data);
          }
        }
      );
    };

    $scope.getVertexShaderContent = function() {
      ApiService.readAsset(
        ProjectService.getProjectUUID(),
        $scope.asset.type.toLowerCase(),
        $scope.asset.uuid,
        ProjectService.ASSET_FORMAT_SHADER_VERTEX,
        function(data) {
          if (data) {
            $scope.getVertexShaderEditor().setValue(data);
          }
        }
      );
    };

    $scope.getFragmentShaderContent = function() {
      ApiService.readAsset(
        ProjectService.getProjectUUID(),
        $scope.asset.type.toLowerCase(),
        $scope.asset.uuid,
        ProjectService.ASSET_FORMAT_SHADER_FRAGMENT,
        function(data) {
          if (data) {
            $scope.getFragmentShaderEditor().setValue(data);
          }
        }
      );
    };

    $scope.getScriptEditorElement = function() {
      return $window.document.getElementById(SCRIPT_EDITOR);
    };

    $scope.getVertexShaderEditorElement = function() {
      return $window.document.getElementById(VERTEX_SHADER_EDITOR);
    };

    $scope.getFragmentShaderEditorElement = function() {
      return $window.document.getElementById(FRAGMENT_SHADER_EDITOR);
    };

    $scope.getScriptEditor = function() {
      return $window.ace.edit(SCRIPT_EDITOR);
    };

    $scope.getVertexShaderEditor = function() {
      return $window.ace.edit(VERTEX_SHADER_EDITOR);
    };

    $scope.getFragmentShaderEditor = function() {
      return $window.ace.edit(FRAGMENT_SHADER_EDITOR);
    };

    $scope.getDreamAnimationInterpolationTypes = function() {
      return ProjectService.getDreamAnimationInterpolationTypes();
    };

    $scope.onScriptSaveClicked = function() {
      var ace = $scope.getScriptEditor();
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

    $scope.onVertexShaderSaveClicked = function() {
      var vse = $scope.getVertexShaderEditor();
      var vs = vse.getValue();
      console.log("Saving Vertex Shader",vs);
      ProjectService.saveVertexShaderAsset($scope.asset,vs,function(success) {
        if (success) {
          UIService.addAlert("Saved Vertex Shader","success");
        }
        else {
          UIService.addAlert("Error Saving Vertex Shader","danger");
        }
      });
    };

    $scope.onFragmentShaderSaveClicked = function() {
      var fse = $scope.getFragmentShaderEditor();
      var fs = fse.getValue();
      console.log("Saving Fragment Shader",fs);
      ProjectService.saveFragmentShaderAsset($scope.asset,fs,function(success) {
        if (success) {
          UIService.addAlert("Saved Fragment Shader","success");
        }
        else {
          UIService.addAlert("Error Saving Fragment Shader","danger");
        }
      });
    };

    $scope.loadAudioResource = function() {
      console.log("Loading Audio Resource");
      var assetURL = ApiService.generateAssetURL(
        ProjectService.getProjectUUID(),
        $scope.asset.type,
        $scope.asset.uuid,
        $scope.asset.format);
        console.log("AssetEditor: Loading audio asset from",assetURL);
      $scope.audio = ngAudio.load(assetURL);
    };

    $scope.getAudioPlaybackPercentage = function() {
      var percent = 0;
      if ($scope.audio) {
          percent =  Math.round((100/$scope.audio.duration) * $scope.audio.currentTime);
      }

      return {
        width : percent+'%'
      };
    };

    $scope.onEditKeyFrameButtonClicked = function(uuid) {
      console.log("Editing KeyFrame",uuid);
      ProjectService.getKeyFrameFromAssetByUUID($scope.asset,uuid,function(keyFrame) {
        $scope.selectedKeyFrame = keyFrame;
      });
    };

    $scope.onAddKeyFrameButtonClicked = function() {
      var newKeyFrame = ProjectService.createDreamAnimationKeyFrame($scope.asset.keyframes.length+1);
      if (!$scope.asset.keyframes) {
          $scope.asset.keyframes = [];
      }
      $scope.asset.keyframes.push(newKeyFrame);
      ProjectService.updateAsset($scope.asset);
    };

    $scope.onRemoveKeyFrameButtonClicked = function(keyFrameUUID) {
      ProjectService.removeKeyFrameFromAssetByUUID($scope.asset,keyFrameUUID);
      ProjectService.updateAsset($scope.asset);
    };

    // On Load

    if (ProjectService.isProjectOpen()) {
      $scope.project = ProjectService.getProject();
      $scope.getAssetTypeNames = ProjectService.getAssetTypeNames;
      ProjectService.getAssetByUUID($scope.assetUUID, function(asset) {
        if (asset === null) {
          UIService.addAlert("Error: Unable to find asset"+$scope.assetUUID,"danger");
        } else {
          $scope.asset = asset;
          console.log("Editing Asset:",$scope.asset.name);
          UIService.setBreadcrumbs([ProjectService.getName(),"Assets",$scope.asset.name]);
          UIService.update();
          setTimeout(function() {
            switch ($scope.asset.type) {
              case ProjectService.ASSET_TYPE_SCRIPT:
                $scope.createScriptEditor();
                break;
              case ProjectService.ASSET_TYPE_SHADER:
                $scope.createVertexShaderEditor();
                $scope.createFragmentShaderEditor();
                break;
            case ProjectService.ASSET_TYPE_AUDIO:
                $scope.loadAudioResource();
                break;
            }
          },50);
        }
      });
    } else {
      UIService.addAlert("No Project Open to Edit!","danger");
      $state.go("Home");
    }
  }
]);
