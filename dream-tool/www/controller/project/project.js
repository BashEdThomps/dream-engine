App.controller("Project",
	["$scope","ProjectService",
	function($scope,ProjectService) {
		$scope.project = ProjectService.getProject();
	}
]);
