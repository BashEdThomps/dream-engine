App.controller("OpenFileModal",
    ["$scope","$uibModalInstance","ProjectService","UtilService","UIService",
    function($scope, $uibModalInstance,ProjectService,UtilService,UIService) {

        $scope.open = function () {
            var fileElement = document.getElementById('file');

            UtilService.readFileAsTextFromElement(fileElement,function(data){
                if (data !== null) {
                    ProjectService.openProject(JSON.parse(data));
                    $uibModalInstance.close(true);
                } else {
                    UIService.addalert("Unable to load file!","danger");
                }
            });
        };

        $scope.cancel = function () {
            $uibModalInstance.close(false);
        };
    }
]);
