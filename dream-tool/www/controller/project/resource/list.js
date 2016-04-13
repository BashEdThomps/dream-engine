App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {

        if (ProjectService.isProjectOpen()) {
            $scope.project = ProjectService.getProject();
            $scope.newResource = ProjectService.createResource();
            UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);
            UIService.update();
        }

        $scope.getResourecTypes = function() {
            return ProjectService.getResourecTypes();
        };

        $scope.onNewResourceAddButtonClicked = function() {
            if ($scope.newResource.name === "") {
                UIService.addAlert("Resource name cannot be blank.","danger");
            } else {
                ProjectService.pushResource($scope.newResource);
                UIService.addTreeProjectResource(UIService.createTreeProjectResource($scope.newResource));
                $scope.newResource = ProjectService.createResource();
            }
        };

        $scope.onNewResourceClearButtonClicked = function() {
            $scope.newResource = ProjectService.createResource();
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
