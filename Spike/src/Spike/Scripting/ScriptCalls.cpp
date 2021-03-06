//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "Spike/Scene/Entity.h"
#include "ScriptCalls.h"
#include "Panels/ConsolePanel.h"
#include "ScriptEngine.h"
#include "ScriptUtils.h"
#include <box2d/box2d.h>
#include <mono/jit/jit.h>

namespace Spike
{
    extern std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
    extern std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;
}

namespace Spike::Scripting
{ 
    void Spike_Console_LogInfo(MonoObject* message)
    {
        char* msg = CovertMonoObjectToCppChar(message);
        SPK_CORE_LOG_INFO(msg);
    }
    void Spike_Console_LogWarn(MonoObject* message)
    {
        char* msg = CovertMonoObjectToCppChar(message);
        SPK_CORE_LOG_WARN(msg);
    }
    void Spike_Console_LogDebug(MonoObject* message)
    {
        char* msg = CovertMonoObjectToCppChar(message);
        SPK_CORE_LOG_DEBUG(msg); 
    }
    void Spike_Console_LogError(MonoObject* message)
    {
        char* msg = CovertMonoObjectToCppChar(message);
        SPK_CORE_LOG_ERROR(msg);
    }
    void Spike_Console_LogCritical(MonoObject* message) {
        char* msg = CovertMonoObjectToCppChar(message);
        SPK_CORE_LOG_CRITICAL(msg);
    }

    /* [Spike] INPUT [Spike] */
    bool Spike_Input_IsKeyPressed(KeyCode key) { return Spike::Input::IsKeyPressed(key);}
    bool Spike_Input_IsMouseButtonPressed(MouseCode button) { return Spike::Input::IsMouseButtonPressed(button);}
    void Spike_Input_SetCursorMode(MousePointerMode mode) { Input::SetCursorMode(mode); }
    MousePointerMode Spike_Input_GetCursorMode() { return Spike::Input::GetCursorMode(); }

    void Spike_Input_GetMousePosition(glm::vec2* outPosition)
    {
        glm::vec2 result =  Spike::Input::GetMousePosition();
        *outPosition = result;
    }

    /* [Spike] ENTITY [Spike] */
    void Spike_Entity_CreateComponent(uint64_t entityID, void* type)
    {
        auto& entityMap = ValidateSceneAndReturnEntityMap(ScriptEngine::GetSceneContext(), entityID);
        Entity entity = entityMap.at(entityID);
        MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
        s_CreateComponentFuncs[monoType](entity);
    }

    bool Spike_Entity_HasComponent(uint64_t entityID, void* type)
    {
        auto& entityMap = ValidateSceneAndReturnEntityMap(ScriptEngine::GetSceneContext(), entityID);
        Entity entity = entityMap.at(entityID);
        MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
        bool result = s_HasComponentFuncs[monoType](entity);
        return result;
    }

    uint64_t Spike_Entity_FindEntityByTag(MonoString* tag)
    {
        Ref<Scene> scene = ScriptEngine::GetSceneContext();
        SPK_CORE_ASSERT(scene, "No active scene!");

        Entity entity = scene->FindEntityByTag(ConvertMonoStringToCppString(tag));
        if (entity)
            return entity.GetComponent<IDComponent>().ID;

        return 0;
    }

    /* [Spike] TAG COMPONENT [Spike] */
    MonoString* Spike_TagComponent_GetTag(uint64_t entityID)
    {
        auto& component = ValidateSceneAndReturnAComponent<TagComponent>(ScriptEngine::GetSceneContext(), entityID);
        std::string tag = component.Tag;
        return ConvertCppStringToMonoString(mono_domain_get(), tag.c_str());
    }

    void Spike_TagComponent_SetTag(uint64_t entityID, MonoString* inTag)
    {
        auto& component = ValidateSceneAndReturnAComponent<TagComponent>(ScriptEngine::GetSceneContext(), entityID);
        std::string& tag = ConvertMonoStringToCppString(inTag);
        component.Tag = tag;
    }

