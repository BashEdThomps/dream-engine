App.controller("ProjectSceneEditor",
    ["$scope","$state","$stateParams","ProjectService","UIService",
    function($scope,$state,$stateParams,ProjectService,UIService) {
        $scope.sceneUUID           = $stateParams.scene;
        $scope.scenegraphTree      = [];
        $scope.selectedSceneObject = null;
        $scope.resourceList        = null;

        $scope.initScenegraphTree = function() {
            $scope.scenegraphTree.push({
                label    : $scope.scene.name,
                onSelect :  $scope.onScenegraphTreeSelection,
                children : [],
            });
            $scope.scene.objects.forEach(function(sceneObject){
                $scope.scenegraphTree[0].children.push({
                    label:sceneObject.name,
                    uuid:sceneObject.uuid,
                    onSelect: $scope.onScenegraphTreeSelection,
                    children: [],
                });
            });
        };

        $scope.onScenegraphTreeSelection = function(branch) {
            $scope.selectedSceneObject = branch.uuid;
        };

        $scope.projectModified = function () {
            ProjectService.setProjectModified(true);
            UIService.update();
        };

        $scope.onAddResourceLinkButtonClicked = function() {
            ProjectService.addResourceLinkToSceneObject(
                $scope.sceneUUID,
                $scope.selectedSceneObject,
                $scope.selectedNewResourceLink
            );
        };

        if (ProjectService.isProjectOpen()) {
            ProjectService.getSceneByUUID($scope.sceneUUID,function (scene){
                if (scene === null) {
                    UIService.addAlert("Error: Unable to find scene: "+$scope.sceneUUID,"danger");
                } else {
                    $scope.scene = scene;
                    UIService.setBreadcrumbs([ProjectService.getName(),"Scenes",$scope.scene.name]);
                    UIService.update();
                    $scope.initScenegraphTree();
                }
            });
            ProjectService.getResourceList(function(list){
                $scope.resourceList = list;
            });
        } else {
            $state.go("Home");
        }
    }
]);
