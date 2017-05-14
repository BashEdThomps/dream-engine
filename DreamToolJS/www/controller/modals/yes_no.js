App.controller("YesNoModal",["$scope","$uibModalInstance",
    function($scope, $uibModalInstance) {

        $scope.yes = function () {
            $uibModalInstance.close(true);
        };

        $scope.no = function () {
            $uibModalInstance.close(false);
        };
    }
]);
