App.controller("index",
["$state","$scope", "ApiService","ProjectService",
 "UIService", "$window",
function($state,$scope, ApiService, ProjectService,
     UIService, $window) {

    // Alerts ------------------------------------------------------------------

    $scope.closeAlert = function(index) {
        UIService.closeAlert(index);
    };

    // Tree Event Handlers -----------------------------------------------------

    $scope.onTreeProjectSelected = function(branch) {
        console.log("Selected Project:", branch);
        $state.go("Project");
    };

    $scope.onTreeProjectSceneSelected = function(branch) {
        console.log("Selected Scenes Parent Node:",branch);
        $state.go("ProjectSceneList");
    };

    $scope.onTreeProjectResourceSelected = function(branch) {
        console.log("Selected Resources Parent Node:",branch);
        $state.go("ProjectResourceList");
    };

    $scope.onTreeProjectSceneInstanceSelected = function(branch) {
        console.log("Selected Scene:",branch);
        $state.go("ProjectSceneEditor",{scene:branch.uuid});
    };

    $scope.onTreeProjectResourceInstanceSelected = function(branch) {
        console.log("Selected Resource:",branch);
        $state.go("ProjectResourceEditor",{resource:branch.uuid});
    };

    // Toolbar Button Callbacks ------------------------------------------------

    $scope.onToggleFullScreenButtonClicked = function() {
        UIService.isFullScreen = !UIService.isFullScreen;
    };

    $scope.onNewButtonClicked = function() {
        if (ProjectService.isModified()) {
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
        $scope.reloadUI();
    };

    $scope.onOpenButtonClicked = function() {
        if (ProjectService.isModified()) {
            UIService.showSaveModifiedModal(
                function yes() {
                    ProjectService.saveProject();
                    $scope.afterOpenConfirmed();
                },
                function no() {
                    $scope.afterOpenConfirmed();
                }
            );
        } else {
            $scope.afterOpenConfirmed();
        }
    };

    $scope.afterOpenConfirmed = function() {
        UIService.showOpenModal(function(openResult){
            if (openResult) {
                $scope.reloadUI();
                UIService.addAlert("Project Opened Successfuly!","success");
                $state.go("Project");
            }
        });
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

    $scope.reloadUI = function() {
        UIService.setHostController($scope);
        UIService.setBreadcrumbs([ProjectService.project.name]);
        UIService.generateTreeData();
        UIService.update();
    };

    $scope.reloadUI();
}
]);
