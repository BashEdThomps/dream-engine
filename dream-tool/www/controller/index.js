App.controller("index",
["$state","$scope", "ApiConnector","ProjectService",
 "UIService", "$window",
function($state,$scope, ApiConnector, ProjectService,
     UIService, $window) {

    // Help Pages --------------------------------------------------------------

    $scope.showHelpProject = true;

    $scope.onHelpProjectClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProject = true;
    };

    $scope.onHelpProjectConfigurationClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectConfiguration = true;
    };

    $scope.onHelpProjectSceneClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectScene = true;
    };

    $scope.onHelpProjectSceneScenegraphClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectSceneScenegraph = true;
    };

    $scope.onHelpProjectSceneBulletWorldClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectSceneBulletWorld = true;
    };

    $scope.onHelpProjectSceneSceneObjectClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectSceneSceneObject = true;
    };

    $scope.onHelpProjectSceneSceneObjectModifierClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectSceneSceneObjectModifier = true;
    };

    $scope.onHelpProjectSceneSceneObjectModifierEventListenerClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectSceneSceneObjectModifierEventListener = true;
    };

    $scope.onHelpProjectResourceClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResource = true;
    };

    $scope.onHelpProjectResourceModelClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResource = true;
    };

    $scope.onHelpProjectResourceAnimationClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResourceAnimation = true;
    };

    $scope.onHelpProjectResourceAnimationBezierClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResourceAnimationBezier = true;
    };

    $scope.onHelpProjectResourceAnimationLinearClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResourceAnimationLinear = true;
    };

    $scope.onHelpProjectResourceAnimationOrbitalClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResourceAnimationOrbital = true;
    };

    $scope.onHelpProjectResourceAudioClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResourceAudio = true;
    };

    $scope.onHelpProjectResourceAudioSoundEffectClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResourceAudioSoundEffect = true;
    };

    $scope.onHelpProjectResourceAudioMusicClicked = function() {
        $scope.hideAllHelp();
        $scope.showHelpProjectResourceAudioMusic = true;
    };
    $scope.hideAllHelp = function() {
        $scope.showHelpProject                 = false;

        $scope.showHelpProjectConfiguration    = false;

        $scope.showHelpProjectScene            = false;
        $scope.showHelpProjectSceneScenegraph  = false;
        $scope.showHelpProjectSceneBulletWorld = false;
        $scope.showHelpProjectSceneSceneObject = false;
        $scope.showHelpProjectSceneSceneObject = false;
        $scope.showHelpProjectSceneSceneObjectModifier = false;
        $scope.showHelpProjectSceneSceneObjectModifierEventListener = false;

        $scope.showHelpProjectResource          = false;

        $scope.showHelpProjectResourceModel     = false;

        $scope.showHelpProjectResourceAnimation = false;
        $scope.showHelpProjectResourceAnimationBezier = false;
        $scope.showHelpProjectResourceAnimationLinear = false;
        $scope.showHelpProjectResourceAnimationOrbital = false;

        $scope.showHelpProjectResourceAudio            = false;
        $scope.showHelpProjectResourceAudioSoundEffect = false;
        $scope.showHelpProjectResourceAudioMusic       = false;
    };

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


}
]);
