App.config(function($stateProvider, $urlRouterProvider) {
  $urlRouterProvider.otherwise("/home");

  $stateProvider.state('home', {
      url: "/home",
      templateUrl: "view/partials/home.html",
      controller: "home"
    });
});
