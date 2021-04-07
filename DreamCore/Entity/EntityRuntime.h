/*
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

#pragma once

#include "Common/AssetType.h"
#include "EntityTasks.h"
#include "BoundingBox.h"
#include "Base/Runtime.h"
#include "Components/Graphics/Frustum.h"
#include "Math/Transform.h"

// Runtime Types
#include "Components/Animation/AnimationRuntime.h"
#include "Components/Audio/AudioRuntime.h"
#include "Components/Graphics/Font/FontRuntime.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Path/PathRuntime.h"
#include "Components/Physics/PhysicsRuntime.h"
#include "Components/Script/ScriptRuntime.h"
#include "Components/Graphics/Texture/TextureRuntime.h"

#include <optional>
#include <vector>
#include <memory>
#include <map>

using std::vector;
using std::function;
using std::shared_ptr;
using std::unique_ptr;
using std::map;
using nlohmann::json;
using std::reference_wrapper;
using std::optional;

namespace octronic::dream
{
  class Event;
  class SceneEntityDefinition;
  class TemplateEntityDefinition;
  class SceneRuntime;

  class AssetDefinition;
  class AnimationDefinition;
  class AudioDefinition;
  class FontDefinition;
  class ModelDefinition;
  class PathDefinition;
  class PhysicsDefinition;
  class ScriptDefinition;
  class TextureDefinition;

  class EntityRuntime : public Runtime
  {
  public:
    EntityRuntime(ProjectRuntime&,
                  SceneRuntime&,
                  SceneEntityDefinition&,
                  TemplateEntityDefinition&);

    EntityRuntime(EntityRuntime&&) = default;
    EntityRuntime& operator=(EntityRuntime&&) = default;

    void collectGarbage();

    SceneRuntime& getSceneRuntime();
    SceneEntityDefinition& getSceneEntityDefinition();
    TemplateEntityDefinition& getTemplateEntityDefinition();

    bool createAssetRuntimes(map<AssetType,UuidType>);
    bool createAnimationRuntime(AnimationDefinition&);
    bool createPathRuntime(PathDefinition&);
    bool createAudioRuntime(AudioDefinition&);
    bool createModelRuntime(ModelDefinition&);
    bool createScriptRuntime(ScriptDefinition&);
    bool createPhysicsRuntime(PhysicsDefinition&);
    bool createFontRuntime(FontDefinition&);
    bool createTextureRuntime(TextureDefinition&);

    AssetRuntime& getAssetRuntime(AssetType type);
    // Discrete Assets
    AnimationRuntime& getAnimationRuntime();
    PathRuntime&  getPathRuntime();
    PhysicsRuntime& getPhysicsRuntime();
    // Shared Assets
    FontRuntime& getFontRuntime() const;
    AudioRuntime& getAudioRuntime() const;
    ModelRuntime& getModelRuntime() const;
    ScriptRuntime& getScriptRuntime() const;
    TextureRuntime& getTextureRuntime() const;

    bool hasAnimationRuntime() const;
    bool hasPathRuntime() const;
    bool hasAudioRuntime() const;
    bool hasModelRuntime() const;
    bool hasScriptRuntime() const;
    bool hasPhysicsRuntime() const;
    bool hasFontRuntime() const;
    bool hasTextureRuntime() const;

    Transform getTransform() const;
    Transform getInitialTransform() const;
    void setTransform(const Transform& transform);

    bool hasEvents() const;
    void addEvent(const Event& event);
    vector<Event> getEventQueue();
    void clearProcessedEvents();

    EntityRuntime& getChildRuntimeByUuid(UuidType uuid);
    int countAllChildren() const;
    size_t countChildren() const;
    void removeChildRuntime(EntityRuntime&);
    EntityRuntime& createChildRuntime(SceneEntityDefinition&);
    vector<reference_wrapper<EntityRuntime>> getChildRuntimes() const;
    bool isChildOf(EntityRuntime&) const;

    void setScriptInitialised(bool i);
    bool getScriptInitialised() const;

    void setScriptError(bool i);
    bool getScriptError() const;

    bool isParentOf(EntityRuntime& child) const;
    void setParentEntityRuntime(EntityRuntime& parent);
    EntityRuntime& getParentEntityRuntime();
    vector<reference_wrapper<EntityRuntime>> generateFlatVector();

    bool loadFromDefinition() override;

    bool getDeleted() const;
    void setDeleted(bool deleted);

    void removeAnimationRuntime();
    void removeAudioRuntime();
    void removePathRuntime();
    void removeModelRuntime();
    void removeScriptRuntime();
    void removePhysicsRuntime();
    void removeFontRuntime();
    void removeTextureRuntime();

    BoundingBox getBoundingBox() const;
    void setBoundingBox(const BoundingBox& boundingBox);

    float distanceFrom(EntityRuntime& other) const;
    float distanceFrom(const vec3& other) const;
    bool visibleInFrustum();
    bool containedInFrustum();

    void initTransform();

    shared_ptr<EntityScriptCreateStateTask> getScriptCreateStateTask() const;
    shared_ptr<EntityScriptOnInitTask> getScriptOnInitTask() const;
    shared_ptr<EntityScriptOnEventTask> getScriptOnEventTask() const;
    shared_ptr<EntityScriptOnUpdateTask> getScriptOnUpdateTask() const;

    string getAttribute(const string& key) const;
    void setAttribute(const string& key, const string& value);
    const map<string,string>& getAttributesMap() const;

    string getFontText() const;
    void setFontText(const string& fontText);

    vec4 getFontColor() const;
    void setFontColor(const vec4& fontColor);

    float getFontScale() const;
    void setFontScale(float fontScale);

    void pushTasks();
    bool allRuntimesLoaded() const;
    ProjectRuntime& getProjectRuntime() const;

  protected:
    reference_wrapper<ProjectRuntime> mProjectRuntime;
    reference_wrapper<SceneRuntime> mSceneRuntime;
    reference_wrapper<TemplateEntityDefinition> mTemplateEntityDefinition;
    optional<reference_wrapper<EntityRuntime>> mParentEntityRuntime;
    vector<unique_ptr<EntityRuntime>> mChildRuntimes;

    // Discrete Assets =====================================================
    optional<AnimationRuntime> mAnimationRuntime;
    optional<PathRuntime> mPathRuntime;
    optional<PhysicsRuntime> mPhysicsRuntime;

    // Shared Runtimes =====================================================
    // Audio
    optional<reference_wrapper<AudioRuntime>> mAudioRuntime;

    // Font
    optional<reference_wrapper<FontRuntime>> mFontRuntime;
    string mFontText;
    vec4 mFontColor;
    float mFontScale;

    // Model
    optional<reference_wrapper<ModelRuntime>> mModelRuntime;

    // Script
    optional<reference_wrapper<ScriptRuntime>> mScriptRuntime;
    bool mScriptError;
    bool mScriptInitialised;
    shared_ptr<EntityScriptCreateStateTask> mScriptCreateStateTask;
    shared_ptr<EntityScriptOnInitTask> mScriptOnInitTask;
    shared_ptr<EntityScriptOnUpdateTask> mScriptOnUpdateTask;
    shared_ptr<EntityScriptOnEventTask> mScriptOnEventTask;
    shared_ptr<EntityScriptRemoveStateTask> mScriptRemoveStateTask;

    // Sprite Texture
    optional<reference_wrapper<TextureRuntime>> mTextureRuntime;



    // Transform
    Transform mInitialTransform;
    Transform mCurrentTransform;
    BoundingBox mBoundingBox;

    // Flags
    bool mDeleted;
    vector<Event> mEventQueue;
    map<string,string> mAttributes;
  };
}
