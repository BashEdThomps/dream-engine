App.controller("ProjectSceneEditor",
    ["$scope","$state","$stateParams","ProjectService","UIService",
    function($scope,$state,$stateParams,ProjectService,UIService) {
        $scope.sceneName = $stateParams.scene;
         ProjectService.getSceneByUUID($scope.sceneName,function (scene){
             if (scene === null) {
                $scope.scene = ProjectService.createScene();
            } else {
                $scope.scene = scene;
            }
         });

        UIService.setBreadcrumbs([ProjectService.getName(),"Scenes",$scope.sceneName]);
        console.log("Editing Scene:",$scope.sceneName);

        $scope.projectModified = function () {
            ProjectService.setProjectModified(true);
        };

    }
]);
