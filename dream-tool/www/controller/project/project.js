App.controller("Project",
	["$scope","$state","ProjectService","UIService",
	function($scope,$state,ProjectService,UIService) {
		if (ProjectService.isProjectOpen()){
			$scope.project = ProjectService.getProject();
			UIService.setBreadcrumbs([$scope.project.name]);
			UIService.update();
		} else {
			$state.go("Home");
		}
	}
]);
