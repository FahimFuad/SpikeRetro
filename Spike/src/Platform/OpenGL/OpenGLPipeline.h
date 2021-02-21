/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : OpenGLVertexArray
File Type      : h
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#pragma once
#include "Spike/Renderer/Pipeline.h"

namespace Spike
{
    class OpenGLPipeline : public Pipeline
    {
    public:
        OpenGLPipeline(const PipelineSpecification& spec);
        virtual ~OpenGLPipeline();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const PipelineSpecification& GetSpecification() const override { return m_Specification; }
    private:
        RendererID m_RendererID;
        uint32_t m_VertexBufferIndex = 0;
        PipelineSpecification m_Specification;
    };
}