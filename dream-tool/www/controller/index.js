App.controller("index",["$document","$scope","$uibModal",
function($document, $scope,$uibModal) {
    $scope.projectFileModified    = true;
    $scope.modalAnimationsEnabled = true;
    $scope.isFullScreen = false;

    $scope.breadcrumbs = [];
    $scope.projectFile = {
        name: "Untitled Project",
        animations: [],
        collisionWorld: [],
        scenegraph: [],
    };

    $scope.generateTreeData = function() {
        $scope.treeData = [];

        var treeDataRoot = {};
        treeDataRoot.label = $scope.projectFile.name;

        if ($scope.projectFile.animations.length > 0) {
            $scope.treeDataRoot.animations = [];
            $scope.projectFile.animations.forEach(function(animation) {
                treeDataRoot.animations.push({label:animation.name});
            });
        }

        $scope.treeData.push(treeDataRoot);
    };

    $scope.generateBreadcrumbs = function (breadcrumbs) {
        $scope.breadcrumbs.push($scope.projectFile.name);
    };

    // Toolbar Button Callbacks ------------------------------------------------

    $scope.onNewButtonClicked = function() {
        if ($scope.isProjectFileModified()) {
            $scope.showSaveModifiedModal();
        }
        $scope.showCreateNewFileModal();
    };

    $scope.onOpenButtonClicked = function() {
        if ($scope.isProjectFileModified()) {
            $scope.showSaveModifiedModal();
        }
        $scope.showOpenFileModal();
    };

    $scope.onSaveButtonClicked = function() {
        $scope.showSaveFileModal();
    };

    $scope.isProjectFileModified = function() {
        return $scope.projectFileModified;
    };

    $scope.showSaveModifiedModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'view/modals/save_modified.html',
            controller: 'SaveModifiedModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveFileModal();
            } else {
                $scope.initialiseProjectFile();
            }
        }, function () {
            console.log('showConfirmNewFileModal dismissed at: ' + new Date());
        });
    };

    $scope.showNewFileModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'modals/confirm_new_file.html',
            controller: 'ConfirmNewFileModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveFileModal();
            } else {
                $scope.initialiseProjectFile();
            }
        }, function () {
            console.log('showConfirmNewFileModal dismissed at: ' + new Date());
        });
    };

    $scope.showOpenFileModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'modals/confirm_new_file.html',
            controller: 'ConfirmNewFileModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveFileModal();
            } else {
                $scope.initialiseProjectFile();
            }
        }, function () {
            console.log('showConfirmNewFileModal dismissed at: ' + new Date());
        });
    };

    $scope.showSaveFileModal = function() {
        var modal = $uibModal.open({
            animation: $scope.modalAnimationsEnabled,
            templateUrl: 'modals/confirm_new_file.html',
            controller: 'ConfirmNewFileModal'
        });

        modal.result.then(function (result) {
            if (result) {
                $scope.showSaveFileModal();
            } else {
                $scope.initialiseProjectFile();
            }
        }, function () {
            console.log('showConfirmNewFileModal dismissed at: ' + new Date());
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
