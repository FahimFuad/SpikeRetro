//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved

#include "MaterialPanel.h"
#include "UIUtils/UIUtils.h"
#include "Spike/Utility/FileDialogs.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Spike
{
    void MaterialPanel::OnImGuiRender(bool* show, Entity& selectedEntity)
    {
        ImGui::Begin("Material Inspector", show);

        if (selectedEntity && selectedEntity.HasComponent<MeshComponent>())
        {
            auto& mesh = selectedEntity.GetComponent<MeshComponent>().Mesh;
            if (mesh)
            {
                auto material = mesh->GetMaterial();
                ImGui::TextColored(ImVec4(0.1f, 0.9f, 0.1f, 1.0f), "Shader: %s", material->GetShader()->GetName().c_str());
                ImGui::Separator();
                GUI::DrawColorControl3("Color", material->m_Color, 150.0f);
                GUI::DrawFloatControl("Shininess", &material->m_Shininess, 150.0f);
                GUI::DrawBoolControl("Use Diffuse Texture", &material->m_AlbedoTexToggle, 150.0f);
                if (GUI::DrawBoolControl("Flip Texture", &material->m_Flipped, 150.0f))
                    material->FlipTextures(material->m_Flipped);
            }
        }
        ImGui::End();
    }
}
