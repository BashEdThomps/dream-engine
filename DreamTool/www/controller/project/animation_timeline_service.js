angular.module('AnimationTimelineDirectives', ['d3'])
    .directive('AnimationTimeline', ['d3Service',
        function(d3Service) {
            return {
                link: function(scope, element, attrs) {
                    d3Service.d3().then(function(d3) {
                        // d3 is the raw d3 object
                        var testData = [
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

                        function timelineBgndTick() {
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

                          var svg = d3.select("#timelineBgndTick").append("svg").attr("width", width)
                              .datum(testData).call(chart);
                        }

                        timelineBgndTick();
                    });
                }
            };
        }
    ]
);
