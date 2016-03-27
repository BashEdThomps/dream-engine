App.controller("ProjectSceneList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        $scope.project  = ProjectService.getProject();
        $scope.newScene = ProjectService.createScene();
        UIService.setBreadcrumbs([ProjectService.getName(),"Scenes"]);
    }
]);
