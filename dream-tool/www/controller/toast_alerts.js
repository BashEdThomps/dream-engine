App.factory('ToastAlerts',
    function() {
        var defaultDuration = 3000;
        return {
            // Create an alert object
            newAlert : function(text, type, duration) {
                return {
                    text: text,
                    type: type,
                    duration: (duration === undefined ? defaultDuration : duration)
                };
            },
            // Add an alert to the page
   		    addAlert : function(alertList,alert) {
   			    alertList.push(alert);
   		    },
            // Close an alert from the page
   		    closeAlert : function(alertList,index) {
   			    alertList.splice(index, 1);
   		    }
        };
    }
);
