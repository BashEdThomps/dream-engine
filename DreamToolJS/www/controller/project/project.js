App.controller("Project",
	["$scope","$state","ProjectService","UIService",
	function($scope,$state,ProjectService,UIService) {
		if (ProjectService.isProjectOpen()){
			$scope.project = ProjectService.getProject();

			ProjectService.getSceneList(function(sceneList) {
				$scope.sceneList = sceneList;
			});

			UIService.setBreadcrumbs([$scope.project.name]);
			UIService.update();
		} else {
			$state.go("Home");
		}

		$scope.modified = function() {
			console.log("Project Modified");
			ProjectService.updateProject($scope.project);
			UIService.updateProject($scope.project);
		};
	}
]);
