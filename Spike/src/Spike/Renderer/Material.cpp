/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Material
File Type      : cpp
File created on: 2021/03/04
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Material.h"

namespace Spike
{

    Ref<Material> Material::Create(const Ref<Shader>& shader)
    {
        return Ref<Material>::Create(shader);
    }

    Material::Material(const Ref<Shader>& shader)
        :m_Shader(shader)
    {

    }

    Material::~Material()
    {

    }

    void Material::Bind(uint32_t index)
    {
        m_Shader->Bind();
        for (size_t i = 0; i < m_Textures.size(); i++)
        {
            auto& texture = m_Textures[index];
            if (texture)
                texture->Bind(i);
        }
        m_Shader->SetInt("u_Texture", index); //TODO: automate
    }

    void Material::PushTexture(const Ref<Texture2D>& tex, uint32_t slot)
    {
         m_Textures[slot] = tex;
    }
}