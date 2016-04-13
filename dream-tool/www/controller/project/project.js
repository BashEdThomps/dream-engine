App.controller("Project",
	["$scope","ProjectService","UIService",
	function($scope,ProjectService,UIService) {
		if (ProjectService.isProjectOpen()){
			$scope.project = ProjectService.getProject();
			UIService.setBreadcrumbs([$scope.project.name]);
			UIService.update();
		}
	}
]);
