App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {

        $scope.project = ProjectService.getProject();
        $scope.newResource = ProjectService.createAnimationResource();

        UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);

        $scope.getResourecTypes = function() {
            return ProjectService.getResourecTypes();
        };

        $scope.onNewResourceAddButtonClicked = function() {
            ProjectService.pushResource($scope.newResource);
            UIService.addTreeProjectResource(UIService.createTreeProjectResource($scope.newResource));
            $scope.newResource = ProjectService.createAnimationResource();
        };

        $scope.onNewResourceClearButtonClicked = function() {
            $scope.newResource = ProjectService.createAnimationResource();
        };

        $scope.onRemoveResourceButtonClicked = function(uuid) {
            ProjectService.getResourceByUUID(uuid,function(resource){
                if (resource !== null) {
                    ProjectService.removeResource(resource);
                    UIService.removeTreeProjectResourceByUUID(uuid);
                } else {
                    UIService.addAlert("Error: Could not remove resource! ","danger");
                }
            });
        };
    }
]);
