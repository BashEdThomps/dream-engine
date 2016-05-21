App.controller("Project",
	["$scope","$state","ProjectService","UIService",
	function($scope,$state,ProjectService,UIService) {
		if (ProjectService.isProjectOpen()){
			$scope.project = ProjectService.getProject();
			ProjectService.getSelectedPlugins(function(selectedPlugins) {
				$scope.selectedPlugins = selectedPlugins;
			});

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
			ProjectService.setSelectedPlugins($scope.selectedPlugins);
			ProjectService.updateProject($scope.project);
			UIService.updateProject($scope.project);
		};
	}
]);
