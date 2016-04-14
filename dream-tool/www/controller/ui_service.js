App.service('UIService',
    ['ProjectService','$uibModal','ngToast',
    function(ProjectService,$uibModal,ngToast) {

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
            ui.addTreeProjectScene(ui.createTreeProjectScene(scene));
        });
        treeDataRoot.children.push(this.treeProjectScenes);
    };

    this.createTreeProjectScene = function(scene) {
        return {
            label    : scene.name,
            uuid     : scene.uuid,
            onSelect : hostController.onTreeProjectSceneInstanceSelected
        };
    };

    this.getTreeProjectSceneByUUID = function(name,callback){
        var retval = null;
        this.treeProjectScenes.children.forEach(function(scene) {
            if (scene.uuid === name) {
                retval  = scene;
            }
        });
        callback(retval);
    };

    this.removeTreeProjectSceneByUUID = function(uuid) {
        var ui = this;
        this.getTreeProjectSceneByUUID(uuid,function(scene) {
            if (scene !== null) {
                ui.removeTreeProjectScene(scene);
            }
        });
    };

    this.addTreeProjectScene = function(scene) {
        this.treeProjectScenes.children.push(scene);
    };

    this.removeTreeProjectScene = function(scene) {
        var index = this.treeProjectScenes.children.indexOf(scene);
        this.treeProjectScenes.children.splice(index,1);
    };

    // Resource ----------------------------------------------------------------

    this.addTreeProjectResource = function(resource) {
        this.treeProjectResources.children.push(resource);
    };

    this.createTreeProjectResource = function(resource) {
        return {
            label    : resource.name,
            uuid     : resource.uuid,
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
            ui.treeProjectResources.children.push(ui.createTreeProjectResource(resource));
        });
        treeDataRoot.children.push(this.treeProjectResources);
    };

    this.removeTreeProjectResource = function(resource) {
        var index = this.treeProjectResources.children.indexOf(resource);
        this.treeProjectResources.children.splice(index,1);
    };

    this.getTreeProjectResourceByUUID = function(uuid,callback){
        var retval = null;
        this.treeProjectResources.children.forEach(function(res) {
            if (res.uuid === uuid) {
                retval  = res;
            }
        });
        callback(retval);
    };

    this.removeTreeProjectResourceByUUID = function(uuid) {
        var ui = this;
        this.getTreeProjectResourceByUUID(uuid, function(res){
            if (res !== null) {
                ui.removeTreeProjectResource(res);
            }
        });
    };

    // Toast Alerts ------------------------------------------------------------

    // Add an alert to the page
  	this.addAlert = function(msg,type) {
        ngToast.create({
            className : type,
            content   : msg,
        });
  	};

    // Close an alert from the page
  	this.closeAlert = function(index) {
  	    ngToast.dismiss(index);
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

    this.showUploadModal = function(callback) {
        var modal = $uibModal.open({
            animation: this.modalAnimationsEnabled,
            templateUrl: 'view/partials/modals/upload.html',
            controller: 'UploadProjectModal'
        });
        modal.result.then(callback);
    };

    this.showOpenProjectModal = function(callback) {
        var modal = $uibModal.open(
            {
                animation: this.modalAnimationsEnabled,
                templateUrl: 'view/partials/modals/open.html',
                controller: 'OpenProjectModal'
            }
        );
        modal.result.then(callback);
    };

    this.showSettingsThemeModal = function() {
        var modal = $uibModal.open(
            {
                animation: this.modalAnimationsEnabled,
                templateUrl: 'view/partials/modals/settings/theme.html',
                controller: 'SettingsThemeModal'
            }
        );
        modal.result.then(function(){});
    };

    this.showSettingsAboutModal = function() {
        var modal = $uibModal.open(
            {
                animation: this.modalAnimationsEnabled,
                templateUrl: 'view/partials/modals/settings/about.html',
                controller: 'SettingsAboutModal'
            }
        );
        modal.result.then(function(){});
    };

    // Return UIService --------------------------------------------------------

    return this;
}]);
