App.controller("SettingsModal",
    ["$scope","$uibModalInstance","CookieService","UIService","cssInjector",
    function($scope, $uibModalInstance,CookieService,UIService,cssInjector) {
        $scope.themeNames = [
            "bootstrap",
            "cosmo"    ,
            "cyborg"   ,
            "darkly"   ,
            "flatly"   ,
            "simplex"  ,
            "slate"    ,
            "spacelab" ,
            "superhero",
        ];

        $scope.execPath = CookieService.getDreamExecPath();
        $scope.theme = CookieService.getTheme();

        $scope.onCancelClicked = function() {
            $uibModalInstance.dismiss();
        };

        $scope.onSaveClicked = function() {
            CookieService.setTheme($scope.theme);
            CookieService.setDreamExecPath($scope.execPath);
            UIService.addAlert("Settings Saved","success");
            cssInjector.removeAll();
            cssInjector.add('css/'+$scope.theme+".css");
            $uibModalInstance.close(true);
        };
    }
]);
