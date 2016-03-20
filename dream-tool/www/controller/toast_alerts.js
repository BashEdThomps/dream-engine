App.factory('toastAlerts',
    function() {
        return {
            // Create an alert object
            newAlert : function(text, type, duration) {
                return {
                    text: text,
                    type: type,
                    duration: duration
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
