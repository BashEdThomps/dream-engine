App.service('UIService',
    ["ProjectService","$state",
    function(ProjectService,$state) {

    // Breadcrumbs ---------------------------------------------------------
    this.breadcrumbs = [];

    this.setBreadcrumbs = function (bc) {
        console.log("Setting breadcrumbs:",bc);
        this.breadcrumbs = bc;
    };

    this.getBreadcrumbs = function() {
        return this.breadcrumbs;
    };

    
    // Return UIService --------------------------------------------------------

    return this;
}]);
