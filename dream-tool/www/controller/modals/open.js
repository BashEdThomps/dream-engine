App.controller("OpenFileModal",
    ["$scope","$uibModalInstance","ProjectService",
    function($scope, $uibModalInstance,ProjectService) {

        $scope.open = function () {
            var f = document.getElementById('file').files[0],
            r = new FileReader();
            r.onloadend = function(e){
                var data = e.target.result;
                console.log("Data?",data);
                ProjectService.openProject(JSON.parse(data));
                $uibModalInstance.close(true);
            };
            r.readAsText(f);
        };

        $scope.cancel = function () {
            $uibModalInstance.close(false);
        };
    }
]);
