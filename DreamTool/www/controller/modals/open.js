App.controller("OpenProjectModal",
    ["$scope","$uibModalInstance","ProjectService","UtilService","UIService","ApiService",
    function($scope, $uibModalInstance,ProjectService,UtilService,UIService,ApiService) {

        $scope.selectedForOpen = null;

        $scope.readProjectList = function() {
            ApiService.readProjectList(function(list){
                list.forEach(function(proj) {
                    $scope.projectTreeData.push({
                        label: proj.name + " ("+proj.uuid.toUpperCase()+")",
                        uuid : proj.uuid,
                        onSelect: $scope.projectSelected
                    });
                });
            });
        };

        $scope.projectSelected = function(selected) {
            $scope.selectedForOpen = selected.uuid;
            console.log("Selected",selected,"for open");
        };

        $scope.cancel = function () {
            $uibModalInstance.dismiss();
        };

        $scope.open = function() {
            ProjectService.openProject($scope.selectedForOpen,function(success){
                if (success) {
                    $uibModalInstance.close(true);
                } else {
                    $uibModalInstance.close(false);
                }
            });
        };

        console.log("open state activated");
        $scope.projectTreeData = [];
        $scope.readProjectList();
    }
]);
