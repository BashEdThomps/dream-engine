App.controller("ProjectSceneList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        if (ProjectService.isProjectOpen()) {
            UIService.setBreadcrumbs([ProjectService.getName(),"Scenes"]);
            $scope.project  = ProjectService.getProject();
            $scope.newScene = ProjectService.createScene();
            UIService.update();
        }

        $scope.onNewSceneAddButtonClicked = function() {
            if ($scope.newScene.name === "") {
                UIService.addAlert("Scene name cannot be blank.","danger");
            } else {
                ProjectService.pushScene($scope.newScene);
                UIService.addTreeProjectScene(UIService.createTreeProjectScene($scope.newScene));
                $scope.newScene = ProjectService.createScene();
            }
        };

        $scope.onNewSceneClearButtonClicked = function() {
            $scope.newScene = ProjectService.createScene();
        };

        $scope.onRemoveSceneButtonClicked = function(uuid) {
            ProjectService.getSceneByUUID(uuid,function(scene){
                if (scene !== null) {
                    ProjectService.removeScene(scene);
                    UIService.removeTreeProjectSceneByUUID(uuid);
                } else {
                    UIService.addAlert("Error: Could not remove scene!" ,"danger");
                }
            });
        };
    }
]);
