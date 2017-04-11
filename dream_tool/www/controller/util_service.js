App.service('UtilService',
    [function() {
        this.generateUUID = function() {
              return this.s4()+'-'+this.s4()+'-'+this.s4()+'-'+this.s4();
        };

        this.s4 = function() {
            return Math.floor((1 + Math.random()) * 0x10000).toString(16).substring(1);
        };

        this.readFileAsTextFromElement = function(element, callback) {
            var f = element.files[0];

            if (f === null) {
                callback(null);
            }

            r = new FileReader();
            r.onloadend = function(e){
                var data = e.target.result;
                callback(data);
            };
            r.readAsText(f);
        };

        this.readFileAsBinaryFromElement = function(element, callback) {
            var f = element.files[0];

            if (f === null) {
                callback(null);
            }

            r = new FileReader();
            r.onloadend = function(e){
                var data = e.target.result;
                callback(data);
            };
            r.readAsDataURL(f);
        };

        this.readFileNameFromElement = function(element,callback) {
            var f = element.files[0];

            if (f === null) {
                callback(null);
            }
            console.log("Getting name from file",f);
            callback(f.name);
        };

        return this;
}]);
