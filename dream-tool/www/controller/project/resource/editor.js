App.controller("ProjectResourceEditor",
    ["$scope","$state","$stateParams",
    function($scope,$state,$stateParams) {
        $scope.resourceName = $stateParams.resource;
        console.log("Editing Resource:",$scope.resourceName);
    }
]);
