App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {
        $scope.newResource = ProjectService.createResource();
        UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);
    }
]);
