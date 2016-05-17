App.service('UIService',
    ['ProjectService','$uibModal','ngToast',
    function(ProjectService,$uibModal,ngToast) {

    this.isFullScreen = false;

    // Project Tree ------------------------------------------------------------
    this.treeData       = [];
    this.treeDataRoot   = {};
    this.hostController = null;

    this.setHostController = function(hc) {
        hostController = hc;
        console.log("UIService: Setting host controller",hostController);
    };

    this.generateTreeData = function() {
        console.log("Generating Tree Data");
        this.treeData     = [];
        this.treeDataRoot = {};

        this.treeDataRoot.label    = ProjectService.project.name;
        this.treeDataRoot.children = [];
        this.treeDataRoot.onSelect = hostController.onTreeProjectSelected;

        this.generateTreeProjectScenes();
        this.generateTreeProjectAssets();
        this.treeData.push(this.treeDataRoot);
    };

    // Scene -------------------------------------------------------------------

    this.generateTreeProjectScenes = function() {
        this.treeProjectScenes = {
            label    : "Scenes",
            children : [],
            onSelect : hostController.onTreeProjectSceneSelected
        };
        var ui = this;
        ProjectService.project.scenes.forEach(function(scene) {
            console.log("Adding scene to tree:", scene.name);
            ui.addTreeProjectScene(ui.createTreeProjectScene(scene));
        });
        ui.treeDataRoot.children.push(this.treeProjectScenes);
    };

    this.createTreeProjectScene = function(scene) {
        var ui = this;
        var retval = {
            label    : scene.name,
            uuid     : scene.uuid,
            onSelect : hostController.onTreeProjectSceneInstanceSelected,
            children : []
        };
        /*
        scene.objects.forEach(function(sceneObject){
            retval.children.push(ui.createTreeProjectSceneSceneObject(sceneObject));
        });
        */
        return retval;
    };

    this.createTreeProjectSceneSceneObject = function(sceneObject) {
        return {
            label : sceneObject.name,
            uuit  : sceneObject.uuid,
            onSelect : null
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

    // Asset ----------------------------------------------------------------

    this.addTreeProjectAsset = function(asset) {
        this.treeProjectAssets.children.push(asset);
    };

    this.createTreeProjectAsset = function(asset) {
        return {
            label    : asset.name,
            uuid     : asset.uuid,
            onSelect : hostController.onTreeProjectAssetInstanceSelected,
        };
    };

    this.generateTreeProjectAssets = function() {
        this.treeProjectAssets = {
            label: "Assets",
            children: [],
            onSelect: hostController.onTreeProjectAssetSelected
        };
        var ui = this;
        ProjectService.getProject().assets.forEach(function(asset){
            ui.treeProjectAssets.children.push(ui.createTreeProjectAsset(asset));
        });
        ui.treeDataRoot.children.push(this.treeProjectAssets);
    };

    this.removeTreeProjectAsset = function(asset) {
        var index = this.treeProjectAssets.children.indexOf(asset);
        this.treeProjectAssets.children.splice(index,1);
    };

    this.getTreeProjectAssetByUUID = function(uuid,callback){
        var retval = null;
        this.treeProjectAssets.children.forEach(function(res) {
            if (res.uuid === uuid) {
                retval  = res;
            }
        });
        callback(retval);
    };

    this.removeTreeProjectAssetByUUID = function(uuid) {
        var ui = this;
        this.getTreeProjectAssetByUUID(uuid, function(res){
            if (res !== null) {
                ui.removeTreeProjectAsset(res);
            }
        });
    };

    this.updateProject = function(project){
        this.treeDataRoot.label = project.name;
    };

    this.updateAsset = function(assetObject) {
        this.updateTreeAssetName(assetObject);
    };

    this.updateTreeAssetName = function(assetObject) {
        this.getTreeProjectAssetByUUID(assetObject.uuid,function(branch){
            branch.label = assetObject.name;
        });
    };

    this.updateScene = function(sceneObject) {
        this.updateTreeSceneName(sceneObject);
    };

    this.updateTreeSceneName = function(sceneObject) {
        this.getTreeProjectSceneByUUID(sceneObject.uuid,function(branch){
            branch.label = sceneObject.name;
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

    this.showSettingsModal = function() {
        var modal = $uibModal.open(
            {
                animation: this.modalAnimationsEnabled,
                templateUrl: 'view/partials/modals/settings/settings.html',
                controller: 'SettingsModal'
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
