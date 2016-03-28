App.controller("index",
["$state","$scope","$uibModal", "ToastAlerts","ApiConnector","ProjectService",
 "UIService", "$window",
function($state,$scope, $uibModal, ToastAlerts, ApiConnector, ProjectService,
         UIService, $window) {
    $scope.modalAnimationsEnabled = true;
    $scope.isFullScreen = false;
    $scope.alertList = [];
    $scope.treeData = [];
    $scope.isProjectModified = ProjectService.isModified;
    $scope.breadcrumbs  = UIService.breadcrumbs;

    // Alerts ------------------------------------------------------------------

    $scope.addAlert = function(message,type) {
        ToastAlerts.addAlert($scope.alertList,ToastAlerts.newAlert(message,type));
    };

    $scope.closeAlert = function(index) {
        ToastAlerts.closeAlert($scope.alertList,index);
    };

    // Project Tree ------------------------------------------------------------

    $scope.generateTreeData = function() {
        console.log("Generating Tree Data");
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
        ProjectService.getProject().resources.forEach(function(resource){
            resourcesNode.children.push({
                label : resource.name,
                onSelect: $scope.onTreeProjectResourceInstanceSelected
            });
        });
        treeDataRoot.children.push(resourcesNode);
    };

    // Tree Event Handlers -----------------------------------------------------

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

    $scope.onTreeProjectResourceInstanceSelected = function(branch) {
        console.log("Selected Resource:",branch);
        $state.go("ProjectResourceEditor",{resource:branch.label});
    };

    // Toolbar Button Callbacks ------------------------------------------------

    $scope.onToggleFullScreenButtonClicked = function() {
        $scope.isFullScreen = !$scope.isFullScreen;
    };

    $scope.onNewButtonClicked = function() {
        if (ProjectService.isModified() === true) {
            $scope.showSaveModifiedModal(
                function yes() {
                    ProjectService.saveProject();
                }, function no() {
                    $scope.newProjectAction();
                }
            );
        } else {
            $scope.newProjectAction();
        }

    };

    $scope.newProjectAction = function() {
        ProjectService.initialise();
        $scope.addAlert("New Project Created","success");
        $state.go("Project");
        //$scope.updateUI();
    };

    $scope.onOpenButtonClicked = function() {
        if (ProjectService.isModified() === true) {
            $scope.showSaveModifiedModal(
                function yes() {
                    ProjectService.saveProject();
                    $scope.showOpenModal();
                },
                function no() {
                    $scope.showOpenModal();
                }
            );
        } else {
            $scope.showOpenModal();
        }
    };

    $scope.onSaveButtonClicked = function() {
        ProjectService.saveProject();
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

    // Modals ------------------------------------------------------------------

    $scope.showSaveModifiedModal = function(yesCallback,noCallback) {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'view/partials/modals/save_modified.html',
            controller: 'YesNoModal'
        });

        modal.result.then(function (result) {
            if (result) {
                yesCallback();
            } else {
                noCallback();
            }
        }, function () {
            noCallback();
        });
    };

    $scope.showOpenModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'view/partials/modals/open.html',
            controller: 'OpenFileModal'
        });

        modal.result.then(function (result) {
            if (result) {

            } else {

            }
        }, function () {

        });
    };

    // onLoad Function Calls ---------------------------------------------------

    ProjectService.getProject();

    $scope.projectModified = function() {
        ProjectService.setProjectModified(true);
    };

    $scope.updateUI = function() {
        $scope.generateTreeData();
        UIService.setBreadcrumbs([ProjectService.project.name]);
    };

    $scope.updateUI();
}
]);
