App.controller("index",["$document","$scope","$uibModal",
function($document, $scope,$uibModal) {
    $scope.projectModified    = true;
    $scope.modalAnimationsEnabled = true;
    $scope.isFullScreen = false;

    $scope.breadcrumbs = [];
    $scope.project = {
        name: "Untitled Project",
        animation:{
            enabled: true,
            objects: [
                {
                    name: "An Animation",
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

    $scope.generateTreeData = function() {
        $scope.treeData = [];

        var treeDataRoot = {};
        treeDataRoot.label = $scope.project.name;

        if ($scope.project.animation.enabled) {
            treeDataRoot.animations = [];
            $scope.project.animation.objects.forEach(function(animation) {
                treeDataRoot.animations.push(
                    {
                        label: animation.name
                    }
                );
            });
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
        $scope.showCreateNewModal();
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
            controller: 'SaveModifiedModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveModal();
            } else {
                $scope.initialiseProject();
            }
        }, function () {
            console.log('showConfirmNewModal dismissed at: ' + new Date());
        });
    };

    $scope.showNewModal = function() {

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
                $scope.initialiseProject();
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
                $scope.initialiseProject();
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
    $scope.generateTreeData();
    $scope.generateBreadcrumbs();
}
]);
