App.service('UIService',
    ["ProjectService",'$uibModal',
    function(ProjectService,$uibModal) {

    this.isFullScreen = false;

    // Project Tree ------------------------------------------------------------
    this.treeData = [];
    var hostController = null;

    this.setHostController = function(hc){
        hostController = hc;
        console.log("UIService: Setting host controller",hostController);
    };

    this.generateTreeData = function() {
        console.log("Generating Tree Data");
        this.treeData = [];
        var treeDataRoot = {};
        treeDataRoot.label = ProjectService.project.name;
        treeDataRoot.children = [];
        treeDataRoot.onSelect = hostController.onTreeProjectNodeSelected;
        this.setupTreeScenes(treeDataRoot);
        this.setupTreeResources(treeDataRoot);
        this.treeData.push(treeDataRoot);
    };

    this.setupTreeScenes = function(treeDataRoot) {
        var scenes = {
            label:"Scenes",
            children:[],
            onSelect: hostController.onTreeSceneNodeSelected
        };

        ProjectService.project.scenes.forEach(function(scene) {
            console.log("Adding scene to tree:", scene.name);

            var sceneChild = {
                label    : scene.name,
                children: [],
                onSelect : hostController.onTreeSceneInstanceSelected,
            };
            scenes.children.push(sceneChild);
        });

        treeDataRoot.children.push(scenes);
    };

    this.setupTreeResources = function(treeDataRoot) {
        var resourcesNode = {
            label: "Resources",
            children: [],
            onSelect: hostController.onTreeResourcesNodeSelected
        };
        ProjectService.getProject().resources.forEach(function(resource){
            resourcesNode.children.push({
                label : resource.name,
                onSelect: hostController.onTreeProjectResourceInstanceSelected
            });
        });
        treeDataRoot.children.push(resourcesNode);
    };

    // Toast Alerts ------------------------------------------------------------

    this.defaultToastDuration = 3000;
    this.toastAlertList = [];

    // Create an alert object
    this.newAlert = function(text, type, duration) {
        return {
            text: text,
            type: type,
            duration: (duration === undefined ? this.defaultToastDuration : duration)
        };
    };

    // Add an alert to the page
  	this.addAlert = function(msg,type) {
  	    this.toastAlertList.push(this.newAlert(msg,type,this.defaultToastDuration));
  	};

    // Close an alert from the page
  	this.closeAlert = function(index) {
  	    this.toastAlertList.splice(index, 1);
 	};

    // Breadcrumbs ---------------------------------------------------------

    this.breadcrumbs = [];

    this.setBreadcrumbs = function (bc) {
        console.log("Setting breadcrumbs:",bc);
        this.breadcrumbs = bc;
    };

    this.getBreadcrumbs = function() {
        return this.breadcrumbs;
    };

    this.update = function() {
        console.log("Updating UI");
        this.generateTreeData();
        hostController.alertList = this.toastAlertList;
        hostController.treeData = this.treeData;
        hostController.isProjectModified = ProjectService.isModified;
        hostController.breadcrumbs  = this.breadcrumbs;
    };

    // Modals ------------------------------------------------------------------

    this.modalAnimationsEnabled = true;

    this.showSaveModifiedModal = function(yesCallback,noCallback) {
        var modal = $uibModal.open({
            animation: this.modalAnimationsEnabled,
            templateUrl: 'view/partials/modals/save_modified.html',
            controller: 'YesNoModal'
        });

        modal.result.then(function (result) {
            if (result) {
                yesCallback();
            } else {
                noCallback();
            }
        }, function () {
            noCallback();
        });
    };

    this.showOpenModal = function() {
        var modal = $uibModal.open({
            animation: this.odalAnimationsEnabled,
            templateUrl: 'view/partials/modals/open.html',
            controller: 'OpenFileModal'
        });

        modal.result.then(function (result) {
            if (result) {

            } else {

            }
        }, function () {

        });
    };

    // Return UIService --------------------------------------------------------

    return this;
}]);
