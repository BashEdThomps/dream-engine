App.service('CookieService',
    ['$cookieStore',
    function($cookieStore){

        var COOKIE_EXEC_PATH = "dream-bin";
        var COOKIE_THEME     = "theme";

        this.setTheme = function(theme) {
            $cookieStore.put(COOKIE_THEME,theme);
        };

        this.getTheme = function() {
            return $cookieStore.get(COOKIE_THEME);
        };

        this.setDreamExecPath = function(path) {
            $cookieStore.put(COOKIE_EXEC_PATH,path);
        };

        this.getDreamExecPath = function() {
            return $cookieStore.get(COOKIE_EXEC_PATH);
        };

        return this;
    }
]);
