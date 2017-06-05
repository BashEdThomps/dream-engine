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
        this.treeDataRoot.expanded = true;

        this.generateTreeProjectScenes();
        this.generateTreeProjectAssets();
        this.treeData.push(this.treeDataRoot);
    };

    // Scene -------------------------------------------------------------------

    this.generateTreeProjectScenes = function() {
        this.treeProjectScenes = {
            label    : "Scenes",
            children : [],
            onSelect : hostController.onTreeProjectSceneSelected,
            expanded : true,
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
            children : [],
            expanded : true,
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
            onSelect : null,
            expanded : true
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

    // Assets ---------------------------------------------------------------

    this.addTreeProjectAsset = function(asset) {
      var node = this.createTreeProjectAsset(asset);
        switch (asset.type) {
          case ProjectService.ASSET_TYPE_ANIMATION:
            this.treeAnimationAssetsNode.children.push(node)
            break;
          case ProjectService.ASSET_TYPE_AUDIO:
            this.treeAudioAssetsNode.children.push(node);
            break;
          case ProjectService.ASSET_TYPE_FONT:
            this.treeFontAssetsNode.children.push(node);
            break;
          case ProjectService.ASSET_TYPE_LIGHT:
            this.treeLightAssetsNode.children.push(node);
            break;
          case ProjectService.ASSET_TYPE_MODEL:
            this.treeModelAssetsNode.children.push(node);
            break;
          case ProjectService.ASSET_TYPE_PHYSICS:
            this.treePhysicsAssetsNode.children.push(node);
            break;
          case ProjectService.ASSET_TYPE_SHADER:
            this.treeShaderAssetsNode.children.push(node);
            break;
          case ProjectService.ASSET_TYPE_SCRIPT:
            this.treeScriptAssetsNode.children.push(node);
            break;
          case ProjectService.ASSET_TYPE_SPRITE:
            this.treeSpriteAssetsNode.children.push(node);
            break;
        };
    };

    this.createTreeProjectAsset = function(asset) {
        return {
            label    : asset.name,
            uuid     : asset.uuid,
            onSelect : hostController.onTreeProjectAssetInstanceSelected,
            expanded : true
        };
    };

    this.generateTreeProjectAssets = function() {

        this.treeProjectAssets = {
            label: "Asset Definitions",
            children: [],
            onSelect: hostController.onTreeProjectAssetSelected,
            expanded : true
        };

        this.treeAnimationAssetsNode = {
            label: "Animation",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeAnimationAssetsNode);

        this.treeAudioAssetsNode = {
            label: "Audio",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeAudioAssetsNode);

        this.treeFontAssetsNode = {
            label: "Font",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeFontAssetsNode);

        this.treeLightAssetsNode = {
            label: "Light",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeLightAssetsNode);

        this.treeModelAssetsNode = {
            label: "Model",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeModelAssetsNode);

        this.treePhysicsAssetsNode = {
            label: "Physics",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treePhysicsAssetsNode);

        this.treeShaderAssetsNode = {
            label: "Shader",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeShaderAssetsNode);

        this.treeScriptAssetsNode = {
            label: "Script",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeScriptAssetsNode);

        this.treeSpriteAssetsNode = {
            label: "Sprite",
            children: [],
            expanded : true
        };
        this.treeProjectAssets.children.push(this.treeSpriteAssetsNode);

        var ui = this;
        ProjectService.getProject().assets.forEach(function(asset){
           ui.addTreeProjectAsset(asset);
        });

        ui.treeDataRoot.children.push(this.treeProjectAssets);
    };

    this.removeTreeProjectAsset = function(asset) {
        var index = this.treeProjectAssets.children.indexOf(asset);
        if (this.treeProjectAssets) {
          this.treeProjectAssets.children.splice(index,1);
        }
    };

    this.getTreeProjectAssetByUUID = function(uuid,callback){
        var retval = null;
        if (this.treeProjectAssets) {
          this.treeProjectAssets.children.forEach(function(res) {
              if (res.uuid === uuid) {
                  retval  = res;
              }
          });
        }
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
          if (branch !== null) {
            branch.label = assetObject.name;
          }
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
