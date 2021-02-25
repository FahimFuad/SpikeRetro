/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : VertexBuffer
File Type      : cpp
File created on: 2021/01/09
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
#include "spkpch.h"
#include "RendererAPI.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "Platform/DX11/DX11VertexBuffer.h"

namespace Spike
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferLayout layout)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(size, layout);
            case RendererAPI::API::DX11:    return Ref<DX11VertexBuffer>::Create(size, layout);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }


    Ref<VertexBuffer> VertexBuffer::Create(void* vertices, uint32_t size, VertexBufferLayout layout)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    SPK_INTERNAL_ASSERT("RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return Ref<OpenGLVertexBuffer>::Create(vertices, size, layout);
            case RendererAPI::API::DX11:    return Ref<DX11VertexBuffer>::Create(vertices, size, layout);
        }

        SPK_INTERNAL_ASSERT("Unknown RendererAPI!");
        return nullptr;
    }
}