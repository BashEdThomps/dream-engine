App.directive('ainmationTimeline',
    ['d3Factory','animationTimelineFactory',
    function(d3Factory, animationTimelineFactory) {
        return {
            restrict: 'EA',
            scope: {},
            link: function(scope, element, attrs) {
                d3Factory.d3().then(function(d3) {
                    scope.testData = [
                        {
                            times: [
                                {"starting_time": 1355752800000, "ending_time": 1355759900000},
                            ]
                        },
                        {
                            times: [
                                {"starting_time": 1355759910000, "ending_time": 1355761900000},
                            ]
                        },
                        {
                            times: [
                                {"starting_time": 1355761910000, "ending_time": 1355763910000}
                            ]
                        },
                    ];

                    var width = 500;
                    var chart =
                        d3.timeline().stack().showTimeAxisTick().background('grey')
                        .tickFormat({
                            format: d3.time.format("%I %p"),
                            tickTime: d3.time.hours,
                            tickInterval: 1,
                            tickSize: 30
                        })
                        .display("circle")
                        .click(function (d, i, datum) {
                            alert(datum.label);
                        });

                    var svg = d3.select(element[0]).append("svg").attr("width", width)
                        .datum(testData).call(chart);

                    // Browser onresize event
                    window.onresize = function() {
                        scope.$apply();
                    };

                    // Watch for resize event
                    scope.$watch(function() {
                        return angular.element($window)[0].innerWidth;
                    }, function() {
                        scope.render(scope.testData);
                    });

                      scope.render = function(data) {
                        // our custom d3 code
                    };
                });
            }
        };
    }
]);
