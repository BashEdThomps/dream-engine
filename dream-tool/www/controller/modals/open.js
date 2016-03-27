App.controller("OpenFileModal",
    ["$scope","$uibModalInstance","ProjectService",
    function($scope, $uibModalInstance,ProjectService) {

        $scope.openFile = null;

        $scope.open = function () {
            console.log("Loading:",$scope.openFile);
            var f = document.getElementById('file').files[0],
            r = new FileReader();
            r.onloadend = function(e){
                var data = e.target.result;
                //send you binary data via $http or $resource or do anything else with it
                console.log("Data?",data);
            };
            r.readAsArrayBuffer(f);
            //ProjectService.openFile($scope.openFile);
            $uibModalInstance.close(true);
        };

        $scope.cancel = function () {
            $uibModalInstance.close(false);
        };
    }
]);
