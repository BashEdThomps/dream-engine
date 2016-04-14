App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {

        if (ProjectService.isProjectOpen()) {
            $scope.project = ProjectService.getProject();
            $scope.currentResource = ProjectService.createResource();
            UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);
            UIService.update();
        } else {
            $state.go("Home");
        }

        $scope.getResourecTypes = function() {
            return ProjectService.getResourecTypes();
        };

        $scope.onNewResourceAddButtonClicked = function() {
            if ($scope.currentResource.name === "") {
                UIService.addAlert("Resource name cannot be blank.","danger");
            } else {
                ProjectService.pushResource($scope.currentResource);
                UIService.addTreeProjectResource(UIService.createTreeProjectResource($scope.currentResource));
                $scope.currentResource = ProjectService.createResource();
            }
        };

        $scope.onNewResourceClearButtonClicked = function() {
            $scope.currentResource = ProjectService.createResource();
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

        $scope.onResourceSelected = function(uuid) {
            console.log("Selected Resource",uuid);
            ProjectService.getResourceByUUID(uuid,function(resource){
                $scope.currentResource = resource;
            });
        };
    }
]);
