/*
                               SPIKE ENGINE
               This file is licensed under the SPIKE LICENSE
                          --Spike Source Code--

         Copyright 2021 - SpikeTechnologies - All Rights Reserved

File Name      : Mesh
File Type      : cpp
File created on: 2021/01/09
File created by: Fahim Fuad
Other editors  : None
Github repository : https://github.com/FahimFuad/Spike
 
0.This software is provided 'AS-IS', without any express or implied warranty.
  In no event will the authors or contributors be held liable for any damages
  arising from the use of this software.

1.The origin of this software must not be misrepresented; you must not claim/
  that you wrote the original software.
 
2.You MUST NOT change or alter this file. This excludes the contributions done
  by people. Changing this file is PERFECTLY LEGAL if you are contributing.

3. THIS NOTICE MAY NOT BE REMOVED OR ALTERED FROM ANY SOURCE DISTRIBUTION.
*/
#include "spkpch.h"
#include "Spike/Renderer/Mesh.h"
#include "Texture.h"
#include <glad/glad.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

namespace Spike
{
    static const uint32_t s_MeshImportFlags =
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_OptimizeMeshes |
        aiProcess_FlipUVs;

    struct LogStream : public Assimp::LogStream
    {
        static void Initialize()
        {
            if (Assimp::DefaultLogger::isNullLogger())
            {
                Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
                Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
            }
        }

        virtual void write(const char* message) override
        {
            SPK_CORE_LOG_WARN("Assimp: {0}", message);
        }
    };

    Mesh::Mesh(const std::string& filepath)
        : m_FilePath(filepath)
    {
        Generate(filepath);
    }

    Mesh::Mesh(const std::string& filepath, uint32_t entityID)
        :m_FilePath(filepath), m_ObjectID(entityID)
    {
        Generate(filepath, entityID);
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::Generate(const std::string& filepath, uint32_t entityID)
    {
        LogStream::Initialize();

        SPK_CORE_LOG_INFO("Loading mesh: {0}", filepath.c_str());
        m_Importer = CreateScope<Assimp::Importer>();

        const aiScene* scene = m_Importer->ReadFile(filepath, s_MeshImportFlags);
        if (!scene || !scene->HasMeshes())
            SPK_CORE_LOG_ERROR("Failed to load mesh file: {0}", filepath);

        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;

        m_Submeshes.reserve(scene->mNumMeshes);
        for (size_t m = 0; m < scene->mNumMeshes; m++)
        {
            aiMesh* mesh = scene->mMeshes[m];

            Submesh& submesh = m_Submeshes.emplace_back();
            submesh.BaseVertex = vertexCount;
            submesh.BaseIndex = indexCount;
            //submesh.MaterialIndex = mesh->mMaterialIndex;
            submesh.IndexCount = mesh->mNumFaces * 3;
            submesh.MeshName = mesh->mName.C_Str();

            vertexCount += mesh->mNumVertices;
            indexCount += submesh.IndexCount;

            SPK_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
            SPK_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

            // Extract vertices from mesh
            m_Vertices.reserve(mesh->mNumVertices);
            for (size_t i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
                vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
                vertex.ObjectID = entityID;

                if (mesh->HasTextureCoords(0))
                    vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                m_Vertices.push_back(vertex);
            }

            // Extract indices from mesh
            m_Indices.reserve(mesh->mNumFaces);
            for (uint32_t i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (uint32_t j = 0; j < face.mNumIndices; j++)
                    m_Indices.push_back(face.mIndices[j]);
            }
        }

        m_MeshShader = Shader::Create("Spike-Editor/assets/shaders/MeshShader.glsl");

        VertexBufferLayout layout =
        {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float3, "a_Normal" },
            { ShaderDataType::Float2, "a_TexCoord" },
            { ShaderDataType::Int, "a_ObjectID" }
        };

        m_VertexArray = VertexArray::Create();

        m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
        m_VertexBuffer->SetLayout(layout);

        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), std::size(m_Indices));

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        m_VertexArray->Bind();
        //DumpVertexBuffer();
    }

    void Mesh::DumpVertexBuffer()
    {
        SPK_CORE_LOG_INFO("------------------------------------------------------");
        SPK_CORE_LOG_INFO("Vertex Buffer Dump");
        SPK_CORE_LOG_INFO("Mesh: {0}", m_FilePath);

        for (size_t i = 0; i < m_Vertices.size(); i++)
        {
            auto& vertex = m_Vertices[i];
            SPK_CORE_LOG_INFO("Vertex: {0}", i);
            SPK_CORE_LOG_INFO("Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z);
            SPK_CORE_LOG_INFO("Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
            //SPK_CORE_LOG_INFO("Binormal: {0}, {1}, {2}", vertex.Binormal.x, vertex.Binormal.y, vertex.Binormal.z);
            //SPK_CORE_LOG_INFO("Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z);
            SPK_CORE_LOG_INFO("TexCoord: {0}, {1}", vertex.Texcoord.x, vertex.Texcoord.y);
            SPK_CORE_LOG_INFO("--");
        }

        SPK_CORE_LOG_INFO("------------------------------------------------------");
    }
}