
//*********************************************************
//
// Copyright (c) 2023 Aleksei Shevchenko.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#include "CoreTypes.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SG_MAX_ROOT_SIGNATURE_PARAMETERS 7
#define SG_MAX_HIT_GROUPS_COUNT 32

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SG_DECLARE_CLASS(ISGInputLayout);
SG_DECLARE_CLASS(ISGPipelineState);
SG_DECLARE_CLASS(ISGBlendState);
SG_DECLARE_CLASS(ISGRasterizerState);
SG_DECLARE_CLASS(ISGDepthStencilState);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SG_SHADER_VISIBILITY
{
    SG_SHADER_VISIBILITY_ALL = 0,
    SG_SHADER_VISIBILITY_VERTEX = 1,
    SG_SHADER_VISIBILITY_HULL = 1,
    SG_SHADER_VISIBILITY_DOMAIN = 3,
    SG_SHADER_VISIBILITY_GEOMETRY = 4,
    SG_SHADER_VISIBILITY_PIXEL = 5,
    SG_SHADER_VISIBILITY_AMPLIFICATION = 6,
    SG_SHADER_VISIBILITY_MESH = 7
} SG_SHADER_VISIBILITY;

typedef enum SG_ROOT_SIGNATURE_TYPE
{
    SG_ROOT_SIGNATURE_TYPE_TABULAR = 0,
} SG_ROOT_SIGNATURE_TYPE;

typedef enum SG_PIPELINE_STATE_TYPE
{
    SG_PIPELINE_STATE_TYPE_UNDEFINED = 0,
    SG_PIPELINE_STATE_TYPE_GRAPHICS = 1,
    SG_PIPELINE_STATE_TYPE_MESH = 2,
    SG_PIPELINE_STATE_TYPE_COMPUTE = 3,
    SG_PIPELINE_STATE_TYPE_RAYTRACING = 4
} SG_PIPELINE_STATE_TYPE;

typedef enum SG_BLEND
{
    SG_BLEND_ZERO = 1,
    SG_BLEND_ONE = 2,

    SG_BLEND_SRC_COLOR = 3,
    SG_BLEND_INV_SRC_COLOR = 4,
    SG_BLEND_SRC_ALPHA = 5,
    SG_BLEND_INV_SRC_ALPHA = 6,
    SG_BLEND_DEST_ALPHA = 7,
    SG_BLEND_INV_DEST_ALPHA = 8,
    SG_BLEND_DEST_COLOR = 9,
    SG_BLEND_INV_DEST_COLOR = 10,
    SG_BLEND_SRC_ALPHA_SAT = 11,

    SG_BLEND_BLEND_FACTOR = 14,
    SG_BLEND_INV_BLEND_FACTOR = 15,

    SG_BLEND_SRC1_COLOR = 16,
    SG_BLEND_INV_SRC1_COLOR = 17,
    SG_BLEND_SRC1_ALPHA = 18,
    SG_BLEND_INV_SRC1_ALPHA = 19,
} SG_BLEND;

typedef enum SG_BLEND_OP
{
    SG_BLEND_OP_ADD = 1,
    SG_BLEND_OP_SUB = 2,
    SG_BLEND_OP_REV_SUB = 3,
    SG_BLEND_OP_MIN = 4,
    SG_BLEND_OP_MAX = 5
} SG_BLEND_OP;

