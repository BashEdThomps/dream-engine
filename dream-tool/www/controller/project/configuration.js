App.controller("ProjectConfiguration",[
	"$scope","ProjectService",
	function($scope,ProjectService) {
		$scope.project = ProjectService.project;

		$scope.onRemoveResourceButtonClicked = function(resourceName) {
			console.log("Removing Resource:",resourceName);
		};
	}
]);
