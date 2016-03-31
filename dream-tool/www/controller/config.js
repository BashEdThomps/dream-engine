App.config(['$compileProvider',
    function ($compileProvider) {
        $compileProvider.aHrefSanitizationWhitelist(/^\s*(https?|ftp|mailto|tel|file|blob):/);
}]);

App.config(function($stateProvider, $urlRouterProvider) {
    // Home Page -----------------------------------------------------------------
    //$urlRouterProvider.otherwise("/home");
    $stateProvider.state('home', {
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
    // Resource List
    $stateProvider.state('ProjectResourceList', {
        url         : "/project/resource/list",
        templateUrl : "view/partials/project/resource/list.html",
        controller  : "ProjectResourceList"
    });
    // Resource Editor
    $stateProvider.state('ProjectResourceEditor', {
        url         : "/project/resource/editor/:resource",
        templateUrl : "view/partials/project/resource/editor.html",
        controller  : "ProjectResourceEditor"
    });
	// Help ----------------------------------------------------------------------
});
