App.controller("ProjectResourceEditor",
    ["$scope","$state","$stateParams","ProjectService","UIService",
    function($scope,$state,$stateParams,ProjectService,UIService) {
        $scope.resourceUUID = $stateParams.resource;

        ProjectService.getResourceByUUID($scope.resourceUUID, function(resource)
        {
            if (resource === null) {
                $scope.resource = ProjectService.createAnimationResource();
            } else {
                $scope.resource = resource;
            }

        });

        $scope.getResourecTypes = ProjectService.getResourecTypes;

        console.log("Editing Resource:",$scope.resourceName);
        UIService.setBreadcrumbs([ProjectService.getName(),"Resources",$scope.resourceName]);
    }
]);
