App.config(['$compileProvider',
    function ($compileProvider) {
        $compileProvider.aHrefSanitizationWhitelist(/^\s*(https?|ftp|mailto|tel|file|blob):/);
}]);

App.config(function($stateProvider, $urlRouterProvider) {
    // Home Page -----------------------------------------------------------------
    //$urlRouterProvider.otherwise("/home");
    $stateProvider.state('Home', {
        url: "/home",
        templateUrl: "view/partials/home/home.html",
        controller: "home"
    });
    // Project -------------------------------------------------------------------
    $stateProvider.state('Project', {
        url         : "/project",
        templateUrl : "view/partials/project/project.html",
        controller  : "Project"
    });
    // Configuration
    $stateProvider.state('ProjectConfiguration', {
        url         : "/project/configuration",
        templateUrl : "view/partials/project/configuration.html",
        controller  : "ProjectConfiguration"
    });
    // Scene List
    $stateProvider.state('ProjectSceneList', {
        url         : "/project/scene/list",
        templateUrl : "view/partials/project/scene/list.html",
        controller  : "ProjectSceneList"
    });
    // Scene Editor
    $stateProvider.state('ProjectSceneEditor', {
        url         : "/project/scene/editor/:scene",
        templateUrl : "view/partials/project/scene/editor.html",
        controller  : "ProjectSceneEditor"
    });
    // Asset List
    $stateProvider.state('ProjectAssetList', {
        url         : "/project/asset/list",
        templateUrl : "view/partials/project/asset/list.html",
        controller  : "ProjectAssetList"
    });
    // Asset Editor
    $stateProvider.state('ProjectAssetEditor', {
        url         : "/project/asset/editor/:asset",
        templateUrl : "view/partials/project/asset/editor.html",
        controller  : "ProjectAssetEditor"
    });
	// Help ----------------------------------------------------------------------
});

App.config(['ngToastProvider',function(ngToast) {
    ngToast.configure({
        verticalPosition   : 'bottom',
        horizontalPosition : 'center',
        maxNumber          : 3,
        animation          : 'slide'
    });
}]);