typedef enum SG_LOGIC_OP
{
    SG_LOGIC_OP_CLEAR = 0,
    SG_LOGIC_OP_SET = (SG_LOGIC_OP_CLEAR + 1),
    SG_LOGIC_OP_COPY = (SG_LOGIC_OP_SET + 1),
    SG_LOGIC_OP_COPY_INVERTED = (SG_LOGIC_OP_COPY + 1),
    SG_LOGIC_OP_NOOP = (SG_LOGIC_OP_COPY_INVERTED + 1),
    SG_LOGIC_OP_INVERT = (SG_LOGIC_OP_NOOP + 1),
    SG_LOGIC_OP_AND = (SG_LOGIC_OP_INVERT + 1),
    SG_LOGIC_OP_NAND = (SG_LOGIC_OP_AND + 1),
    SG_LOGIC_OP_OR = (SG_LOGIC_OP_NAND + 1),
    SG_LOGIC_OP_NOR = (SG_LOGIC_OP_OR + 1),
    SG_LOGIC_OP_XOR = (SG_LOGIC_OP_NOR + 1),
    SG_LOGIC_OP_EQUIV = (SG_LOGIC_OP_XOR + 1),
    SG_LOGIC_OP_AND_REVERSE = (SG_LOGIC_OP_EQUIV + 1),
    SG_LOGIC_OP_AND_INVERTED = (SG_LOGIC_OP_AND_REVERSE + 1),
    SG_LOGIC_OP_OR_REVERSE = (SG_LOGIC_OP_AND_INVERTED + 1),
    SG_LOGIC_OP_OR_INVERTED = (SG_LOGIC_OP_OR_REVERSE + 1)
} SG_LOGIC_OP;

typedef enum SG_PRIMITIVE_TOPOLOGY
{
    SG_PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
    SG_PRIMITIVE_TOPOLOGY_POINT_LIST = 1,
    SG_PRIMITIVE_TOPOLOGY_LINE_LIST = 2,
    SG_PRIMITIVE_TOPOLOGY_LINE_STRIP = 3,
    SG_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 4,
    SG_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP = 5,
} SG_PRIMITIVE_TOPOLOGY;

typedef enum SG_FILL_MODE
{
    SG_FILL_MODE_WIREFRAME = 2,
    SG_FILL_MODE_SOLID = 3,
} SG_FILL_MODE;

typedef enum SG_CULL_MODE
{
    SG_CULL_MODE_NONE = 0,
    SG_CULL_MODE_FRONT = 1,
    SG_CULL_MODE_BACK = 2,
} SG_CULL_MODE;

typedef enum SG_STENCIL_OP
{
    SG_STENCIL_OP_KEEP = 0,
    SG_STENCIL_OP_ZERO = 1,
    SG_STENCIL_OP_REPLACE = 2,
    SG_STENCIL_OP_INCREMENT_SAT = 3,
    SG_STENCIL_OP_DESCREMENT_SAT = 4,
    SG_STENCIL_OP_INVERT = 5,
    SG_STENCIL_OP_INCREMENT_WRAP = 6,
    SG_STENCIL_OP_DECREMENT_WRAP = 7
} SG_STENCIL_OP;

typedef enum SG_DEPTH_WRITE_MASK
{
    SG_DEPTH_WRITE_MASK_ZERO = 0,
    SG_DEPTH_WRITE_MASK_ALL = 1
} SG_DEPTH_WRITE_MASK;

typedef enum SG_INPUT_CLASSIFICATION
{
    SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA = 0,
    SG_INPUT_CLASSIFICATION_PER_INSTANCE_DATA = 1
} SG_INPUT_CLASSIFICATION;

typedef enum SG_HIT_GROUP_TYPE
{
    SG_HIT_GROUP_TYPE_TRIANGLES = 0,
    SG_HIT_GROUP_TYPE_PROCEDURAL_PRIMITIVE = 0x1
} SG_HIT_GROUP_TYPE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SG_INPUT_ELEMENT_DESC
{
    char const* SemanticName;
    SgU32 SemanticIndex;
    SG_FORMAT Format;
    SgU32 InputSlot;
    SgU32 AlignedByteOffset;
    SG_INPUT_CLASSIFICATION InputSlotClass;
    SgU32 InstanceDataStepRate;
} SG_INPUT_ELEMENT_DESC;

typedef struct SG_SHADER_BYTECODE
{
    void* pData;
    size_t SizeBytes;
} SG_SHADER_BYTECODE;

typedef struct SG_STATIC_SAMPLER_DESC
{
    SgU32 RegisterSpace;
    SgU32 ShaderRegister;
    SG_SAMPLER_DESC SamplerDesc;
} SG_STATIC_SAMPLER_DESC;

typedef struct SG_BINDING_RANGE
{
    SgU32 RegisterSpace;
    SgU32 BaseShaderRegister;
    SgU32 NumRegisters;
} SG_BINDING_RANGE;

