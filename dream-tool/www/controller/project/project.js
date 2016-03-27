App.controller("Project",[
	"$scope","$stateParams",
	function($scope,$stateParams) {
		$scope.project = $stateParams.project;
	}
]);
