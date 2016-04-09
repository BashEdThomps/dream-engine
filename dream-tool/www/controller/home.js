App.controller("home",["$scope","ProjectService","UIService",
    function($scope,ProjectService,UIService) {
        UIService.setBreadcrumbs([ProjectService.getName(),"Home"]);
        // Help Pages --------------------------------------------------------------

        $scope.showProject = true;

        $scope.onProjectClicked = function() {
            $scope.hideAllHelp();
            $scope.showProject = true;
        };

        $scope.onProjectConfigurationSelected = function() {
            $scope.hideAllHelp();
            $scope.showProjectConfiguration = true;
        };

        $scope.onProjectSceneClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectScene = true;
        };

        $scope.onProjectSceneScenegraphClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectSceneScenegraph = true;
        };

        $scope.onProjectSceneBulletWorldClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectSceneBulletWorld = true;
        };

        $scope.onProjectSceneScenegraphSceneObjectClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectSceneScenegraphSceneObject = true;
        };

        $scope.onProjectSceneScenegraphSceneObjectModifierClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectSceneScenegraphSceneObjectModifier = true;
        };

        $scope.onProjectSceneScenegraphSceneObjectModifierEventListenerClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectSceneScenegraphSceneObjectModifierEventListener = true;
        };

        $scope.onProjectResourceClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResource = true;
        };

        $scope.onProjectResourceModelClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResource = true;
        };

        $scope.onProjectResourceAnimationSelected = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAnimation = true;
        };

        $scope.onProjectResourceAnimationBezierClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAnimationBezier = true;
        };

        $scope.onProjectResourceAnimationLinearClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAnimationLinear = true;
        };

        $scope.onProjectResourceAnimationOrbitalClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAnimationOrbital = true;
        };

        $scope.onProjectResourceAudioClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAudio = true;
        };

        $scope.onProjectResourceAudioSoundEffectClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAudioSoundEffect = true;
        };

        $scope.onProjectResourceAudioMusicClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAudioMusic = true;
        };

        $scope.hideAllHelp = function() {
            $scope.showProject                 = false;
            $scope.showProjectConfiguration    = false;
            $scope.showProjectScene            = false;
            $scope.showProjectSceneScenegraph  = false;
            $scope.showProjectSceneBulletWorld = false;
            $scope.showProjectSceneScenegraphSceneObject = false;
            $scope.showProjectSceneScenegraphSceneObject = false;
            $scope.showProjectSceneScenegraphSceneObjectModifier = false;
            $scope.showProjectSceneScenegraphSceneObjectModifierEventListener = false;
            $scope.showProjectResource          = false;
            $scope.showProjectResourceModel     = false;
            $scope.showProjectResourceAnimation = false;
            $scope.showProjectResourceAnimationBezier = false;
            $scope.showProjectResourceAnimationLinear = false;
            $scope.showProjectResourceAnimationOrbital = false;
            $scope.showProjectResourceAudio            = false;
            $scope.showProjectResourceAudioSoundEffect = false;
            $scope.showProjectResourceAudioMusic       = false;
        };

        // Tree Data

        $scope.helpTreeData = [
            {
                label    : "Project",
                onSelect  : $scope.onProjectClicked,
                children : [
                    {
                        label    : "Scene",
                        onSelect  : $scope.onProjectSceneClicked,
                        children : [
                            {
                                label    : "Scenegraph",
                                onSelect  : $scope.onProjectSceneScenegraphClicked,
                                children : [
                                    {
                                        label    : "Scene Object",
                                        onSelect  : $scope.onProjectSceneScenegraphSceneObjectClicked,
                                        children : []
                                    }
                                ]
                            },
                            {
                                label: "Bullet World",
                                onSelect: $scope.onProjectSceneBulletWorldClicked,
                                children: []
                            }
                        ]
                    },
                    {
                        label    : "Resource",
                        onSelect  : $scope.onProjectResourceClicked,
                        children : []
                    }
                ],
            }
        ];
    }
]);
