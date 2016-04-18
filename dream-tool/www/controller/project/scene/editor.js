App.controller("ProjectSceneEditor",
    ["$scope","$state","$stateParams","ProjectService","UIService",
    function($scope,$state,$stateParams,ProjectService,UIService) {
        $scope.sceneUUID               = $stateParams.scene;
        $scope.scenegraphTree          = [];
        $scope.selectedSceneObject     = null;
        $scope.selectedSceneObjectUUID = null;
        $scope.resourceList            = null;

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

        $scope.goToResourceEditor = function(uuid) {
            $state.go("ProjectResourceEditor",{resource: uuid});
        };

        $scope.getResourceByUUID = function(uuid) {
            var retval = null;
            ProjectService.getResourceByUUID(uuid,function(rsc)
            {
                retval = rsc;
            });
            return retval;
        };

        $scope.onScenegraphTreeSelection = function(branch) {
            $scope.selectedSceneObjectUUID = branch.uuid;
            ProjectService.getSceneObjectByUUID($scope.scene,$scope.selectedSceneObjectUUID,function(so) {
                $scope.selectedSceneObject = so;
            });
        };

        $scope.modified = function () {
            console.log("Scene Modified");
            ProjectService.updateScene($scope.scene);
            UIService.updateScene($scope.scene);
        };

        $scope.onAddResourceLinkButtonClicked = function() {
            ProjectService.addResourceLinkToSceneObject(
                $scope.sceneUUID,
                $scope.selectedSceneObjectUUID,
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
