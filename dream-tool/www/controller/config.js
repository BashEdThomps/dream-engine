App.config(function($stateProvider, $urlRouterProvider) {
    // Home Page -----------------------------------------------------------------
    $urlRouterProvider.otherwise("/home");
    $stateProvider.state('home', {
        url: "/home",
        templateUrl: "view/partials/home.html",
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
	// Help ----------------------------------------------------------------------
});
