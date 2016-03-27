App.controller("index",
["$state","$scope","$uibModal", "ToastAlerts","ApiConnector","ProjectService",
 "UIService",
function($state,$scope, $uibModal, ToastAlerts, ApiConnector, ProjectService,
         UIService) {
    $scope.modalAnimationsEnabled = true;
    $scope.isFullScreen = false;
    $scope.breadcrumbs = [];
    $scope.alertList   = [];
    $scope.treeData  = {};

    // Alerts --------------------------------------------------------------

    $scope.addAlert = function(message,type) {
        ToastAlerts.addAlert($scope.alertList,ToastAlerts.newAlert(message,type));
    };

    $scope.closeAlert = function(index) {
        ToastAlerts.closeAlert($scope.alertList, index);
    };

    // Project Tree --------------------------------------------------------

    $scope.generateTreeData = function() {
        $scope.treeData = [];

        var treeDataRoot = {};
        treeDataRoot.label = ProjectService.project.name;
        treeDataRoot.children = [];
        treeDataRoot.onSelect = $scope.onTreeProjectNodeSelected;

        $scope.setupTreeScenes(treeDataRoot);
        $scope.setupTreeResources(treeDataRoot);

        $scope.treeData.push(treeDataRoot);
    };

    $scope.setupTreeScenes = function(treeDataRoot) {
        var scenes = {
            label:"Scenes",
            children:[],
            onSelect: $scope.onTreeSceneNodeSelected
        };

        ProjectService.project.scenes.forEach(function(scene) {
            console.log("Adding scene to tree:", scene.name);

            var sceneChild = {
                label    : scene.name,
                children: [],
                onSelect : $scope.onTreeSceneInstanceSelected,
            };

            $scope.setupTreeSceneAnimations(scene, sceneChild);
            scenes.children.push(sceneChild);
        });

        treeDataRoot.children.push(scenes);
    };

    $scope.setupTreeResources = function(treeDataRoot) {
        var resourcesNode = {
            label: "Resources",
            children: [],
            onSelect: $scope.onTreeResourcesNodeSelected
        };
        treeDataRoot.children.push(resourcesNode);
    };

    $scope.setupTreeSceneAnimations = function(scene,sceneChild) {
        var animations = {
            label: "Animations",
            children: []
        };

        scene.animations.forEach(function(animation) {
            console.log("Adding Animation to Tree:", animation.name);
            animations.children.push( {
                label: animation.name,
                onSelect: $scope.onTreeAnimaionInstanceSelected,
            });
        });

        sceneChild.children.push(animations);
    };

    $scope.onTreeProjectNodeSelected = function(branch) {
        console.log("Selected Project:", branch);
        $state.go("Project");
    };

    $scope.onTreeSceneNodeSelected = function(branch) {
        console.log("Selected Scenes Parent Node:",branch);
        $state.go("ProjectSceneList");
    };

    $scope.onTreeResourcesNodeSelected = function(branch) {
        console.log("Selected Resources Parent Node:",branch);
        $state.go("ProjectResourceList");
    };

    $scope.onTreeSceneInstanceSelected = function(branch) {
        console.log("Selected Scene:",branch);
        $state.go("ProjectSceneEditor",{scene:branch.label});
    };

    $scope.onTreeAnimationInstanceSelected = function(branch) {
        console.log("Selected Animation:",branch);
    };
    // Toolbar Button Callbacks --------------------------------------------

    $scope.toggleFullScreen = function() {
        $scope.isFullScreen = !$scope.isFullScreen;
    };

    $scope.onNewButtonClicked = function() {
        if ($scope.isProjectModified()) {
            $scope.showSaveModifiedModal();
        }
        ProjectService.initialise();
    };

    $scope.onOpenButtonClicked = function() {
        if ($scope.isProjectModified()) {
            $scope.showSaveModifiedModal();
        }
        $scope.showOpenModal();
    };

    $scope.onSaveButtonClicked = function() {
        $scope.showSaveModal();
    };

    $scope.onPlayButtonClicked = function() {
        ApiConnector.runDreamProject(ProjectService.project,function(success){
            if (success) {
                $scope.addAlert("Started " + ProjectService.project.name, "success");
            } else {
                $scope.addAlert("Failed to Start " + ProjectService.project.name, "danger");
            }
        });
    };

    $scope.isProjectModified = function() {
        return ProjectService.project.isModified;
    };

    // Modals --------------------------------------------------------------

    $scope.showSaveModifiedModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'view/partials/modals/save_modified.html',
            controller: 'YesNoModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveModal();
            } else {
                ProjectService.initialise();
            }
        }, function () {
            console.log('showConfirmNewModal dismissed at: ' + new Date());
        });
    };

    $scope.showOpenModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'modals/confirm_new_file.html',
            controller: 'ConfirmNewModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveModal();
            } else {
                ProjectService.initialise();
            }
        }, function () {
            console.log('showConfirmNewModal dismissed at: ' + new Date());
        });
    };

    $scope.showSaveModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'view/partials/modals/save.html',
            controller: 'SaveModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveModal();
            } else {
                ProjectService.initialise();
            }
        }, function () {
            console.log('showConfirmNewModal dismissed at: ' + new Date());
        });
    };

    $scope.showYesNoModal = function(template, controller,yes,no){
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: template,
            controller: controller
        });

        modal.result.then(function (result) {
            if (result) {
                yes();
            } else {
                no();
            }
        });
    };

    // Breadcrumbs -------------------------------------------------------------
    $scope.getBreadcrumbs = function() {
        return UIService.getBreadcrumbs();
    };
    
    // onLoad Function Calls -----------------------------------------------

    ProjectService.getProject();

    $scope.updateUI = function() {
        $scope.generateTreeData();
        UIService.setBreadcrumbs([ProjectService.project.name]);
        $scope.breadcrumbs = UIService.getBreadcrumbs();
    };
    $scope.updateUI();
}
]);
