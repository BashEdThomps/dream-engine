App.controller("home",["$scope","ProjectService","UIService",
    function($scope,ProjectService,UIService) {
        UIService.setBreadcrumbs(["Home"]);
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

        $scope.onProjectAssetClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectAsset = true;
        };

        $scope.onProjectAssetModelClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectAssetModel = true;
        };

        $scope.onProjectAssetAnimationClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectAssetAnimation = true;
        };

        $scope.onProjectAssetAudioClicked = function() {
            $scope.hideAllHelp();
            $scope.showProjectAssetAudio = true;
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
            $scope.showProjectAsset          = false;
            $scope.showProjectAssetModel     = false;
            $scope.showProjectAssetAnimation = false;
            $scope.showProjectAssetAudio     = false;
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
                        label    : "Asset",
                        onSelect  : $scope.onProjectAssetClicked,
                        children : [
                            {
                                label:"Animation",
                                onSelect : $scope.onProjectAssetAnimationClicked
                            },
                            {
                                label:"Audio",
                                onSelect : $scope.onProjectAssetAudioClicked
                            },
                            {
                                label:"Model",
                                onSelect : $scope.onProjectAssetModelClicked
                            },
                            {
                                label:"Script",
                                onSelect : $scope.onProjectAssetScriptClicked
                            }
                        ]
                    }
                ],
            }
        ];
    }
]);
