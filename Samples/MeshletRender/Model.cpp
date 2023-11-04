//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "Span.h"
#include "Model.h"

#include <fstream>
#include <unordered_set>

using namespace DirectX;

namespace
{
    const uint32_t c_prolog = 'MSHL';

    enum FileVersion
    {
        FILE_VERSION_INITIAL = 0,
        CURRENT_FILE_VERSION = FILE_VERSION_INITIAL
    };

    struct FileHeader
    {
        uint32_t Prolog;
        uint32_t Version;

        uint32_t MeshCount;
        uint32_t AccessorCount;
        uint32_t BufferViewCount;
        uint32_t BufferSize;
    };

    struct MeshHeader
    {
        uint32_t Indices;
        uint32_t IndexSubsets;
        uint32_t Attributes[Attribute::Count];

        uint32_t Meshlets;
        uint32_t MeshletSubsets;
        uint32_t UniqueVertexIndices;
        uint32_t PrimitiveIndices;
        uint32_t CullData;
    };

    struct BufferView
    {
        uint32_t Offset;
        uint32_t Size;
    };

    struct Accessor
    {
        uint32_t BufferView;
        uint32_t Offset;
        uint32_t Size;
        uint32_t Stride;
        uint32_t Count;
    };

    template <typename T, typename U>
    constexpr T DivRoundUp(T num, U denom)
    {
        return (num + denom - 1) / denom;
    }
}

bool Model::LoadFromFile(const char* filename)
{
    std::ifstream stream(filename, std::ios::binary);
    if (!stream.is_open())
    {
        return false;
    }

    std::vector<MeshHeader> meshes;
    std::vector<BufferView> bufferViews;
    std::vector<Accessor> accessors;

    FileHeader header;
    stream.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.Prolog != c_prolog)
    {
        return false; // Incorrect file format.
    }

    if (header.Version != CURRENT_FILE_VERSION)
    {
        return false; // Version mismatch between export and import serialization code.
    }

    // Read mesh metdata
    meshes.resize(header.MeshCount);
    stream.read(reinterpret_cast<char*>(meshes.data()), meshes.size() * sizeof(meshes[0]));
    
    accessors.resize(header.AccessorCount);
    stream.read(reinterpret_cast<char*>(accessors.data()), accessors.size() * sizeof(accessors[0]));

    bufferViews.resize(header.BufferViewCount);
    stream.read(reinterpret_cast<char*>(bufferViews.data()), bufferViews.size() * sizeof(bufferViews[0]));

    m_buffer.resize(header.BufferSize);
    stream.read(reinterpret_cast<char*>(m_buffer.data()), header.BufferSize);

    char eofbyte;
    stream.read(&eofbyte, 1); // Read last byte to hit the eof bit

    assert(stream.eof()); // There's a problem if we didn't completely consume the file contents.

    stream.close();

    // Populate mesh data from binary data and metadata.
    m_meshes.resize(meshes.size());
    for (uint32_t i = 0; i < static_cast<uint32_t>(meshes.size()); ++i)
    {
        auto& meshView = meshes[i];
        auto& mesh = m_meshes[i];

        // Index data
        {
            Accessor& accessor = accessors[meshView.Indices];
            BufferView& bufferView = bufferViews[accessor.BufferView];

            mesh.IndexSize = accessor.Size;
            mesh.IndexCount = accessor.Count;

            mesh.Indices = MakeSpan(m_buffer.data() + bufferView.Offset, bufferView.Size);
        }

        // Index Subset data
        {
            Accessor& accessor = accessors[meshView.IndexSubsets];
            BufferView& bufferView = bufferViews[accessor.BufferView];

            mesh.IndexSubsets = MakeSpan(reinterpret_cast<Subset*>(m_buffer.data() + bufferView.Offset), accessor.Count);
        }

        // Vertex data & layout metadata

        // Determine the number of unique Buffer Views associated with the vertex attributes & copy vertex buffers.
        std::vector<uint32_t> vbMap;

        for (uint32_t j = 0; j < Attribute::Count; ++j)
        {
            if (meshView.Attributes[j] == -1)
                continue;

            Accessor& accessor = accessors[meshView.Attributes[j]];
            
            auto it = std::find(vbMap.begin(), vbMap.end(), accessor.BufferView);
            if (it != vbMap.end())
            {
                continue; // Already added - continue.
            }

            // New buffer view encountered; add to list and copy vertex data
            vbMap.push_back(accessor.BufferView);
            BufferView& bufferView = bufferViews[accessor.BufferView];

            Span<uint8_t> verts = MakeSpan(m_buffer.data() + bufferView.Offset, bufferView.Size);

            mesh.VertexStrides.push_back(accessor.Stride);
            mesh.Vertices.push_back(verts);
            mesh.VertexCount = static_cast<uint32_t>(verts.size()) / accessor.Stride;
        }

        // Meshlet data
        {
            Accessor& accessor = accessors[meshView.Meshlets];
            BufferView& bufferView = bufferViews[accessor.BufferView];

            mesh.Meshlets = MakeSpan(reinterpret_cast<Meshlet*>(m_buffer.data() + bufferView.Offset), accessor.Count);
        }

        // Meshlet Subset data
        {
            Accessor& accessor = accessors[meshView.MeshletSubsets];
            BufferView& bufferView = bufferViews[accessor.BufferView];

            mesh.MeshletSubsets = MakeSpan(reinterpret_cast<Subset*>(m_buffer.data() + bufferView.Offset), accessor.Count);
        }

        // Unique Vertex Index data
        {
            Accessor& accessor = accessors[meshView.UniqueVertexIndices];
            BufferView& bufferView = bufferViews[accessor.BufferView];

            mesh.UniqueVertexIndices = MakeSpan(m_buffer.data() + bufferView.Offset, bufferView.Size);
        }

        // Primitive Index data
        {
            Accessor& accessor = accessors[meshView.PrimitiveIndices];
            BufferView& bufferView = bufferViews[accessor.BufferView];

            mesh.PrimitiveIndices = MakeSpan(reinterpret_cast<PackedTriangle*>(m_buffer.data() + bufferView.Offset), accessor.Count);
        }
     }

     return true;
}

