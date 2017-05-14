App.service('UtilService',
[
  function() {

    this.generateUUID = function()
    {
      return this.s4()+'-'+this.s4()+'-'+this.s4()+'-'+this.s4();
    };

    this.s4 = function()
    {
      return Math.floor((1 + Math.random()) * 0x10000).toString(16).substring(1);
    };

    this.readFileAsTextFromElement = function(element, callback)
    {
      var f = element.files[0];
      if (f === null)
      {
        callback(null);
      }

      r = new FileReader();
      r.onloadend = function(e)
      {
        var data = e.target.result;
        callback(data);
      };
      r.readAsText(f);
    };

    this.readFileAsBinaryFromElement = function(element, callback) {
      var f = element.files[0];
      if (f === null)
      {
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
    if (f === null)
    {
      callback(null);
    }
    console.log("Getting name from file",f);
    callback(f.name);
  };

  this.readFilesFromElementAsync = function(element,callback) {
    var fileArray = element.files;
    if (fileArray === null)
    {
      callback(null);
    }
    console.log("Getting file array",fileArray);
    for(var i=0;i<fileArray.length;i++)
    {
      (function(myI) 
      {
        var file = fileArray[myI];
        reader = new FileReader();
        reader.onloadend = function(event)
        {
          var data = event.target.result;
          callback(
            {
              name: file.name,
              data: data
            }
          );
        };
        reader.readAsDataURL(file);
      })(i);
    }
  };
  return this;
}]);
