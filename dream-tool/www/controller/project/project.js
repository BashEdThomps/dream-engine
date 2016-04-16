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

		$scope.modified = function() {
			console.log("Modified");
			ProjectService.setName($scoope.project.name);
			ProjectService.setAuthor($scoope.project.author);
			ProjectService.setDescription($scoope.project.description);
		};
	}
]);
