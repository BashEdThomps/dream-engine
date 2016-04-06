App.controller("ProjectSceneList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        $scope.project  = ProjectService.getProject();
        $scope.newScene = ProjectService.createScene();
        UIService.setBreadcrumbs([ProjectService.getName(),"Scenes"]);

        $scope.onNewSceneAddButtonClicked = function() {
            console.log($scope.newScene);
            ProjectService.pushScene($scope.newScene);
            UIService.addTreeProjectScene(UIService.createTreeProjectScene($scope.newScene));
            $scope.newScene = ProjectService.createScene();
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
