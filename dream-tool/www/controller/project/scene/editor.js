App.controller("ProjectSceneEditor",
["$scope","$state","$stateParams","ProjectService","UIService",
  function($scope,$state,$stateParams,ProjectService,UIService) {
    $scope.sceneUUID               = $stateParams.scene;
    $scope.scenegraphTree          = [];
    $scope.selectedSceneObject     = null;
    $scope.selectedSceneObjectUUID = null;
    $scope.resourceList            = null;

    $scope.initScenegraphTree = function() {
      $scope.scenegraphTree = [];
      $scope.scenegraphTree.push({
        label    : $scope.scene.name,
        onSelect :  $scope.onScenegraphTreeSelection,
        children : [],
      });
      $scope.scene.objects.forEach(function(sceneObject){
        $scope.scenegraphTree[0].children.push(
          $scope.generateSceneObjectTreeNode(sceneObject)
        );
      });
    };

    $scope.generateSceneObjectTreeNode = function(sceneObject) {
      var retval = {
        label: sceneObject.name,
        uuid: sceneObject.uuid,
        onSelect: $scope.onScenegraphTreeSelection,
        children: [],
      };
      if (sceneObject.children !== undefined) {
        sceneObject.children.forEach(function(child){
          retval.children.push($scope.generateSceneObjectTreeNode(child));
        });
      }
      return retval;
    };

    $scope.goToResourceEditor = function(uuid) {
      $state.go("ProjectResourceEditor",{resource: uuid});
    };

    $scope.goToSceneObjectEditor = function(so) {
      $scope.selectedSceneObject = so;
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

    $scope.onAddResourceInstanceButtonClicked = function() {
      ProjectService.addResourceInstanceToSceneObject(
        $scope.sceneUUID,
        $scope.selectedSceneObjectUUID,
        $scope.selectedNewResourceInstance
      );
    };

    $scope.onRemoveResourceInstanceButtonClicked = function(instance) {
      ProjectService.removeResourceInstanceFromSceneObject($scope.sceneUUID,$scope.selectedSceneObjectUUID,instance);
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

    $scope.onAddChildButtonClicked = function() {
      if ($scope.selectedSceneObject !== null) {
        var child = ProjectService.createSceneObject();
        //child.parent = $scope.selectedSceneObjectUUID;
        if ($scope.selectedSceneObject.children === undefined) {
          $scope.selectedSceneObject.children = [];
        }
        $scope.selectedSceneObject.children.push(child);
        $scope.initScenegraphTree();
      }
    };
  }
]);
