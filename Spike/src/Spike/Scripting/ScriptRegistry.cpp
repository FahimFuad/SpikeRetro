//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "ScriptRegistry.h"
#include "ScriptCalls.h"
#include "Spike/Scene/Entity.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

/* The Purpose of this file is to register what response should be given
 * from C++ side when the user makes a C# call. Like the Spike::Scripting::Spike_LogInfo
 * is called when the user writes Spike.Console::LogInfo in their C# code
 * and executes this Spike::Scripting::Spike_LogInfo function under the hood */

namespace Spike
{
    std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
    std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;

    extern MonoImage* s_CoreAssemblyImage;

    #define Component_RegisterType(Type) \
    {\
        MonoType* type = mono_reflection_type_from_name("Spike." #Type, s_CoreAssemblyImage);\
        if (type) {\
            Uint id = mono_type_get_type(type);\
            s_HasComponentFuncs[type] = [](Entity& entity) { return entity.HasComponent<Type>(); };\
            s_CreateComponentFuncs[type] = [](Entity& entity) { entity.AddComponent<Type>(); };\
        } else {\
            SPK_CORE_LOG_ERROR("No C# component class found for " #Type "!");\
        }\
    }

    static void InitComponent()
    {
        Component_RegisterType(TagComponent);
        Component_RegisterType(TransformComponent);
        Component_RegisterType(CameraComponent);
        Component_RegisterType(SpriteRendererComponent);
        Component_RegisterType(RigidBody2DComponent);
    }

    void ScriptRegistry::RegisterAll()
    {
        InitComponent();

        /* [Spike] Console [Spike] */
        mono_add_internal_call("Spike.Console::LogInfo_Native",  Spike::Scripting::Spike_Console_LogInfo);
        mono_add_internal_call("Spike.Console::LogWarn_Native",  Spike::Scripting::Spike_Console_LogWarn);
        mono_add_internal_call("Spike.Console::LogDebug_Native", Spike::Scripting::Spike_Console_LogDebug);
        mono_add_internal_call("Spike.Console::LogError_Native", Spike::Scripting::Spike_Console_LogError);
        mono_add_internal_call("Spike.Console::LogFatal_Native", Spike::Scripting::Spike_Console_LogCritical);

        /* [Spike] Input [Spike] */
        mono_add_internal_call("Spike.Input::IsKeyPressed_Native",         Spike::Scripting::Spike_Input_IsKeyPressed);
        mono_add_internal_call("Spike.Input::IsMouseButtonPressed_Native", Spike::Scripting::Spike_Input_IsMouseButtonPressed);
        mono_add_internal_call("Spike.Input::GetMousePosition_Native",     Spike::Scripting::Spike_Input_GetMousePosition);
        mono_add_internal_call("Spike.Input::SetCursorMode_Native",        Spike::Scripting::Spike_Input_SetCursorMode);
        mono_add_internal_call("Spike.Input::GetCursorMode_Native",        Spike::Scripting::Spike_Input_GetCursorMode);

        /* [Spike] Entity [Spike] */
        mono_add_internal_call("Spike.Entity::CreateComponent_Native", Spike::Scripting::Spike_Entity_CreateComponent);
        mono_add_internal_call("Spike.Entity::HasComponent_Native",    Spike::Scripting::Spike_Entity_HasComponent);
        mono_add_internal_call("Spike.Entity::FindEntityByTag_Native", Spike::Scripting::Spike_Entity_FindEntityByTag);

        /* [Spike] Tag Component [Spike] */
        mono_add_internal_call("Spike.TagComponent::GetTag_Native", Spike::Scripting::Spike_TagComponent_GetTag);
        mono_add_internal_call("Spike.TagComponent::SetTag_Native", Spike::Scripting::Spike_TagComponent_SetTag);

        /* [Spike] Transform Component [Spike] */
        mono_add_internal_call("Spike.TransformComponent::GetTransform_Native",   Spike::Scripting::Spike_TransformComponent_GetTransform);
        mono_add_internal_call("Spike.TransformComponent::SetTransform_Native",   Spike::Scripting::Spike_TransformComponent_SetTransform);
        mono_add_internal_call("Spike.TransformComponent::GetTranslation_Native", Spike::Scripting::Spike_TransformComponent_GetTranslation);
        mono_add_internal_call("Spike.TransformComponent::SetTranslation_Native", Spike::Scripting::Spike_TransformComponent_SetTranslation);
        mono_add_internal_call("Spike.TransformComponent::GetRotation_Native",    Spike::Scripting::Spike_TransformComponent_GetRotation);
        mono_add_internal_call("Spike.TransformComponent::SetRotation_Native",    Spike::Scripting::Spike_TransformComponent_SetRotation);
        mono_add_internal_call("Spike.TransformComponent::GetScale_Native",       Spike::Scripting::Spike_TransformComponent_GetScale);
        mono_add_internal_call("Spike.TransformComponent::SetScale_Native",       Spike::Scripting::Spike_TransformComponent_SetScale);

        /* [Spike] RigidBody2D Component [Spike] */
        mono_add_internal_call("Spike.RigidBody2DComponent::ApplyLinearImpulse_Native", Spike::Scripting::Spike_RigidBody2DComponent_ApplyLinearImpulse);
        mono_add_internal_call("Spike.RigidBody2DComponent::GetLinearVelocity_Native",  Spike::Scripting::Spike_RigidBody2DComponent_GetLinearVelocity);
        mono_add_internal_call("Spike.RigidBody2DComponent::SetLinearVelocity_Native",  Spike::Scripting::Spike_RigidBody2DComponent_SetLinearVelocity);

        /* [Spike] SpriteRenderer Component [Spike] */
        mono_add_internal_call("Spike.SpriteRendererComponent::GetColor_Native", Spike::Scripting::Spike_SpriteRendererComponent_GetColor);
        mono_add_internal_call("Spike.SpriteRendererComponent::SetColor_Native", Spike::Scripting::Spike_SpriteRendererComponent_SetColor);

        /* [Spike] Camera Component [Spike] */
        mono_add_internal_call("Spike.CameraComponent::IsPrimary_Native", Spike::Scripting::Spike_CameraComponent_IsPrimary);
        mono_add_internal_call("Spike.CameraComponent::SetAsPrimary_Native", Spike::Scripting::Spike_CameraComponent_SetAsPrimary);
        mono_add_internal_call("Spike.CameraComponent::IsFixedAspectRatio_Native", Spike::Scripting::Spike_CameraComponent_IsFixedAspectRatio);
        mono_add_internal_call("Spike.CameraComponent::SetFixedAspectRatio_Native", Spike::Scripting::Spike_CameraComponent_SetFixedAspectRatio);
    }
}
