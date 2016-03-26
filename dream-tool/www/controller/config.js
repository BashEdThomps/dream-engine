App.config(function($stateProvider, $urlRouterProvider) {

	// Home Page -----------------------------------------------------------------

  $urlRouterProvider.otherwise("/home");

  $stateProvider.state('home', {
      url: "/home",
      templateUrl: "view/partials/home.html",
      controller: "home"
    });

	// Project -------------------------------------------------------------------
		
  $stateProvider.state('project.configuration', {
      url: "/project/configuration",
      templateUrl: "view/partials/project/configuration.html",
      controller: "project.configuration"
    });

	// Help ----------------------------------------------------------------------
});
