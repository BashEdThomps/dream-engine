App.controller("index",
    ["$state","$scope","$uibModal", "toastAlerts","apiConnector",
    function($state,$scope, $uibModal, toastAlerts, apiConnector) {
        $scope.modalAnimationsEnabled = true;
        $scope.isFullScreen = false;
        $scope.breadcrumbs = [];
        $scope.alertList = [];

        $scope.initProject = function() {
            $scope.project = {
                name: "Untitled Project",
								configuration: {
									animationEnabled: true,
								  collisionEnabled: true,
								  physicsEnabled:   true,
								},
								scenes: [ 
									{
										name:"Untitled Scene",
										animations: [ {
											name: "Untitled Animation",
										}],
										audio:{
											soundEffects: [{
												name: "Untitled SoundEffect"
											}],
											music: [{
												name: "Untitled Music"
											}],
										},
									}
								],
               	isModified: false 
            };

            $scope.addAlert("New Project Created","success");
            console.log("Project initialised.");
        };

				// Alerts --------------------------------------------------------------
				
        $scope.addAlert = function(message,type) {
            toastAlerts.addAlert($scope.alertList,toastAlerts.newAlert(message,type));
        };

        $scope.closeAlert = function(index) {
            toastAlerts.closeAlert($scope.alertList, index);
        };

				// Project Tree --------------------------------------------------------
				
        $scope.generateTreeData = function() {
            $scope.treeData = [];

            var treeDataRoot = {};
            treeDataRoot.label = $scope.project.name;
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

					$scope.project.scenes.forEach(function(scene) {
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

				};

				$scope.onTreeConfigurationSelected = function(branch) {
					console.log("Selected Configuration:",branch);
				};

				$scope.onTreeSceneListSelected = function(branch) {
					console.log("Selected Scenes Parent Node:",branch);
				};

				$scope.onTreeSceneInstanceSelected = function(branch) {
					console.log("Selected Scene:",branch);
				};

				$scope.onTreeAnimationInstanceSelected = function(branch) {
					console.log("Selected Animation:",branch);
				};
				
				// Breadcrumbs ---------------------------------------------------------
				
        $scope.generateBreadcrumbs = function () {
            $scope.breadcrumbs.push($scope.project.name);
        };

        // Toolbar Button Callbacks --------------------------------------------
	
        $scope.toggleFullScreen = function() {
            $scope.isFullScreen = !$scope.isFullScreen;
        };

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

				$scope.onPlayButtonClicked = function() {
					apiConnector.runDreamProject($scope.project,function(success){
						if (success) {
							$scope.addAlert("Started " + $scope.project.name, "success");
						} else {
							$scope.addAlert("Failed to Start " + $scope.project.name, "danger");
						}
					});
				};

        $scope.isProjectModified = function() {
            return $scope.project.isModified;
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

				// onLoad Function Calls -----------------------------------------------
				
        $scope.initProject();
        $scope.generateTreeData();
        $scope.generateBreadcrumbs();
    }
]);
