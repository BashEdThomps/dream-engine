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
            $scope.showProjectResourceModel = true;
        };

        $scope.onProjectResourceAnimationClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAnimation = true;
        };

        $scope.onProjectResourceAudioClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectResourceAudio = true;
        };

        $scope.hideAllHelp = function() {
            $scope.showProject                 = false;
            $scope.showProjectConfiguration    = false;
            $scope.showProjectScene            = false;
            $scope.showProjectSceneScenegraph  = false;
            $scope.showProjectSceneBulletWorld = false;
            $scope.showProjectSceneScenegraphSceneObject = false;
            $scope.showProjectSceneScenegraphSceneObjectModifier = false;
            $scope.showProjectSceneScenegraphSceneObjectEventListener = false;
            $scope.showProjectResource          = false;
            $scope.showProjectResourceModel     = false;
            $scope.showProjectResourceAnimation = false;
            $scope.showProjectResourceAudio     = false;
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
                        children : [
                            {
                                label:"Animation",
                                onSelect : $scope.onProjectResourceAnimationClicked
                            },
                            {
                                label:"Audio",
                                onSelect : $scope.onProjectResourceAudioClicked
                            },
                            {
                                label:"Model",
                                onSelect : $scope.onProjectResourceModelClicked
                            }
                        ]
                    }
                ],
            }
        ];
    }
]);
