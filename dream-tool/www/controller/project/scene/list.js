App.controller("ProjectSceneList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        $scope.project  = ProjectService.getProject();
        $scope.newScene = ProjectService.createScene();
        UIService.setBreadcrumbs([ProjectService.getName(),"Scenes"]);

        $scope.onNewSceneAddButtonClicked = function() {
            console.log($scope.newScene);
            ProjectService.pushScene($scope.newScene);
            $scope.newScene = ProjectService.createScene();
        };

        $scope.onNewSceneClearButtonClicked = function() {
            $scope.newScene = ProjectService.createScene();
        };

        $scope.onRemoveSceneButtonClicked = function(name) {
            ProjectService.getSceneByName(name,function(scene){
                if (scene !== null) {
                    ProjectService.removeScene(scene);
                    UIService.addAlert("Removed scene "+name,"success");
                } else {
                    UIService.addAlert("Could not remove "+name,"danger");
                }
            });
        };
    }
]);
