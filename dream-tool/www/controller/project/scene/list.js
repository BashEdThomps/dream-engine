App.controller("ProjectSceneList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        if (ProjectService.isProjectOpen()) {
            UIService.setBreadcrumbs([ProjectService.getName(),"Scenes"]);
            $scope.project  = ProjectService.getProject();
            UIService.update();
        } else {
            $state.go("Home");
        }

        $scope.onNewSceneButtonClicked = function() {
            $scope.currentScene = ProjectService.createScene();
            ProjectService.pushScene($scope.currentScene);
            UIService.addTreeProjectScene(UIService.createTreeProjectScene($scope.currentScene));
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

        $scope.modified = function() {
            ProjectService.setSceneAnimationEnabled($scope.currentScene);
            ProjectService.setSceneInputEnabled($scope.currentScene);
            ProjectService.setSceneAudioEnabled($scope.currentScene);
            ProjectService.setSceneBulletEnabled($scope.currentScene);
            ProjectService.setSceneName($scope.currentScene);
        };
    }
]);
