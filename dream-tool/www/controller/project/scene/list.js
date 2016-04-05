App.controller("ProjectSceneList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        $scope.project  = ProjectService.getProject();
        $scope.newScene = ProjectService.createScene();
        UIService.setBreadcrumbs([ProjectService.getName(),"Scenes"]);

        $scope.onNewSceneAddButtonClicked = function() {
            console.log($scope.newScene);
            ProjectService.pushScene($scope.newScene);
            UIService.addTreeProjectScene(UIService.createTreeProjectScene($scope.newScene.name));
            $scope.newScene = ProjectService.createScene();
        };

        $scope.onNewSceneClearButtonClicked = function() {
            $scope.newScene = ProjectService.createScene();
        };

        $scope.onRemoveSceneButtonClicked = function(name) {
            ProjectService.getSceneByUUID(name,function(scene){
                if (scene !== null) {
                    ProjectService.removeScene(scene);
                    UIService.removeTreeProjectSceneByUUID(name);
                    UIService.addAlert("Removed scene "+name,"success");
                } else {
                    UIService.addAlert("Could not remove "+name,"danger");
                }
            });
        };
    }
]);
