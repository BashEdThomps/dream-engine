/*
 * IScriptComponent
 *
 * Copyright 2016 Octronic. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#include "IScriptComponent.h"

#include <sstream>

#include "ScriptInstance.h"

#include "../Event.h"
#include "../Transform3D.h"
#include "../Time.h"
#include "../Path/PathComponent.h"
#include "../Path/PathInstance.h"
#include "../Audio/AudioComponent.h"
#include "../Audio/AudioInstance.h"
#include "../Graphics/Model/ModelInstance.h"
#include "../Graphics/Camera.h"
#include "../Graphics/GraphicsComponent.h"
#include "../Graphics/Light/LightInstance.h"
#include "../Graphics/Shader/ShaderInstance.h"
#include "../Input/InputComponent.h"
#include "../Physics/PhysicsComponent.h"
#include "../Physics/PhysicsObjectInstance.h"
#include "../Window/IWindowComponent.h"
#include "../../Project/ProjectRuntime.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Utilities/Math.h"


namespace Dream
{
    IScriptComponent::IScriptComponent
    (ProjectRuntime* project, ScriptCache* cache)
        : IComponent(),
          mScriptCache(cache),
          mProjectRuntime(project)
    {
        setLogClassName("IScriptComponent");
        auto log = getLog();
        log->trace( "Constructing Object" );
    }

    IScriptComponent::~IScriptComponent
    ()
    {
        auto log = getLog();
        log->trace( "Destroying Object" );
        mScriptMap.clear();
    }

} // End of Dream

