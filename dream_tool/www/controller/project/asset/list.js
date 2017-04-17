App.controller("ProjectAssetList",
["$scope","$state","ProjectService","UIService","UtilService","ApiService",
function($scope,$state,ProjectService,UIService,UtilService,ApiService)
{

  $scope.assetFormats = {};
  $scope.physicsObjectAssets = [];

  $scope.doOnLoad = function()
  {
    if (ProjectService.isProjectOpen())
    {
      $scope.project = ProjectService.getProject();
      UIService.setBreadcrumbs([ProjectService.getName(),"Assets"]);
      UIService.update();
    }
    else
    {
      UIService.addAlert("No Project Open!","danger");
      $state.go("Home");
    }

    ApiService.getTemplateList(function (templateList)
    {
      if (templateList)
      {
        console.log("Got template list",templateList);
        templateList.forEach(function(typeList)
        {
          if (typeList.type == ProjectService.ASSET_TYPE_SHADER)
          {
            $scope.shaderTemplates = typeList.names;
          }
          else if (typeList.type == ProjectService.ASSET_TYPE_SCRIPT)
          {
            $scope.scriptTemplates = typeList.names;
          }
          else
          {
            console.error ("Unknown template list for type",templateList.type);
          }
        });
      }
      else
      {
        console.error("Unable to get template list");
      }
    });

    ProjectService.getAssetTypesObject(function(assetTypesObj)
    {
      $scope.assetTypesObject = assetTypesObj;
    });

    ProjectService.getFontAssetFormats (function(fontFormats)
    {
      $scope.assetFormats.font = fontFormats;
    });

    ProjectService.getAudioAssetFormats (function(audioFormats)
    {
      $scope.assetFormats.audio = audioFormats;
    });

    ProjectService.getAnimationAssetFormats(function(animFormats)
    {
      $scope.assetFormats.animation = animFormats;
    });

    ProjectService.getPhysicsObjectAssetFormats(function(colShapeFormats)
    {
      $scope.assetFormats.physicsObject = colShapeFormats;
    });

    ProjectService.getModelAssetFormats(function(modelFormats)
    {
      $scope.assetFormats.model = modelFormats;
    });

    ProjectService.getScriptAssetFormats(function(scriptFormats)
    {
      $scope.assetFormats.script = scriptFormats;
    });

    ProjectService.getSpriteAssetFormats(function(spriteFormats)
    {
      $scope.assetFormats.sprite = spriteFormats;
    });


    console.log("Asset Formats:",$scope.assetFormats)
  };

  $scope.isAssetTypeAnimation = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_ANIMATION;
  };

    $scope.isAssetTypeLight = function()
    {
        if (!$scope.currentAsset)
        {
          return false;
        }
        return $scope.currentAsset.type == ProjectService.ASSET_TYPE_LIGHT;
    }

  $scope.isAssetTypeFont = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_FONT;
  };

  $scope.isAssetTypeAudio = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_AUDIO;
  };

  $scope.isAssetTypePhysicsObject = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_PHYSICS_OBJECT;
  };

  $scope.isAssetTypeModel = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_MODEL;
  };

  $scope.isAssetTypeScript= function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_SCRIPT;
  };

  $scope.isAssetTypeShader = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_SHADER;
  };

  $scope.isAssetTypeSprite = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.type == ProjectService.ASSET_TYPE_SPRITE;
  };

  $scope.isAudioAssetFormatMusic = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_MUSIC;
  };

  $scope.isAudioAssetFormatSfx = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_SFX;
  };

  $scope.isModelAssetFormatAssimp = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_MODEL_ASSIMP;
  };

  $scope.isScriptAssetFormatLua = function()
  {
    if (!$scope.currentAsset) return false;
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_SCRIPT_LUA;
  };

  $scope.isPhysicsObjectAssetFormatSphere = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_SPHERE;
  };

  $scope.isPhysicsObjectAssetFormatStaticPlane = function()
  {
      if (!$scope.currentAsset)
      {
        return false;
      }
      return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_STATIC_PLANE;
  };

  $scope.isPhysicsObjectAssetFormatBox = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_BOX;
  };

  $scope.isPhysicsObjectAssetFormatCompound = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_COMPOUND;
  };

  $scope.isPhysicsObjectAssetFormatBvhTriangleMesh = function()
  {
    if (!$scope.currentAsset)
    {
      return false;
    }
    return $scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_BVH_TRIANGLE_MESH;
  };

  $scope.goToAssetEditor = function(uuid)
  {
    $state.go("ProjectAssetEditor",{asset: uuid});
    return;
  };

  $scope.onNewAssetButtonClicked = function()
  {
    $scope.currentAsset = ProjectService.createAsset();
    ProjectService.pushAsset($scope.currentAsset);
    UIService.addTreeProjectAsset($scope.currentAsset);
    UIService.update();
  };

  $scope.onRemoveAssetButtonClicked = function(uuid)
  {
    ProjectService.getAssetByUUID(uuid,function(asset)
    {
      if (asset !== null)
      {
        ProjectService.removeAsset(asset);
        UIService.removeTreeProjectAssetByUUID(uuid);
      }
      else
      {
        UIService.addAlert("Error: Could not remove asset!","danger");
      }
    });
  };

  $scope.onAssetSelected = function(uuid)
  {
    console.log("Selected Asset",uuid);
    ProjectService.getAssetByUUID(uuid,function(asset)
    {
      $scope.currentAsset = asset;
      $scope.updateAssetUIVariables();
    });
  };

  $scope.updateAssetUIVariables = function()
  {
    console.log("Updating Asset Variables for",$scope.currentAsset);

    if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_AUDIO)
    {
      console.log("Checking for existing audio asset");
      if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_SFX)
      {
        ProjectService.assetHasAudioSfx($scope.currentAsset.uuid,function(result)
        {
          console.log("Audio Sfx Asset Exists",result);
          $scope.hasAudioSfx = result;
        });
      }
      else if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_AUDIO_MUSIC)
      {
        ProjectService.assetHasAudioMusic($scope.currentAsset.uuid,function(result)
        {
          console.log("Audio Music Asset Exists",result);
          $scope.hasAudioMusic = result;
        });
      }
    }
    else if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_PHYSICS_OBJECT)
    {
      console.log("Checking for existing physics object asset");
      if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_BVH_TRIANGLE_MESH)
      {
        ProjectService.assetHasPhysicsObjectBvhTriangleMesh($scope.currentAsset.uuid,function(result)
        {
          console.log("PhysicsObject BvhTriangleMesh Asset Exists",result);
          $scope.hasPhysicsObjectBvhTriangleMesh = result;
        });
      }
    }
    else if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_MODEL)
    {
      console.log("Checking for existing model asset");
      if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_MODEL_ASSIMP)
      {
        ProjectService.assetHasModelAssimp($scope.currentAsset.uuid,function(result)
        {
          console.log("Model Assimp Asset Exists",result);
          $scope.hasModelAssimp = result;
        });
      }
    }
    else if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_SHADER)
    {
      console.log("Checking for existsing shader asset");
      ProjectService.assetHasVertexShader($scope.currentAsset.uuid,function(result)
      {
        console.log("VertexShader asset exists",result);
        $scope.hasVertexShader = result;
      });
      ProjectService.assetHasFragmentShader($scope.currentAsset.uuid,function(result)
      {
        console.log("FragmentShader exists",result);
        $scope.hasFragmentShader = result;
      })
    }
    else if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_SPRITE)
    {
    console.log("Checking for existsing sprite asset");
      ProjectService.assetHasSprite($scope.currentAsset.uuid,function(result)
      {
        console.log("Sprite asset exists",result);
        $scope.hasSprite = result;
      });
    }
    else if ($scope.currentAsset.type == ProjectService.ASSET_TYPE_FONT)
    {
    console.log("Checking for existsing font asset");
      if ($scope.currentAsset.format == ProjectService.ASSET_FORMAT_FONT_TRUETYPE)
      {
        ProjectService.assetHasFontTtf($scope.currentAsset.uuid,function(result)
        {
          console.log("Font asset exists",result);
          $scope.hasFont = result;
        });
      }
    }
  };

  $scope.modified = function()
  {
    console.log("Asset List Item Modified");
    if ($scope.isPhysicsObjectAssetFormatCompound())
{
      console.log("Getting physics object assets");
      $scope.physicsObjectAssets = ProjectService.getAllPhysicsObjectAssets($scope.currentAsset);
    }
    ProjectService.updateAsset($scope.currentAsset);
    UIService.updateAsset($scope.currentAsset);
  };

  $scope.onAssetModelAssimpUploadButtonClicked = function()
{
    UIService.addAlert("Uploading Asset File...","info");
    var assimpFile = document.getElementById('wf-assimp-file');
    UtilService.readFileAsBinaryFromElement(assimpFile, function(data)
{
      var path = ProjectService.getProjectUUID()+"/asset/model/"+$scope.currentAsset.uuid+"/assimp";
      ApiService.uploadAsset(path,data,function(success)
{
        if (success)
{
          UIService.addAlert("Model Asset Uploaded Successfuly!","success");
          $scope.updateAssetUIVariables();
        }
        else
{
          UIService.addAlert("Error uploading asset.","danger");
        }
      });
    });
  };

  $scope.onAssetModelOtherUploadButtonClicked = function()