typedef struct SG_BINDING_TABLE_DESC
{
    SG_SHADER_VISIBILITY ShaderVisibility;
    SG_BINDING_RANGE ConstantBuffers;
    SG_BINDING_RANGE SRVs;
    SG_BINDING_RANGE UAVs;
    SG_BINDING_RANGE AsSRVs;

    SG_BINDING_RANGE Samplers;
    SgU32 NumStaticSamplers;
    SG_STATIC_SAMPLER_DESC* pStaticSamplers;
} SG_BINDING_TABLE_DESC;

typedef struct SG_TABULAR_ROOT_SIGNATURE
{
    SgU32 NumTables;
    SG_BINDING_TABLE_DESC* pTables;
} SG_TABULAR_ROOT_SIGNATURE;

typedef struct SG_ROOT_SIGNATURE_DESC
{
    SG_ROOT_SIGNATURE_TYPE Type;
    union
    {
        SG_TABULAR_ROOT_SIGNATURE Tabular;
    };
} SG_ROOT_SIGNATURE_DESC;

typedef struct SG_GRAPHICS_PIPELINE_STATE_DESC
{
    SG_ROOT_SIGNATURE_DESC RootSignature;
    SG_SHADER_BYTECODE PS;
    SG_SHADER_BYTECODE VS;
    SG_SHADER_BYTECODE DS;
    SG_SHADER_BYTECODE HS;
    SG_SHADER_BYTECODE GS;
    ISGInputLayout*  pInputLayout;
} SG_GRAPHICS_PIPELINE_STATE_DESC;

typedef struct SG_MESH_PIPELINE_STATE_DESC
{
    SG_ROOT_SIGNATURE_DESC RootSignature;
    SG_SHADER_BYTECODE PS;
    SG_SHADER_BYTECODE MS;
    SG_SHADER_BYTECODE AS;
} SG_MESH_PIPELINE_STATE_DESC;

typedef struct SG_COMPUTE_PIPELINE_STATE_DESC
{
    SG_ROOT_SIGNATURE_DESC RootSignature;
    SG_SHADER_BYTECODE CS;
} SG_COMPUTE_PIPELINE_STATE_DESC;

typedef struct SG_RAYTRACING_SHADER
{
    char const* pName;
    SG_SHADER_BYTECODE ByteCode;
} SG_RAYTRACING_SHADER;

typedef struct SG_RAYTRACING_HIT_GROUP
{
    SG_HIT_GROUP_TYPE Type;
    SG_RAYTRACING_SHADER ClosestHit;
    SG_RAYTRACING_SHADER AnyHit;
    SG_RAYTRACING_SHADER IntersectionShader;
} SG_RAYTRACING_HIT_GROUP;

typedef struct SG_RAYTRACING_PIPELINE_DESC
{
    SG_ROOT_SIGNATURE_DESC RootSignature;

    SG_RAYTRACING_SHADER RayGenShader;
    SG_RAYTRACING_SHADER MissShader;

    SgU32 HitGroupCount;
    SG_RAYTRACING_HIT_GROUP* pHitGroups;

    SgU32 MaxPayloadSize;
    SgU32 MaxAttributeSize;
    SgU32 MaxRecursionDepth;
} SG_RAYTRACING_PIPELINE_DESC;

typedef struct SG_RENDER_TARGET_BLEND_DESC
{
    SgBool BlendEnable;
    SgBool LogicOpEnable;
    SG_BLEND SrcColor;
    SG_BLEND DestColor;
    SG_BLEND_OP BlendOp;

    SG_BLEND SrcAlpha;
    SG_BLEND DestAlpha;
    SG_BLEND_OP BlendOpAlpha;

    SG_LOGIC_OP LogicOp;

    SgU8 WriteMask;
} SG_RENDER_TARGET_BLEND_DESC;

typedef struct SG_BLEND_STATE_DESC
{
    SgBool AlphaToCoverageEnable;
    SgBool IndependentBlendEnable;
    SG_RENDER_TARGET_BLEND_DESC RenderTarget[8];
} SG_BLEND_STATE_DESC;