    /* [Spike] TRANSFORM COMPONENT [Spike] */
    void Spike_TransformComponent_GetTransform(uint64_t entityID, TransformComponent* outTransform)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        *outTransform = component;
    }

    void Spike_TransformComponent_SetTransform(uint64_t entityID, TransformComponent* inTransform)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        component = *inTransform;
    }

    void Spike_TransformComponent_GetTranslation(uint64_t entityID, glm::vec3* outTranslation)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        *outTranslation = component.Translation;
    }

    void Spike_TransformComponent_SetTranslation(uint64_t entityID, glm::vec3* inTranslation)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        component.Translation = *inTranslation;
    }

    void Spike_TransformComponent_GetRotation(uint64_t entityID, glm::vec3* outRotation)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        *outRotation = component.Rotation;
    }

    void Spike_TransformComponent_SetRotation(uint64_t entityID, glm::vec3* inRotation)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        component.Rotation = *inRotation;
    }

    void Spike_TransformComponent_GetScale(uint64_t entityID, glm::vec3* outScale)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        *outScale = component.Scale;
    }

    void Spike_TransformComponent_SetScale(uint64_t entityID, glm::vec3* inScale)
    {
        auto& component = ValidateSceneAndReturnAComponent<TransformComponent>(ScriptEngine::GetSceneContext(), entityID);
        component.Scale = *inScale;
    }

    /* [Spike] RIGIDBODY2D COMPONENT [Spike] */
    void Spike_RigidBody2DComponent_ApplyLinearImpulse(uint64_t entityID, glm::vec2* impulse, glm::vec2* offset, bool wake)
    {
        auto& component = ValidateSceneAndReturnAComponent<RigidBody2DComponent>(ScriptEngine::GetSceneContext(), entityID);
        b2Body* body = (b2Body*)component.RuntimeBody;
        body->ApplyLinearImpulse(*(const b2Vec2*)impulse, body->GetWorldCenter() + *(const b2Vec2*)offset, wake);
    }

    void Spike_RigidBody2DComponent_GetLinearVelocity(uint64_t entityID, glm::vec2* outVelocity)
    {
        auto& component = ValidateSceneAndReturnAComponent<RigidBody2DComponent>(ScriptEngine::GetSceneContext(), entityID);
        b2Body* body = (b2Body*)component.RuntimeBody;
        const auto& velocity = body->GetLinearVelocity();
        SPK_CORE_ASSERT(outVelocity, "OutVelocity is NULL!");
        *outVelocity = { velocity.x, velocity.y };
    }

    void Spike_RigidBody2DComponent_SetLinearVelocity(uint64_t entityID, glm::vec2* velocity)
    {
        auto& component = ValidateSceneAndReturnAComponent<RigidBody2DComponent>(ScriptEngine::GetSceneContext(), entityID);
        b2Body* body = (b2Body*)component.RuntimeBody;
        SPK_CORE_ASSERT(velocity, "InVelocity is NULL!");
        body->SetLinearVelocity({ velocity->x, velocity->y });
    }

    /* [Spike] SPRITE RENDERER COMPONENT [Spike] */
    void Spike_SpriteRendererComponent_GetColor(uint64_t entityID, glm::vec4* outColor)
    {
        auto& component = ValidateSceneAndReturnAComponent<SpriteRendererComponent>(ScriptEngine::GetSceneContext(), entityID);
        *outColor = component.Color;
    }

    void Spike_SpriteRendererComponent_SetColor(uint64_t entityID, glm::vec4* inColor)
    {
        auto& component = ValidateSceneAndReturnAComponent<SpriteRendererComponent>(ScriptEngine::GetSceneContext(), entityID);
        component.Color = *inColor;
    }

    /* [Spike] CAMERA COMPONENT [Spike] */
    void Spike_CameraComponent_SetAsPrimary(uint64_t entityID, bool* isPrimary)
    {
        auto& component = ValidateSceneAndReturnAComponent<CameraComponent>(ScriptEngine::GetSceneContext(), entityID);
        component.Primary = isPrimary;
    }

    bool Spike_CameraComponent_IsPrimary(uint64_t entityID)
    {
        auto& component = ValidateSceneAndReturnAComponent<CameraComponent>(ScriptEngine::GetSceneContext(), entityID);
        return component.Primary;
    }

    void Spike_CameraComponent_SetFixedAspectRatio(uint64_t entityID, bool isAspectRatioFixed)
    {
        auto& component = ValidateSceneAndReturnAComponent<CameraComponent>(ScriptEngine::GetSceneContext(), entityID);
        component.FixedAspectRatio = isAspectRatioFixed;
    }

    bool Spike_CameraComponent_IsFixedAspectRatio(uint64_t entityID)
    {
        auto& component = ValidateSceneAndReturnAComponent<CameraComponent>(ScriptEngine::GetSceneContext(), entityID);
        return component.FixedAspectRatio;
    }

}
