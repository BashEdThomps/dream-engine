App.controller("ProjectSceneList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        if (ProjectService.isProjectOpen()) {
            UIService.setBreadcrumbs([ProjectService.getName(),"Scenes"]);
            $scope.project  = ProjectService.getProject();
            $scope.currentScene = ProjectService.createScene();
            UIService.update();
        } else {
            $state.go("Home");
        }

        $scope.onNewSceneAddButtonClicked = function() {
            if ($scope.currentScene.name === "") {
                UIService.addAlert("Scene name cannot be blank.","danger");
            } else {
                ProjectService.pushScene($scope.currentScene);
                UIService.addTreeProjectScene(UIService.createTreeProjectScene($scope.currentScene));
                $scope.currentScene = ProjectService.createScene();
            }
        };

        $scope.onNewSceneClearButtonClicked = function() {
            $scope.currentScene = ProjectService.createScene();
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


        $scope.onSceneSelected = function(uuid) {
            console.log("Selected Scene",uuid);
            ProjectService.getSceneByUUID(uuid,function(scene) {
                $scope.currentScene = scene;
            });
        };
    }
]);
