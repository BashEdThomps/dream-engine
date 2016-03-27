App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        $scope.project = ProjectService.getProject();
        $scope.newResource = ProjectService.createAnimationResource();
        UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);
    }
]);
