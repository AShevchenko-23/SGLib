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

#pragma once

#include "Span.h"
#include <windows.h>
#include <vector>
#include <DirectXCollision.h>
#include "SGX/SGHelpers.h"

#include <wrl.h>
using Microsoft::WRL::ComPtr;

struct Attribute
{
    enum EType : uint32_t
    {
        Position,
        Normal,
        TexCoord,
        Tangent,
        Bitangent,
        Count
    };

    EType    Type;
    uint32_t Offset;
};

struct Subset
{
    uint32_t Offset;
    uint32_t Count;
};

struct MeshInfo
{
    uint32_t IndexSize;
    uint32_t MeshletCount;

    uint32_t LastMeshletVertCount;
    uint32_t LastMeshletPrimCount;
};

struct Meshlet
{
    uint32_t VertCount;
    uint32_t VertOffset;
    uint32_t PrimCount;
    uint32_t PrimOffset;
};

struct PackedTriangle
{
    uint32_t i0 : 10;
    uint32_t i1 : 10;
    uint32_t i2 : 10;
};

struct StructuredBuffer
{
    void Init(ISGDevice* pDevice, U32 sizeBytes, U32 stride)
    {
        SG_BUFFER_DESC desc = FastBufferDesc::Structured(sizeBytes, true, false, false);
        pDevice->CreateBuffer(&desc, &Resourse);

        SG_SHADER_RESOURCE_VIEW_DESC viewDesc = FastViewDesc::AsStructuredBuffer(0, sizeBytes / stride, stride);
        pDevice->CreateShaderResourceView(Resourse.Get(), &viewDesc, &View);
    }

    ComPtr<ISGBuffer> Resourse;
    ComPtr<ISGShaderResourceView> View;
};

struct Mesh
{
    std::vector<Span<uint8_t>> Vertices;
    std::vector<uint32_t>      VertexStrides;
    uint32_t                   VertexCount;

    Span<Subset>               IndexSubsets;
    Span<uint8_t>              Indices;
    uint32_t                   IndexSize;
    uint32_t                   IndexCount;

    Span<Subset>                    MeshletSubsets;
    std::vector<ComPtr<ISGBuffer>>  MeshletInfoCBs;
    Span<Meshlet>                   Meshlets;
    Span<uint8_t>                   UniqueVertexIndices;
    Span<PackedTriangle>            PrimitiveIndices;

    // D3D resource references
    std::vector<StructuredBuffer> VertexResources;
    StructuredBuffer    IndexResource;
    StructuredBuffer    MeshletResource;
    StructuredBuffer    UniqueVertexIndexResource;
    StructuredBuffer    PrimitiveIndexResource;
    StructuredBuffer    MeshInfoResource;

    // Calculates the number of instances of the last meshlet which can be packed into a single threadgroup.
    uint32_t GetLastMeshletPackCount(uint32_t subsetIndex, uint32_t maxGroupVerts, uint32_t maxGroupPrims) 
    { 
        if (Meshlets.size() == 0)
            return 0;

        auto& subset = MeshletSubsets[subsetIndex];
        auto& meshlet = Meshlets[subset.Offset + subset.Count - 1];

        return min(maxGroupVerts / meshlet.VertCount, maxGroupPrims / meshlet.PrimCount);
    }

    void GetPrimitive(uint32_t index, uint32_t& i0, uint32_t& i1, uint32_t& i2) const
    {
        auto prim = PrimitiveIndices[index];
        i0 = prim.i0;
        i1 = prim.i1;
        i2 = prim.i2;
    }

    uint32_t GetVertexIndex(uint32_t index) const
    {
        const uint8_t* addr = UniqueVertexIndices.data() + index * IndexSize;
        if (IndexSize == 4)
        {
            return *reinterpret_cast<const uint32_t*>(addr);
        }
        else 
        {
            return *reinterpret_cast<const uint16_t*>(addr);
        }
    }
};

class Model
{
public:
    bool LoadFromFile(const char* filename);
    bool UploadGpuResources(ISGDevice* pDevice, ISGCommandList* pCmdList);

    uint32_t GetMeshCount() const { return static_cast<uint32_t>(m_meshes.size()); }
    const Mesh& GetMesh(uint32_t i) const { return m_meshes[i]; }

    // Iterator interface
    auto begin() { return m_meshes.begin(); }
    auto end() { return m_meshes.end(); }

private:
    std::vector<Mesh>                      m_meshes;
    std::vector<uint8_t>                   m_buffer;
};
