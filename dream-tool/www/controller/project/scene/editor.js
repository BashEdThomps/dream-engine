App.controller("ProjectSceneEditor",
    ["$scope","$state","$stateParams","ProjectService","UIService",
    function($scope,$state,$stateParams,ProjectService,UIService) {
        $scope.sceneUUID = $stateParams.scene;
         ProjectService.getSceneByUUID($scope.sceneUUID,function (scene){
             if (scene === null) {
                UIService.addAlert("Error: Unable to find scene!"+$scop.sceneUUID,"danger");
            } else {
                $scope.scene = scene;
                UIService.setBreadcrumbs([ProjectService.getName(),"Scenes",$scope.scene.name]);
                UIService.update();
            }
         });

        $scope.projectModified = function () {
            ProjectService.setProjectModified(true);
            UIService.update();
        };
    }
]);
