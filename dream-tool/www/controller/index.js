App.controller("index",
    ["$state","$scope","$uibModal", "ToastAlerts","ApiConnector","ProjectService",
    function($state,$scope, $uibModal, ToastAlerts, ApiConnector,ProjectService) {
        $scope.modalAnimationsEnabled = true;
        $scope.isFullScreen = false;
        $scope.breadcrumbs = [];
        $scope.alertList = [];

		// Alerts --------------------------------------------------------------

        $scope.addAlert = function(message,type) {
            ToastAlerts.addAlert($scope.alertList,ToastAlerts.newAlert(message,type));
        };

        $scope.closeAlert = function(index) {
            ToastAlerts.closeAlert($scope.alertList, index);
        };

		// Project Tree --------------------------------------------------------

        $scope.generateTreeData = function() {
            $scope.treeData = [];

            var treeDataRoot = {};
            treeDataRoot.label = ProjectService.project.name;
            treeDataRoot.children = [];
						treeDataRoot.onSelect = $scope.onTreeProjectInstanceSelected;

            $scope.setupTreeConfiguration(treeDataRoot);
						$scope.setupTreeScenes(treeDataRoot);

            $scope.treeData.push(treeDataRoot);
        };

		$scope.setupTreeScenes = function(treeDataRoot) {
          var scenes = {
						label:"Scenes",
						children:[],
						onSelect: $scope.onTreeSceneListSelected
					};

					ProjectService.project.scenes.forEach(function(scene) {
						console.log("Adding scene to tree:", scene.name);

						var sceneChild = {
							label    : scene.name,
							children: [],
							onSelect : $scope.onTreeSceneInstanceSelected,
						};

						$scope.setupTreeSceneAnimations(scene, sceneChild);
						scenes.children.push(sceneChild);
					});

          treeDataRoot.children.push(scenes);
				};

		$scope.setupTreeSceneAnimations = function(scene,sceneChild) {
          var animations = {
						label: "Animations",
						children: []
					};

          scene.animations.forEach(function(animation) {
          	console.log("Adding Animation to Tree:", animation.name);
            animations.children.push( {
              label: animation.name,
						  onSelect: $scope.onTreeAnimaionInstanceSelected,
            });
          });

					sceneChild.children.push(animations);
				};

        $scope.setupTreeConfiguration = function(treeDataRoot)  {
          var configuration = {
						label:"Configuration",
						children: [],
						onSelect: $scope.onTreeConfigurationSelected
					};
          treeDataRoot.children.push(configuration);
				};

		$scope.onTreeProjectInstanceSelected = function(branch) {
			console.log("Selected Project:", branch);
            $state.go("Project");
		};


		$scope.onTreeConfigurationSelected = function(branch) {
			console.log("Selected Configuration:",branch);
            $state.go("ProjectConfiguration");
		};

		$scope.onTreeSceneListSelected = function(branch) {
			console.log("Selected Scenes Parent Node:",branch);
            $state.go("ProjectSceneList");
		};

		$scope.onTreeSceneInstanceSelected = function(branch) {
			console.log("Selected Scene:",branch);
            $state.go("ProjectSceneEditor",{scene:branch.label});
		};

		$scope.onTreeAnimationInstanceSelected = function(branch) {
			console.log("Selected Animation:",branch);
		};

		// Breadcrumbs ---------------------------------------------------------

        $scope.generateBreadcrumbs = function () {
            $scope.breadCrumbs = [];
            $scope.breadcrumbs.push(ProjectService.project.name);
        };

        // Toolbar Button Callbacks --------------------------------------------

        $scope.toggleFullScreen = function() {
            $scope.isFullScreen = !$scope.isFullScreen;
        };

        $scope.onNewButtonClicked = function() {
            if ($scope.isProjectModified()) {
                $scope.showSaveModifiedModal();
            }
            ProjectService.initialise();
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

				$scope.onPlayButtonClicked = function() {
					ApiConnector.runDreamProject(ProjectService.project,function(success){
						if (success) {
							$scope.addAlert("Started " + ProjectService.project.name, "success");
						} else {
							$scope.addAlert("Failed to Start " + ProjectService.project.name, "danger");
						}
					});
				};

        $scope.isProjectModified = function() {
            return ProjectService.project.isModified;
        };

				// Modals --------------------------------------------------------------

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
                    ProjectService.initialise();
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
                    ProjectService.initialise();
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
                    ProjectService.initialise();
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

		// onLoad Function Calls -----------------------------------------------

        ProjectService.initialise();

        $scope.updateUI = function() {
            $scope.generateTreeData();
            $scope.generateBreadcrumbs();
        };

        $scope.updateUI();

    }
]);
