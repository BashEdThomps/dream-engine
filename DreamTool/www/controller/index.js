App.controller("index",
    ["$state","$scope","ApiService","ProjectService","UIService","$window","CookieService","cssInjector",
    function($state,$scope,ApiService,ProjectService,UIService,$window,CookieService,cssInjector) {

    $scope.findUiElements = function() {
      setTimeout(function() {
        $scope.uiBody = document.getElementsByTagName('body')[0];
        console.log("Found Body:",$scope.uiBody);

        $scope.uiNavbar = document.getElementById("index-nav");
        console.log("Found Navbar:",$scope.uiNavbar);

        $scope.uiContainer = document.getElementById("index-content");
        console.log("Found Container:",$scope.uiContainer);

        var foundRequiredUIElements = $scope.uiBody && $scope.uiNavbar && $scope.uiContainer;

        if (foundRequiredUIElements) {
          var uiBodyHeight      = $scope.uiBody.style.height;
          var uiNavbarHeight    = $scope.uiNavbar.clientHeight;
          var uiContainerHeight = uiBodyHeight - uiNavbarHeight;

          console.log("uiBodyHeight",uiBodyHeight);
          console.log("uiNavbarHeight",uiNavbarHeight);
          console.log("uiContainerHeight",uiContainerHeight);

          $scope.uiContainer.style.height = uiContainerHeight;
        }
      },1000);
    };

    // Alerts ------------------------------------------------------------------

    $scope.closeAlert = function(index) {
        UIService.closeAlert(index);
    };

    // Tree Event Handlers -----------------------------------------------------

    $scope.onTreeProjectSelected = function(branch) {
        console.log("Selected Project:", branch);
        if (ProjectService.isProjectOpen()) {
            $state.go("Project");
        } else {
            UIService.addAlert("Please Create or Open a Project","danger");
        }
    };

    $scope.onTreeProjectSceneSelected = function(branch) {
        console.log("Selected Scenes Parent Node:",branch);
        if (ProjectService.isProjectOpen()) {
            $state.go("ProjectSceneList");
        } else {
            UIService.addAlert("Please Create or Open a Project","danger");
        }
    };

    $scope.onTreeProjectAssetSelected = function(branch) {
        console.log("Selected Assets Parent Node:",branch);
        if (ProjectService.isProjectOpen()) {
            $state.go("ProjectAssetList");
        } else {
            UIService.addAlert("Please Create or Open a Project","danger");
        }
    };

    $scope.onTreeProjectSceneInstanceSelected = function(branch) {
        console.log("Selected Scene:",branch);
        if (ProjectService.isProjectOpen()) {
            $state.go("ProjectSceneEditor",{scene:branch.uuid});
        } else {
            UIService.addAlert("Please Create or Open a Project","danger");
        }
    };

    $scope.onTreeProjectAssetInstanceSelected = function(branch) {
        console.log("Selected Asset:",branch);
        if (ProjectService.isProjectOpen()) {
            $state.go("ProjectAssetEditor",{asset:branch.uuid});
        } else {
            UIService.addAlert("Please Create or Open a Project","danger");
        }
    };

    // Toolbar Button Callbacks ------------------------------------------------

    $scope.onNewButtonClicked = function() {
        if (ProjectService.isModified()) {
            UIService.showSaveModifiedModal(
                function yes() {
                    $scope.saveProject();
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
        UIService.showOpenProjectModal(function(openResult){
            if (openResult) {
                $scope.reloadUI();
                UIService.addAlert("Project Opened Successfuly!","success");
                $state.go("Project");
            } else {
                UIService.addAlert("Failed to Open "+ProjectService.getName(),"danger");
            }
        });
    };

    $scope.onUploadButtonClicked = function() {
        UIService.addAlert("Not Available","warning");
    };

    $scope.saveProject = function() {
        if (!ProjectService.isProjectOpen()) {
            UIService.addAlert("No project open to save!","danger");
            return;
        }

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

    $scope.onSaveButtonClicked = function() {
        $scope.saveProject();
    };

    $scope.onPlayButtonClicked = function() {
        if (ProjectService.isProjectOpen()) {
            ApiService.execDream(CookieService.getDreamExecPath(),ProjectService.project.uuid,function(success){
                if (success) {
                    UIService.addAlert("Started " + ProjectService.project.name, "success");
                } else {
                    UIService.addAlert("Failed to Start " + ProjectService.project.name, "danger");
                }
            });
        } else {
            UIService.addAlert("No project open to run","danger");
        }
    };

    $scope.onDownloadTarballButtonClicked = function() {
        //UIService.addAlert("Not Available","warning");
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

    $scope.onDeleteButtonClicked = function() {
        UIService.addAlert("Not Available","warning");
    };

    $scope.onCloseProjectButtonClicked = function() {
        ProjectService.closeProject();
        $scope.reloadUI();
        $state.go("Home");
        UIService.addAlert("Project Closed","success");
    };

    $scope.onSettingsClicked = function() {
        UIService.showSettingsModal();
    };

    $scope.onSettingsHelpClicked = function() {
        console.log("Showing Help");
        $state.go("Home");
    };

    $scope.onSettingsAboutClicked = function() {
        UIService.showSettingsAboutModal();
    };

    // Helpers -----------------------------------------------------------------

    $scope.projectModified = function() {
        ProjectService.setProjectModified(true);
    };

    $scope.reloadUI = function() {
      $scope.findUiElements();
      UIService.setHostController($scope);
      if(ProjectService.getProject()) {
        UIService.setBreadcrumbs([ProjectService.project.name]);
        UIService.generateTreeData();
      } else {
        UIService.setBreadcrumbs(["Home"]);
      }
      UIService.update();
    };

    $scope.reloadUI();

    cssInjector.add("css/"+( CookieService.getTheme() || "bootstrap" )+".css");
}
]);
