App.controller("ProjectResourceEditor",
    ["$scope","$state","$stateParams","$window","ProjectService","UIService","UtilService","ApiService",
    function($scope,$state,$stateParams,$window,ProjectService,UIService,UtilService,ApiService) {
        $scope.resourceUUID = $stateParams.resource;
        if (ProjectService.isProjectOpen()) {
            $scope.project = ProjectService.getProject();
            $scope.getResourecTypes = ProjectService.getResourecTypes;
            ProjectService.getResourceByUUID($scope.resourceUUID, function(resource) {
                if (resource === null) {
                    UIService.addAlert("Error: Unable to find resource"+$scope.resourceUUID,"danger");
                } else {
                    $scope.resource = resource;
                    console.log("Editing Resource:",$scope.resource.name);
                    UIService.setBreadcrumbs([ProjectService.getName(),"Resources",$scope.resource.name]);
                    UIService.update();
                    switch ($scope.resource.type) {
                        case "Script":
                            $window.setTimeout(function() {
                                console.log("setting up script editor");
                                var editor = $scope.getAceEditor();
                                editor.setTheme("ace/theme/vibrant_ink");
                                editor.getSession().setMode("ace/mode/javascript");
                                editor.setShowPrintMargin(false);
                                editor.setFontSize(18);
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
            ApiService.readResource(ProjectService.getProjectUUID(),$scope.resource.type.toLowerCase(),$scope.resource.uuid,"js",function(data)
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
            ProjectService.saveScriptResource($scope.resource,script,function(success) {
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
