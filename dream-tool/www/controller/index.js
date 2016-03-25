App.controller("index",
    ["$document","$scope","$uibModal", "toastAlerts",
    function($document, $scope, $uibModal, toastAlerts) {
        $scope.projectModified = false;
        $scope.modalAnimationsEnabled = true;
        $scope.isFullScreen = false;
        $scope.breadcrumbs = [];
        $scope.alertList = [];

        $scope.initProject = function() {
            $scope.project = {
                name: "Untitled Project",
                animation:{
                    enabled: true,
                    objects: [
                        {
                            name: "Untitled Animation",
                            keyFrames: [
                                {
                                    index: 1,
                                    deltas: [

                                    ]
                                }
                            ]

                        }
                    ]
                },
                collisionWorld: {
                    enabled: true,
                    aabbMin: -100.0,
                    aabbMax: 100.0,
                    objects: []
                },
                scenegraph: {
                    enabled: true,
                    objects: []
                },
            };
            $scope.projectModified = false;
            $scope.addAlert("New Project Created","success");
            console.log("Project initialised.");

        };

        $scope.addAlert = function(message,type) {
            toastAlerts.addAlert($scope.alertList,toastAlerts.newAlert(message,type));
        };

        $scope.closeAlert = function(index) {
            toastAlerts.closeAlert($scope.alertList, index);
        };

        $scope.generateTreeData = function() {
            $scope.treeData = [];

            var treeDataRoot = {};
            treeDataRoot.label = $scope.project.name;

            if ($scope.project.animation.enabled) {
                treeDataRoot.children = [];
                
                // Collision World
                var collisionWorld = {label:"Collision World", children: []};
                treeDataRoot.children.push(collisionWorld);

                // Scenegraph
                var scenegraph = {label:"Scenegraph",children:[]};
                treeDataRoot.children.push(scenegraph);

                // Animations
                var animations = {label: "Animations", children: []};
                $scope.project.animation.objects.forEach(function(animation) {
                    console.log("Adding animation to tree",animation.name);
                    animations.children.push(
                        {
                            label: animation.name
                        }
                    );
                });
                treeDataRoot.children.push(animations);
            }

            $scope.treeData.push(treeDataRoot);
        };

        $scope.generateBreadcrumbs = function (breadcrumbs) {
            $scope.breadcrumbs.push($scope.project.name);
        };

        // Toolbar Button Callbacks ------------------------------------------------

        $scope.onNewButtonClicked = function() {
            if ($scope.isProjectModified()) {
                $scope.showSaveModifiedModal();
            }
            $scope.initProject();
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

        $scope.isProjectModified = function() {
            return $scope.projectModified;
        };

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
                    $scope.initProject();
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
                    $scope.initProject();
                }
            }, function () {
                console.log('showConfirmNewModal dismissed at: ' + new Date());
            });
        };

        $scope.showSaveModal = function() {
            var modal = $uibModal.open({
                animation: $scope.modalAnimationsEnabled,
                templateUrl: 'modals/confirm_new_file.html',
                controller: 'ConfirmNewModal'
            });

            modal.result.then(function (result) {
                if (result) {
                    $scope.showSaveModal();
                } else {
                    $scope.initProject();
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

        $scope.toggleFullScreen = function() {
            $scope.isFullScreen = !$scope.isFullScreen;
        };

        /* Load Functions */
        $scope.initProject();
        $scope.generateTreeData();
        $scope.generateBreadcrumbs();
    }
]);
