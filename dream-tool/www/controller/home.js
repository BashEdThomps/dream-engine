App.controller("home",["$scope","ProjectService","UIService",
    function($scope,ProjectService,UIService) {
        UIService.setBreadcrumbs([ProjectService.getName(),"Home"]);
    }
]);
