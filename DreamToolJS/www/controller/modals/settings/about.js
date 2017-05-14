App.controller("SettingsAboutModal",["$scope","$uibModalInstance",
    function($scope, $uibModalInstance) {
        $scope.dismissModal = function() {
            $uibModalInstance.dismiss();
        };
    }
]);
