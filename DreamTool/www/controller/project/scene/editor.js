App.controller("ProjectSceneEditor",
["$scope","$state","$stateParams","ProjectService","UIService",
  function($scope,$state,$stateParams,ProjectService,UIService) {
    $scope.sceneUUID               = $stateParams.scene;
    $scope.scenegraphTree          = [];
    $scope.selectedSceneObject     = null;
    $scope.selectedSceneObjectUUID = null;
    $scope.assetList            = null;

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

    $scope.goToAssetEditor = function(uuid) {
      $state.go("ProjectAssetEditor",{asset: uuid});
    };

    $scope.goToSceneObjectEditor = function(so) {
      $scope.selectedSceneObject = so;
    };

    $scope.getAssetByUUID = function(uuid) {
      var retval = null;
      ProjectService.getAssetByUUID(uuid,function(rsc)
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

    $scope.onAddAssetInstanceButtonClicked = function() {
      ProjectService.addAssetInstanceToSceneObject(
        $scope.sceneUUID,
        $scope.selectedSceneObjectUUID,
        $scope.selectedNewAssetInstance,
        function(success) {
          if (!success) {
            UIService.addAlert("SceneObject all ready contains an instance to this Asset","danger");
          }
        }
      );
    };

    $scope.onRemoveAssetInstanceButtonClicked = function(instance) {
      ProjectService.removeAssetInstanceFromSceneObject($scope.sceneUUID,$scope.selectedSceneObjectUUID,instance);
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
      ProjectService.getAssetList(function(list){
        $scope.assetList = list;
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

    $scope.onRemoveChildButtonClicked = function(child) {
      if ($scope.selectedSceneObject !== null) {
        var childIndex = $scope.selectedSceneObject.children.indexOf(child);
        $scope.selectedSceneObject.children.splice(childIndex,1);
        $scope.initScenegraphTree();
      }
    };
  }
]);
