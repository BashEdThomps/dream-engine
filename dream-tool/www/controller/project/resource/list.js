App.controller("ProjectResourceList",
    ["$scope","$state","ProjectService","UIService",
    function($scope,$state,ProjectService,UIService) {

        if (ProjectService.isProjectOpen()) {
            $scope.project = ProjectService.getProject();
            UIService.setBreadcrumbs([ProjectService.getName(),"Resources"]);
            UIService.update();
        } else {
            $state.go("Home");
        }

        $scope.goToResourceEditor = function(uuid) {
            $state.go("ProjectResourceEditor",{resource: uuid});
        };

        $scope.getResourecTypes = function() {
            return ProjectService.getResourecTypes();
        };

        $scope.onNewResourceButtonClicked = function() {
            $scope.currentResource = ProjectService.createResource();
            ProjectService.pushResource($scope.currentResource);
            UIService.addTreeProjectResource(UIService.createTreeProjectResource($scope.currentResource));
            UIService.update();
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

        $scope.modified = function() {
            console.log("Resource List Item Modified");
            ProjectService.updateResource($scope.currentResource);
            UIService.updateResource($scope.currentResource);
        };
    }
]);
