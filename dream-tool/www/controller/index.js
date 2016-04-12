App.controller("index",
    ["$state","$scope","ApiService","ProjectService","UIService","$window",
    function($state,$scope,ApiService,ProjectService,UIService,$window) {

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
        ApiService.createProjectStructure(ProjectService.getProjectUUID(),function(success) {
            if (success) {
                UIService.addAlert("New Project Created","success");
                $state.go("Project");
                $scope.reloadUI();
            } else {
                UIService.addAlert("Unable to create new project","danger");
                $state.go("Home");
                $scope.reloadUI();
            }
        });

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
        UIService.showOpenProjectModal(function(openResult){
            if (openResult) {
                $scope.reloadUI();
                UIService.addAlert("Project Opened Successfuly!","success");
                $state.go("Project");
            }
        });
    };

    $scope.onSaveButtonClicked = function() {
        var project = ProjectService.getProject();
        delete project.isModified;
        ApiService.saveProjectFile(project,function(success) {
            if (success) {
                UIService.addAlert(project.name+" saved with id: "+project.uuid,"success");
            } else {
                UIService.addAlert("Unable to save"+project.name+" saved with id: "+project.uuid,"danger");
            }
        });
    };

    $scope.onPlayButtonClicked = function() {
        ApiService.runDreamProject(ProjectService.project,function(success){
            if (success) {
                UIService.addAlert("Started " + ProjectService.project.name, "success");
            } else {
                UIService.addAlert("Failed to Start " + ProjectService.project.name, "danger");
            }
        });
    };

    $scope.onDownloadTarballButtonClicked = function() {
        ApiService.saveProjectTarball(ProjectService.project,function(success,resp){
            if (success) {
                console.log("Got Tarball Response",resp);
                var blob = new Blob([resp.data], { type: 'application/octet-binary' });
                var url = $window.URL || $window.webkitURL;
                var tarUrl = url.createObjectURL(blob);
                var downloadButton = angular.element(document.querySelector('#DownloadTarballButton'));
                downloadButton.attr("download",ProjectService.getProject().name+".tar.gz");
                $window.location = tarUrl;
                UIService.addAlert("Saved project "+ProjectService.getProjectUUID()+" as tarball.","success");
            } else {
                UIService.addAlert("Error: Unable to save project "+ProjectService.getProjectUUID()+" as tarball.","danger");
            }
        });
    };

    // onLoad Function Calls ---------------------------------------------------
    ProjectService.getProject();
    ApiService.createProjectDirectory(ProjectService.getProjectUUID(),function(success){
        if (success) {
            UIService.addAlert("Created new project "+ProjectService.getProjectUUID(),"success");
        } else {
            UIService.addAlert("Unable to create new project "+ProjectService.getProjectUUID(),"danger");
        }
    });

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
