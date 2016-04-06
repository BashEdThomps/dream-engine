App.controller("ProjectResourceEditor",
    ["$scope","$state","$stateParams","ProjectService","UIService",
    function($scope,$state,$stateParams,ProjectService,UIService) {
        $scope.resourceUUID = $stateParams.resource;
        ProjectService.getResourceByUUID($scope.resourceUUID, function(resource) {
            if (resource === null) {
                UIService.addAlert("Error: Unable to find resource"+$scope.resourceUUID,"danger");
            } else {
                $scope.resource = resource;
                console.log("Editing Resource:",$scope.resource.name);
                UIService.setBreadcrumbs([ProjectService.getName(),"Resources",$scope.resource.name]);
                UIService.update();
            }

        });
        $scope.getResourecTypes = ProjectService.getResourecTypes;
    }
]);
