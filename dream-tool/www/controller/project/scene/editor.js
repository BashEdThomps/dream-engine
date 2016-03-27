App.controller("ProjectSceneEditor",
    ["$scope","$state","$stateParams",
    function($scope,$state,$stateParams) {
        $scope.sceneName = $stateParams.scene;
        console.log("Editing Scene:",$scope.sceneName);
    }
]);