{
      var otherFiles = document.getElementById('model-other-file');
      console.log("Other files:",otherFiles);

      UtilService.readFilesFromElementAsync(otherFiles,function (fileInfo)

{
        UIService.addAlert("Uploading "+fileInfo.name+"...","info");
        var path = ProjectService.getProjectUUID()+"/asset/model/"+$scope.currentAsset.uuid+"/"+fileInfo.name;
        ApiService.uploadAsset(path,fileInfo.data,function(success)

{
          if (success)

{
            UIService.addAlert(fileInfo.name+" uploaded successfuly!","success");
            $scope.updateAssetUIVariables();
          }
          else

{
            UIService.addAlert("Error uploading asset.","danger");
          }
      });
    });
  };

  $scope.onAssetPhysicsObjectBvhTriangleMeshUploadButtonClicked = function()
{
    UIService.addAlert("Uploading Asset File...","info");
    var btmFile = document.getElementById('po-btm-file');
    UtilService.readFileAsBinaryFromElement(btmFile, function(data)
{

      var path = ProjectService.getProjectUUID() +
            "/asset/physicsObject/" +
            $scope.currentAsset.uuid +"/" +
            ProjectService.ASSET_FORMAT_PHYSICS_OBJECT_BVH_TRIANGLE_MESH;

      ApiService.uploadAsset(path,data,function(success)
{
        if (success)
{
          UIService.addAlert("Collision Model Asset Uploaded Successfuly!","success");
          $scope.updateAssetUIVariables();
        }
        else
{
          UIService.addAlert("Error uploading collision model asset.","danger");
        }
      });
    });
  };

  $scope.onAssetAudioMusicUploadButtonClicked = function()
{
    UIService.addAlert("Uploading Asset File...","info");
    var musicFile = document.getElementById('music-file');
    UtilService.readFileAsBinaryFromElement(musicFile, function(data)
{
      var path = ProjectService.getProjectUUID()+"/asset/"+$scope.currentAsset.type+"/"+$scope.currentAsset.uuid+"/"+$scope.currentAsset.format;
      ApiService.uploadAsset(path,data, function(success)
{
        if (success)
{
          UIService.addAlert("Music Asset uploaded successfuly.","success");
          $scope.updateAssetUIVariables();
        }
        else
{
          UIService.addAlert("Error uploading music asset.","danger");
        }
      });
    });
  };

  $scope.onAssetAudioSfxUploadButtonClicked = function()
{
    UIService.addAlert("Uploading Asset File...","info");
    var sfxFile = document.getElementById('sfx-file');
    UtilService.readFileAsBinaryFromElement(sfxFile, function(data)
{
      var path = ProjectService.getProjectUUID()+"/asset/"+$scope.currentAsset.type+"/"+$scope.currentAsset.uuid+"/"+$scope.currentAsset.format;
      ApiService.uploadAsset(path,data, function(success)
{
        if (success)
{
          UIService.addAlert("Sfx asset uploaded successfuly.","success");
          $scope.updateAssetUIVariables();
        }
        else
{
          UIService.addAlert("Error uploading sfx asset.","danger");
        }
      });
    });
  };

  $scope.onAssetSpriteUploadButtonClicked = function()
{
    UIService.addAlert("Uploading Asset File...","info");
    var spriteFile = document.getElementById('sprite-file');
    UtilService.readFileAsBinaryFromElement(spriteFile, function(data)
{
      var path = ProjectService.getProjectUUID()+"/asset/"+$scope.currentAsset.type+"/"+$scope.currentAsset.uuid+"/sprite";
      ApiService.uploadAsset(path,data, function(success)
{
        if (success)
{
          UIService.addAlert("Asset uploaded successfuly.","success");
          $scope.updateAssetUIVariables();
        }
        else
{
          UIService.addAlert("Error uploading asset.","danger");
        }
      });
    });
  };

  $scope.onAssetFontUploadButtonClicked = function()
{
    UIService.addAlert("Uploading Asset File...","info");
    var spriteFile = document.getElementById('font-file');
    UtilService.readFileAsBinaryFromElement(spriteFile, function(data)
{
      var path = ProjectService.getProjectUUID()+"/asset/"+$scope.currentAsset.type+"/"+$scope.currentAsset.uuid+"/"+$scope.currentAsset.format;
      ApiService.uploadAsset(path,data, function(success)
{
        if (success)
{
          UIService.addAlert("Asset uploaded successfuly.","success");
          $scope.updateAssetUIVariables();
        }
        else
{
          UIService.addAlert("Error uploading asset.","danger");
        }
      });
    });
  };

  $scope.onDeleteAssetDataButtonClicked = function()
{
    console.log("Deleting asset data for",$scope.currentAsset.name,"("+$scope.currentAsset.uuid+")");
    ProjectService.deleteAssetData($scope.currentAsset,function(success)
{
      if (success)
{
        UIService.addAlert("Asset Data Deleted", "success");
        $scope.updateAssetUIVariables();
      } else
{
        UIService.addAlert("Unable to Remove Asset Data", "succes");
      }
    });
  }

  $scope.onScriptTemplateSelected = function()
{
        console.log("Writing Script Template",$scope.selectedScriptTemplate);
        UIService.addAlert("Writing Script Template","info");

        ApiService.getTemplateData(ProjectService.ASSET_TYPE_SCRIPT,$scope.selectedScriptTemplate,"lua", function(luaData)
{
            var luaPath = ProjectService.getProjectUUID()+"/asset/script/"+$scope.currentAsset.uuid+"/lua";
            ApiService.uploadAsset(luaPath,luaData,function(success)
{
                if (success)
{
                    UIService.addAlert("Script written successfuly!","success");
                    $scope.updateAssetUIVariables();
                }
                else
{
                    UIService.addAlert("Error writing script.","danger");
                }
            });
        });
  };

  $scope.onShaderTemplateSelected = function()
{
        console.log("Writing template",$scope.selectedShaderTemplate);
        UIService.addAlert("Writing Shader Template","info");

        ApiService.getTemplateData(ProjectService.ASSET_TYPE_SHADER,$scope.selectedShaderTemplate,"vertex", function(vertexData)
{
            var vertexPath = ProjectService.getProjectUUID()+"/asset/shader/"+$scope.currentAsset.uuid+"/vertex";
            ApiService.uploadAsset(vertexPath,vertexData,function(success)
{
                if (success)
{
                    UIService.addAlert("Vertex shader written successfuly!","success");
                    $scope.updateAssetUIVariables();
                }
                else
{
                    UIService.addAlert("Error writing vertex shader.","danger");
                }
            });
        });

        ApiService.getTemplateData(ProjectService.ASSET_TYPE_SHADER,$scope.selectedShaderTemplate,"fragment", function(fragData)
{
            var fragPath = ProjectService.getProjectUUID()+"/asset/shader/"+$scope.currentAsset.uuid+"/fragment";
            ApiService.uploadAsset(fragPath,fragData,function(success)
{
                if (success)
{
                    UIService.addAlert("Fragment shader written successfuly!","success");
                    $scope.updateAssetUIVariables();
                }
                else
{
                    UIService.addAlert("Error writing fragment shader.","danger");
                }
            });
        });
  };

  $scope.onAssetPhysicsObjectCompoundAddChildButtonClicked = function()
{
    if (!$scope.currentAsset.compoundChildren)
{
      console.log("Creating compound child array for",$scope.currentAsset.name,$scope.currentAsset.uuid);
      $scope.currentAsset.compoundChildren = [];
    }
    var uuid = $scope.compoundChildSelectedUuid;
    var asset = ProjectService.getAssetByUUID(uuid,function(asset)
{
      if (asset)
{
        console.log("Adding compound child",asset);
        $scope.currentAsset.compoundChildren.push(

{
            name: asset.name,
            uuid: asset.uuid,
            translation: { x:0, y:0, z:0 },
            rotation:    { x:0, y:0, z:0 }
          }
        );
      }
    });
  };

  $scope.onAssetPhysicsObjectCompoundRemoveChildButtonClicked = function(shape)
{
    var index = $scope.currentAsset.compoundChildren.indexOf(shape);
    console.log("Removing compound child",shape,"at index",index);
    if (index > -1)
{
      $scope.currentAsset.compoundChildren.splice(index,1);
    } else
{
      console.error(shape,"not found in compoundChildren");
    }
  };

  $scope.doOnLoad();
}]);
