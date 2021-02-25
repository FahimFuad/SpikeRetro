/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : DX11Pipeline
File Type      : h
File created on: 2021/02/12
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.

2. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/Pipeline.h"
#include <d3d11.h>

namespace Spike
{
    class DX11Pipeline : public Pipeline
    {
    public:
        DX11Pipeline(const PipelineSpecification& spec);
        virtual ~DX11Pipeline();
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const PipelineSpecification& GetSpecification() const override { return m_Spec; }
        virtual void SetPrimitiveTopology(PrimitiveTopology topology) override { m_PrimitiveTopology = topology; }
    private:
        ID3D11InputLayout* m_InputLayout = nullptr;
        PipelineSpecification m_Spec;
        PrimitiveTopology m_PrimitiveTopology = PrimitiveTopology::TRIANGLELIST;
    };
}