typedef struct SG_RASTERIZER_STATE_DESC
{
    SG_FILL_MODE FillMode;
    SG_CULL_MODE CullMode;
    SgBool FrontCounterClockwise;

    int DepthBias;
    float DepthBiasClamp;
    float SlopeScaledDepthBias;
    SgBool DepthClipEnable;

    SgBool MultisampleEnable;
    SgBool AntialiasedLineEnable;
    SgU32 ForcedSampleCount;

    SgBool ConservativeRaster;
} SG_RASTERIZER_STATE_DESC;

typedef struct SG_DEPTH_STENCIL_OP_DESC
{
    SG_STENCIL_OP StencilFailOp;
    SG_STENCIL_OP StencilDepthFailOp;
    SG_STENCIL_OP StencilPassOp;
    SG_COMPARISON_FUNC StencilFunc;
} SG_DEPTH_STENCIL_OP_DESC;

typedef struct SG_DEPTH_STENCIL_STATE_DESC
{
    SgBool DepthEnable;
    SG_DEPTH_WRITE_MASK DepthWriteMask;
    SG_COMPARISON_FUNC DepthFunc;

    SgBool StencilEnable;
    SgU8 StencilReadMask;
    SgU8 StencilWriteMask;
    SG_DEPTH_STENCIL_OP_DESC FrontFace;
    SG_DEPTH_STENCIL_OP_DESC BackFace;
} SG_DEPTH_STENCIL_STATE_DESC;


//    static const SgU8 ColorWriteMaskRed = 1;
//    static const SgU8 ColorWriteMaskGreen = 2;
//    static const SgU8 ColorWriteMaskBlue = 4;
//    static const SgU8 ColorWriteMaskAlpha = 8;
//    static const SgU8 ColorWriteMaskAll = (ColorWriteMaskRed | ColorWriteMaskGreen | ColorWriteMaskBlue | ColorWriteMaskAlpha);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef C_STYLE
SG_INTERFACE SG_API ISGInputLayout : public ISGObject
{
public:
};

#else
typedef struct ISGInputLayoutVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGInputLayout* pThis);

    SgU32 (SG_CALL *Release)(
        ISGInputLayout* pThis);

} ISGInputLayoutVtbl;

SG_INTERFACE ISGInputLayout
{
    ISGInputLayoutVtbl * pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGPipelineState : public ISGObject
{
public:
    virtual SG_PIPELINE_STATE_TYPE SG_CALL GetType() = 0;

    virtual void SG_CALL SetDebugName(
        char const* pDebugName) = 0;
};

#else
typedef struct ISGPipelineStateVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGPipelineState* pThis);

    SgU32 (SG_CALL *Release)(
        ISGPipelineState* pThis);

    // ISGPipelineState
    SG_PIPELINE_STATE_TYPE (SG_CALL *GetType)(
        ISGPipelineState* pThis);

    void (SG_CALL *SetDebugName)(
        ISGPipelineState* pThis,
        char const* pDebugName);

} ISGPipelineStateVtbl;

SG_INTERFACE ISGPipelineState
{
    ISGPipelineStateVtbl * pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGBlendState : public ISGObject
{
public:
};

#else
typedef struct ISGBlendStateVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGBlendState* pThis);

    SgU32 (SG_CALL *Release)(
        ISGBlendState* pThis);

} ISGBlendStateVtbl;

SG_INTERFACE ISGBlendState
{
    ISGBlendStateVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGRasterizerState : public ISGObject
{
public:
};

#else
typedef struct ISGRasterizerStateVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGRasterizerState* pThis);

    SgU32 (SG_CALL *Release)(
        ISGRasterizerState* pThis);

} ISGRasterizerStateVtbl;

SG_INTERFACE ISGRasterizerState
{
    ISGRasterizerStateVtbl * pVtbl;
};
#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGDepthStencilState : public ISGObject
{
public:
};

#else
typedef struct ISGDepthStencilStateVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGDepthStencilState* pThis);

    SgU32 (SG_CALL *Release)(
        ISGDepthStencilState* pThis);

} ISGDepthStencilStateVtbl;

SG_INTERFACE ISGDepthStencilState
{
    ISGDepthStencilStateVtbl * pVtbl;
};

#endif

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
