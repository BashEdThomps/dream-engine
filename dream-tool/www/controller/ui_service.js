App.service('UIService',
    ["ProjectService",'$uibModal',
    function(ProjectService,$uibModal) {

    this.isFullScreen = false;

    // Project Tree ------------------------------------------------------------
    this.treeData = [];
    var hostController = null;

    this.setHostController = function(hc) {
        hostController = hc;
        console.log("UIService: Setting host controller",hostController);
    };

    this.generateTreeData = function() {
        console.log("Generating Tree Data");
        this.treeData = [];
        var treeDataRoot = {};
        treeDataRoot.label = ProjectService.project.name;
        treeDataRoot.children = [];
        treeDataRoot.onSelect = hostController.onTreeProjectSelected;
        this.generateTreeProjectScenes(treeDataRoot);
        this.generateTreeProjectResources(treeDataRoot);
        this.treeData.push(treeDataRoot);
    };

    // Scene -------------------------------------------------------------------

    this.generateTreeProjectScenes = function(treeDataRoot) {
        this.treeProjectScenes = {
            label:"Scenes",
            children:[],
            onSelect: hostController.onTreeProjectSceneSelected
        };
        var ui = this;
        ProjectService.project.scenes.forEach(function(scene) {
            console.log("Adding scene to tree:", scene.name);
            ui.treeProjectScenes.children.push(ui.createTreeProjectScene(scene.name));
        });

        treeDataRoot.children.push(this.treeProjectScenes);
    };

    this.createTreeProjectScene = function(name)
    {
        return {
            label :name,
            onSelect: hostController.onTreeProjectResourceInstanceSelected
        };
    };

    this.getTreeProjectSceneByName = function(name,callback){
        var retval = null;
        this.treeProjectScenes.forEach(function(scene) {
            if (scene.label === name) {
                retval  = scene;
            }
        });
        callback(scene);
    };

    this.removeTreeProjectSceneByName = function(name){
        this.getTreeProjectSceneByName(name,function(scene){
            if (scene !== null){
                this.removeTreeProjectScene(scene);
            }
        });
    };

    this.addTreeProjectScene = function(scene) {
        this.treeProjectScenes.push(scene);
    };

    this.removeTreeProjectScene = function(scene) {
        var index = this.treeProjectScenes.indexOf(scene);
        this.treeProjectScenes.splice(index,1);
    };

    // Resource ----------------------------------------------------------------

    this.addTreePrjectResource = function(resource) {
        this.treeProjectResources.push(scene);
    };

    this.createTreeProjectResource = function(name) {
        return {
            label    : name,
            children : [],
            onSelect : hostController.onTreeProjectResourceInstanceSelected,
        };
    };

    this.generateTreeProjectResources = function(treeDataRoot) {
        this.treeProjectResources = {
            label: "Resources",
            children: [],
            onSelect: hostController.onTreeProjectResourceSelected
        };
        var ui = this;

        ProjectService.getProject().resources.forEach(function(resource){
            ui.treeProjectResources.children.push(ui.createTreeProjectResource(resource.name));
        });

        treeDataRoot.children.push(this.treeProjectResources);
    };

    this.removeTreeProjectResource = function(resource) {
        var index = this.treeProjectResources.indexOf(resource);
        this.treeProjectResources.splice(index,1);
    };

    this.getTreeProjectResourceByName = function(name,callback){
        var retval = null;
        this.treeProjectResources.forEach(function(res) {
            if (res.label === name) {
                retval  = res;
            }
        });
        callback(res);
    };

    this.removeTreeProjectResourceByName = function(name) {
        this.getTreeProjectResourceByName(name, function(res){
            if (res !== null) {
                this.removeTreeProjectResource(res);
            }
        });
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
