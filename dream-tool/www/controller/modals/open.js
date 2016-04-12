App.controller("OpenProjectModal",
    ["$scope","$uibModalInstance","ProjectService","UtilService","UIService","ApiService",
    function($scope, $uibModalInstance,ProjectService,UtilService,UIService,ApiService) {

        $scope.readProjectList = function() {
            ApiService.readProjectList(function(list){
                list.forEach(function(proj) {
                    $scope.projectTreeData.push({
                        label: proj.toUpperCase(),
                        onSelect: $scope.projectSelected
                    });
                });
            });
        };

        $scope.projectSelected = function(selected) {
            $scope.selectedForOpen = selected;
            console.log("Selected",selected,"for open");
        };

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

        $scope.projectTreeData = [];
        $scope.readProjectList();
    }
]);
