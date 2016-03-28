App.controller("index",
["$state","$scope", "ApiConnector","ProjectService",
 "UIService", "$window",
function($state,$scope, ApiConnector, ProjectService,
     UIService, $window) {

    // Alerts ------------------------------------------------------------------

    $scope.closeAlert = UIService.closeAlert;

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
        UIService.isFullScreen = !UIService.isFullScreen;
    };

    $scope.onNewButtonClicked = function() {
        if (ProjectService.isModified() === true) {
            UIService.showSaveModifiedModal(
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
        UIService.addAlert("New Project Created","success");
        $state.go("Project");
    };

    $scope.onOpenButtonClicked = function() {
        if (ProjectService.isModified() === true) {
            UIService.showSaveModifiedModal(
                function yes() {
                    ProjectService.saveProject();
                    UIService.showOpenModal();
                },
                function no() {
                    UIService.showOpenModal();
                }
            );
        } else {
            UIService.showOpenModal();
        }
    };

    $scope.onSaveButtonClicked = function() {
        ProjectService.saveProject();
    };

    $scope.onPlayButtonClicked = function() {
        ApiConnector.runDreamProject(ProjectService.project,function(success){
            if (success) {
                UIService.addAlert("Started " + ProjectService.project.name, "success");
            } else {
                UIService.addAlert("Failed to Start " + ProjectService.project.name, "danger");
            }
        });
    };

        // onLoad Function Calls ---------------------------------------------------

    ProjectService.getProject();

    $scope.projectModified = function() {
        ProjectService.setProjectModified(true);
    };

    UIService.setHostController($scope);
    UIService.setBreadcrumbs([ProjectService.project.name]);
    UIService.update();
    $scope.alertList = UIService.toastAlertList;
    $scope.treeData = UIService.treeData;
    $scope.isProjectModified = ProjectService.isModified;
    $scope.breadcrumbs  = UIService.breadcrumbs;

}
]);