bool Model::UploadGpuResources(ISGDevice* pDevice, ISGCommandList* pCmdList)
{
    for (uint32_t i = 0; i < m_meshes.size(); ++i)
    {
        auto& m = m_meshes[i];

        // Create committed D3D resources of proper sizes
        m.IndexResource.Init(pDevice, m.Indices.size(), m.IndexSize);
        m.MeshletResource.Init(pDevice, m.Meshlets.size() * sizeof(m.Meshlets[0]), sizeof(m.Meshlets[0]));
        m.UniqueVertexIndexResource.Init(pDevice, DivRoundUp(m.UniqueVertexIndices.size(), 4) * 4, 4);
        m.PrimitiveIndexResource.Init(pDevice, m.PrimitiveIndices.size() * sizeof(m.PrimitiveIndices[0]), sizeof(m.PrimitiveIndices[0]));
        m.MeshInfoResource.Init(pDevice, sizeof(MeshInfo), sizeof(MeshInfo));

        m_meshes[i].MeshletInfoCBs.resize(m_meshes[i].MeshletSubsets.size());
        for (UINT j = 0; j < m_meshes[i].MeshletInfoCBs.size(); j++)
        {
            struct ShaderMeshInfo
            {
                UINT IndexBytes;
                UINT MeshletOffset;
            } meshInfo;

            meshInfo.IndexBytes = m_meshes[i].IndexSize;
            meshInfo.MeshletOffset = m_meshes[i].MeshletSubsets[j].Offset;

            ComPtr<ISGBuffer>& buffer = m_meshes[i].MeshletInfoCBs[j];

            SG_BUFFER_DESC cbDesc = FastBufferDesc::Constant(sizeof(ShaderMeshInfo));
            pDevice->CreateBuffer(&cbDesc, &buffer);

            uint8_t* memory = nullptr;
            buffer->Map(reinterpret_cast<void**>(&memory));
            std::memcpy(memory, &meshInfo, sizeof(meshInfo));
            buffer->Unmap();
        }

        m.VertexResources.resize(m.Vertices.size());

        for (uint32_t j = 0; j < m.Vertices.size(); ++j)
        {
            SG_BUFFER_DESC vertexDesc = FastBufferDesc::Vertex(m.Vertices[j].size(), true, false, false);
            pDevice->CreateBuffer(&vertexDesc, &m.VertexResources[j].Resourse);

            SG_SHADER_RESOURCE_VIEW_DESC viewDesc = FastViewDesc::AsStructuredBuffer(0, m.Vertices[j].size() / m.VertexStrides[j], m.VertexStrides[j]);
            pDevice->CreateShaderResourceView(m.VertexResources[j].Resourse.Get(), &viewDesc, &m.VertexResources[j].View);
        }

        // Map & copy memory to upload heap
        for (uint32_t j = 0; j < m.Vertices.size(); ++j)
        {
            UploadCommonBuffer(pDevice, pCmdList, m.VertexResources[j].Resourse.Get(), m.Vertices[j].data(), m.Vertices[j].size());
        }

        UploadCommonBuffer(pDevice, pCmdList, m.IndexResource.Resourse.Get(), m.Indices.data(), m.Indices.size());
        UploadCommonBuffer(pDevice, pCmdList, m.MeshletResource.Resourse.Get(), m.Meshlets.data(), m.Meshlets.size() * sizeof(m.Meshlets[0]));
        UploadCommonBuffer(pDevice, pCmdList, m.UniqueVertexIndexResource.Resourse.Get(), m.UniqueVertexIndices.data(), m.UniqueVertexIndices.size());
        UploadCommonBuffer(pDevice, pCmdList, m.PrimitiveIndexResource.Resourse.Get(), m.PrimitiveIndices.data(), m.PrimitiveIndices.size() * sizeof(m.PrimitiveIndices[0]));

        {
            MeshInfo info = {};
            info.IndexSize            = m.IndexSize;
            info.MeshletCount         = static_cast<uint32_t>(m.Meshlets.size());
            info.LastMeshletVertCount = m.Meshlets.back().VertCount;
            info.LastMeshletPrimCount = m.Meshlets.back().PrimCount;

            UploadCommonBuffer(pDevice, pCmdList, m.MeshInfoResource.Resourse.Get(), &info, sizeof(MeshInfo));
        }
    }

    return true;
}
