App.controller("ProjectAssetEditor",
    [
        "$scope","$state","$stateParams","$window","ProjectService","UIService","UtilService","ApiService",'ngAudio',
        function($scope,$state,$stateParams,$window,ProjectService,UIService,UtilService,ApiService,ngAudio) {

            var SCRIPT_EDITOR = "script-editor";
            var VERTEX_SHADER_EDITOR = "vertex-shader-editor";
            var FRAGMENT_SHADER_EDITOR = "fragment-shader-editor";
            var SPRITE_IMG = "sprite_img";

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

                    editor.setShowPrintMargin(false);
                    editor.setKeyboardHandler('ace/keyboard/vim');
                    editor.getSession().setMode("ace/mode/lua");

                    var keyHandler = editor.getKeyboardHandler();
                    console.log("This is the keyhandler",keyHandler);

                    var editorElement = $scope.getScriptEditorElement();
                    if (!editorElement) {
                        console.error("Could not find Editor Element");
                        return;
                    }

                    editorElement.style.position = "relative";
                    editorElement.style.height   = "550px";
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
                    editor.setShowPrintMargin(false);
                    editor.setKeyboardHandler('ace/keyboard/vim');
                    editor.getSession().setMode("ace/mode/glsl");

                    var editorElement = $scope.getVertexShaderEditorElement();
                    if (!editorElement) {
                        console.error("Could not find Vertex Shader Editor Element");
                        return;
                    }

                    editorElement.style.position = "relative";
                    editorElement.style.height   = "550px";
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

                    editor.setShowPrintMargin(false);
                    editor.setKeyboardHandler('ace/keyboard/vim');
                    editor.getSession().setMode("ace/mode/glsl");

                    var editorElement = $scope.getFragmentShaderEditorElement();
                    if (!editorElement) {
                        console.error("Could not find Fragment Shader Editor Element");
                        return;
                    }

                    editorElement.style.position = "relative";
                    editorElement.style.height   = "550px";
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

            $scope.getSpriteContent = function() {
                ApiService.readAssetAsBlob(
                    ProjectService.getProjectUUID(),
                    $scope.asset.type.toLowerCase(),
                    $scope.asset.uuid,
                    ProjectService.ASSET_FORMAT_SPRITE,
                    function(spriteBlob) {
                        if (spriteBlob) {
                            console.log("Got sprite blob", spriteBlob);
                            var image = $window.document.getElementById(SPRITE_IMG);
                            var reader = new FileReader();
                            reader.onload = function(e) {
                                image.src = e.target.result;
                            };
                            reader.readAsDataURL(spriteBlob);
                        }
                    }
                );
            };

            $scope.loadSpriteAssetPreview = function() {
                console.log("Loading Sprite Preview");
                $scope.getSpriteContent();
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

            $scope.onScriptSaveClicked = function(showAlert) {
                var ace = $scope.getScriptEditor();
                var script = ace.getValue();
                if (script === null || script === "") {
                    console.log("Not saving empty script...");
                    return;
                }
                console.log("Saving script",script);
                ProjectService.saveScriptAsset($scope.asset,script,function(success) {
                    if (showAlert) {
                        if (success) {
                            UIService.addAlert("Saved Script","success");
                        }
                        else {
                            UIService.addAlert("Error Saving Script","danger");
                        }
                    }    
                });
            };

            $scope.onVertexShaderSaveClicked = function(showAlert) {
                var vse = $scope.getVertexShaderEditor();
                var vs = vse.getValue();
                if (vs === null || vs === "") {
                    console.log("Not saving empty vertex shader");
                    return;
                }
                console.log("Saving Vertex Shader",vs);
                ProjectService.saveVertexShaderAsset($scope.asset,vs,function(success) {
                    if (showAlert) {
                        if (success) {
                            UIService.addAlert("Saved Vertex Shader","success");
                        }
                        else {
                            UIService.addAlert("Error Saving Vertex Shader","danger");
                        }
                    }
                });
            };

            $scope.onFragmentShaderSaveClicked = function(showAlert) {
                var fse = $scope.getFragmentShaderEditor();
                var fs = fse.getValue();
                if (fs === null || fs === "") {
                    console.log("Not saving empty frag shader");
                    return;
                }
                console.log("Saving Fragment Shader",fs);
                ProjectService.saveFragmentShaderAsset($scope.asset,fs,function(success) {
                    if (showAlert) {
                        if (success) {
                            UIService.addAlert("Saved Fragment Shader","success");
                        }
                        else {
                            UIService.addAlert("Error Saving Fragment Shader","danger");
                        }
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

            $scope.keyFrameHasNoneInterpolation = function() {
                if (!$scope.selectedKeyFrame) return false;
                return $scope.selectedKeyFrame.interpolation === ProjectService.DREAM_ANIMATION_INTERPOLATION_NONE_TYPE;
            };

            $scope.keyFrameHasBezierInterpolation = function() {
                if (!$scope.selectedKeyFrame) return false;
                return $scope.selectedKeyFrame.interpolation === ProjectService.DREAM_ANIMATION_INTERPOLATION_BEZIER_TYPE;
            };

            $scope.keyFrameHasLinearInterpolation = function() {
                if (!$scope.selectedKeyFrame) return false;
                return $scope.selectedKeyFrame.interpolation === ProjectService.DREAM_ANIMATION_INTERPOLATION_LINEAR_TYPE;
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

            // On Leave
            $scope.$on('$stateChangeStart', function(event, toState, toParams, fromState, fromParams) {
                console.log("Navigating away from page...");
                if ($scope.asset !== undefined) {
                    if ($scope.asset.type === ProjectService.ASSET_TYPE_SHADER) {
                        console.log("Navigating away from a shader");
                        $scope.onVertexShaderSaveClicked(false);
                        $scope.onFragmentShaderSaveClicked(false);
                    } else if ($scope.asset.type === ProjectService.ASSET_TYPE_SCRIPT) {
                        console.log("Navigating away from a script");
                        $scope.onScriptSaveClicked(false);
                    }
                }
            });

            // On Load
            $scope.$on('$stateChangeSuccess', function() {
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
                                    case ProjectService.ASSET_TYPE_SPRITE:
                                        $scope.loadSpriteAssetPreview();
                                        break;
                                }
                            },250);
                        }
                    });
                } else {
                    UIService.addAlert("No Project Open to Edit!","danger");
                    $state.go("Home");
                }
            });
        }
    ]);
