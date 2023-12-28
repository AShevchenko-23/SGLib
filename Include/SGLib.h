
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
#include "Format.h"
#include "Resource.h"
#include "ResourceView.h"
#include "Sampler.h"
#include "RTAccelerationStructure.h"
#include "PipelineState.h"
#include "Query.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
extern "C" {
#endif

#define SG_MAX_QUEUE_COUNT 3

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SG_DECLARE_CLASS(ISGAdapter);
SG_DECLARE_CLASS(ISGOutput);
SG_DECLARE_CLASS(ISGSwapChain);

SG_DECLARE_CLASS(ISGDevice);
SG_DECLARE_CLASS(ISGExecutionContext);
SG_DECLARE_CLASS(ISGCommandList);

typedef void (SG_CALL *PFN_SG_DEVICE_REMOVAL_CALLBACK)(SgU32 messageLen, char const* message);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SG_DEBUG_LEVELS
{
    SG_DEBUG_DISABLED = 0,
    SG_DEBUG_L1 = 1,
    SG_DEBUG_L2 = 2,
} SG_DEBUG_LEVELS;

typedef enum SG_DEBUG_TOOLS
{
    SG_DEBUG_TOOL_NONE = 0,
    SG_DEBUG_TOOL_MS_PIX = 0x1,
    SG_DEBUG_TOOL_NV_NSIGHT_AFTERMATH = 0x2,
} SG_DEBUG_TOOLS;

SG_DEFINE_ENUM_OPERATORS(SG_DEBUG_TOOLS);

typedef enum SG_CLEAR_FLAGS
{
    SG_CLEAR_FLAG_DEPTH = 0x1,
    SG_CLEAR_FLAG_STENCIL = 0x2
} SG_CLEAR_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_CLEAR_FLAGS)

typedef enum SG_ADAPTER_INFO_CATEGORIES
{
    SG_ADAPTER_INFO_CAT_COMMON = 0,
    SG_ADAPTER_INFO_CAT_MEMORY = 1,
    SG_ADAPTER_INFO_CAT_EXECUTION = 2,
    SG_ADAPTER_INFO_CAT_SHADERS = 3,
    SG_ADAPTER_INFO_CAT_PIPELINE_STATES = 4
} SG_ADAPTER_INFO_CATEGORIES;

typedef enum SG_SWAP_EFFECTS
{
    SG_SWAP_EFFECT_SEQUENTIAL = 0,
    SG_SWAP_EFFECT_DISCARD = 1
} SG_SWAP_EFFECTS;
    
typedef enum SG_SWAP_CHAIN_FLAGS
{
    SG_SWAP_CHAIN_FLAG_NONE = 0,
    SG_SWAP_CHAIN_FLAG_ALLOW_TEARING = 0x1,
    SG_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH = 0x2,
    SG_SWAP_CHAIN_FLAG_ALLOW_SHADER_RESOURCE = 0x4,
} SG_SWAP_CHAIN_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_SWAP_CHAIN_FLAGS);

typedef enum SG_CONSERVATIVE_RASTERIZATION_TIER
{
    SG_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED = 0,
    SG_CONSERVATIVE_RASTERIZATION_TIER_1 = 1,
    SG_CONSERVATIVE_RASTERIZATION_TIER_2 = 2,
    SG_CONSERVATIVE_RASTERIZATION_TIER_3 = 3
} SG_CONSERVATIVE_RASTERIZATION_TIER;

typedef enum SG_RAYTRACING_TIER
{
    SG_RAYTRACING_TIER_NOT_SUPPORTED = 0,
    SG_RAYTRACING_TIER_1_0 = 10,
    SG_RAYTRACING_TIER_1_1 = 11
} SG_RAYTRACING_TIER;

typedef enum SG_VARIABLE_SHADING_RATE_TIER
{
    SG_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED = 0,
    SG_VARIABLE_SHADING_RATE_TIER_1 = 1,
    SG_VARIABLE_SHADING_RATE_TIER_2 = 2
} SG_VARIABLE_SHADING_RATE_TIER;

typedef enum SG_MESH_SHADER_TIER
{
    SG_MESH_SHADER_TIER_NOT_SUPPORTED = 0,
    SG_MESH_SHADER_TIER_1 = 10
} SG_MESH_SHADER_TIER;

typedef enum SG_SHADER_MODEL
{
    SHADER_MODEL_5_1 = 0x51,
    SHADER_MODEL_6_0 = 0x60,
    SHADER_MODEL_6_1 = 0x61,
    SHADER_MODEL_6_2 = 0x62,
    SHADER_MODEL_6_3 = 0x63,
    SHADER_MODEL_6_4 = 0x64,
    SHADER_MODEL_6_5 = 0x65,
    SHADER_MODEL_6_6 = 0x66,
} SG_SHADER_MODEL;

typedef enum SG_SHADER_MIN_PRECISION_SUPPORT_FLAGS
{
    SHADER_MIN_PRECISION_SUPPORT_NONE = 0,
    SHADER_MIN_PRECISION_SUPPORT_10BITS = 0x1,
    SHADER_MIN_PRECISION_SUPPORT_16BITS = 0x2,
} SG_SHADER_MIN_PRECISION_SUPPORT_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_SHADER_MIN_PRECISION_SUPPORT_FLAGS);

typedef enum SG_MODE_SCANLINE_ORDER
{
    SG_MODE_SCANLINE_ORDER_UNSPECIFIED = 0,
    SG_MODE_SCANLINE_ORDER_PROGRESSIVE = 1,
    SG_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST = 2,
    SG_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST = 3
} SG_MODE_SCANLINE_ORDER;

typedef enum SG_MODE_SCALING
{
    SG_MODE_SCALING_UNSPECIFIED = 0,
    SG_MODE_SCALING_CENTERED = 1,
    SG_MODE_SCALING_STRETCHED = 2
} SG_MODE_SCALING;

typedef enum SG_QUEUE_TYPE
{
    SG_QUEUE_TYPE_GRAPHICS = 0,
    SG_QUEUE_TYPE_COMPUTE = 1,
    SG_QUEUE_TYPE_COPY = 2,
} SG_QUEUE_TYPE;

typedef enum SG_MEMORY_SEGMENT_GROUP
{
    SG_MEMORY_SEGMENT_GROUP_LOCAL = 0,
    SG_MEMORY_SEGMENT_GROUP_NON_LOCAL = 1
} SG_MEMORY_SEGMENT_GROUP;

typedef enum SG_SHADING_RATE
{
    SG_SHADING_RATE_1X1 = 0,
    SG_SHADING_RATE_1X2 = 0x1,
    SG_SHADING_RATE_2X1 = 0x4,
    SG_SHADING_RATE_2X2 = 0x5,
    SG_SHADING_RATE_2X4 = 0x6,
    SG_SHADING_RATE_4X2 = 0x9,
    SG_SHADING_RATE_4X4 = 0xA
} SG_SHADING_RATE;

typedef enum SG_SHADING_RATE_COMBINER
{
    SG_SHADING_RATE_COMBINER_PASSTHROUGH = 0,
    SG_SHADING_RATE_COMBINER_OVERRIDE = 1,
    SG_SHADING_RATE_COMBINER_MIN = 2,
    SG_SHADING_RATE_COMBINER_MAX = 3,
    SG_SHADING_RATE_COMBINER_SUM = 4
} SG_SHADING_RATE_COMBINER;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SG_DEBUG_CONFIG
{
    SG_DEBUG_LEVELS DebugLevel;
    SG_DEBUG_TOOLS DebugTools;
} SG_DEBUG_CONFIG;

typedef struct SG_DRAW_INDIRECT_ARGS
{
    SgU32 VertexCountPerInstance;
    SgU32 InstanceCount;
    SgU32 StartVertexLocation;
    SgU32 StartInstanceLocation;
} SG_DRAW_INDIRECT_ARGS;

typedef struct SG_DRAW_INDEXED_INDIRECT_ARGS
{
    SgU32 IndexCountPerInstance;
    SgU32 InstanceCount;
    SgU32 StartIndexLocation;
    SgI32 BaseVertexLocation;
    SgU32 StartInstanceLocation;
} SG_DRAW_INDEXED_INDIRECT_ARGS;

typedef struct SG_DISPATCH_INDIRECT_ARGS
{
    SgU32 ThreadGroupCountX;
    SgU32 ThreadGroupCountY;
    SgU32 ThreadGroupCountZ;
} SG_DISPATCH_INDIRECT_ARGS;

typedef struct SG_DISPATCH_MESH_INDIRECT_ARGS
{
    SgU32 ThreadGroupCountX;
    SgU32 ThreadGroupCountY;
    SgU32 ThreadGroupCountZ;
} SG_DISPATCH_MESH_INDIRECT_ARGS;

typedef struct SG_ADAPTER_INFO_COMMON
{
    SgU32 VendorID;
    SgU8 DeviceLUID[8];
    char Description[128];
    SgU64 DriverVersion;
} SG_ADAPTER_INFO_COMMON;

typedef struct SG_ADAPTER_INFO_MEMORY
{
    SgBool IsUMA;
    SgU64 LocalMemory;
    SgU64 NonLocalMemory;
} SG_ADAPTER_INFO_MEMORY;

typedef struct SG_ADAPTER_INFO_PIPELINE_STATES
{
    SgBool OutputMergerLogicOp;
    SgBool ROVsSupported;
    SgBool DepthBoundsTestSupported;
    SG_CONSERVATIVE_RASTERIZATION_TIER ConservativeRasterizationTier;
    SgU32  MaxCBsPerStage;
    SgU32  MaxSRVsPerStage;
    SgU32  MaxUAVsPerPass;
    SgU32  MaxSMPsPerStage;
} SG_ADAPTER_INFO_PIPELINE_STATES;

typedef struct SG_ADAPTER_INFO_SHADERS
{
    SG_SHADER_MODEL ShaderModel;
    SG_SHADER_MIN_PRECISION_SUPPORT_FLAGS MinPrecisionSupport;

    SgBool DoublePrecisionFloatShaderOps;
    SgBool Native16BitShaderOps;
    SgBool Int64ShaderOps;
    SgBool PSSpecifiedStencilRefSupported;
    SgBool BarycentricsSupported;
    SgBool VPAndRTArrayIndex;

    SgBool WaveOps;
    SgU32 WaveLaneCountMin;
    SgU32 WaveLaneCountMax;
    SgU32 TotalLaneCount;
} SG_ADAPTER_INFO_SHADERS;

typedef struct SG_ADAPTER_INFO_EXECUTION
{
    SgU8 GraphicsQueues;
    SgU8 ComputeQueues;
    SgU8 CopyQueues;

    SgBool CopyQueueTimeStamps;

    SG_RAYTRACING_TIER RaytracingTier;
    SG_MESH_SHADER_TIER MeshShaderTier;

    SG_VARIABLE_SHADING_RATE_TIER VariableShadingRateTier;
    SgU32 ShadingRateImageTileSize;
    SgBool AdditionalShadingRates;
    SgBool PerPrimitiveShadingRateWithVPIndexing;
} SG_ADAPTER_INFO_EXECUTION;

typedef struct SG_MEMORY_DEBUG_CONFIG
{
    SgU64 LocalBudgetLimit;
    SgBool AllowOutOfBounds;
} SG_MEMORY_DEBUG_CONFIG;

typedef struct SG_EXECUTION_CONTEXT_DESC
{
    SgU8 FrameBuffers;
    SgU8 QueueCount;
    SG_QUEUE_TYPE QueueTypes[SG_MAX_QUEUE_COUNT];
} SG_EXECUTION_CONTEXT_DESC;

typedef struct SG_DEVICE_DESC
{
    PFN_SG_DEVICE_REMOVAL_CALLBACK pDeviceRemovalCallback;
    SG_MEMORY_DEBUG_CONFIG* pMemoryDebugConfig;
    SG_EXECUTION_CONTEXT_DESC* pExecutionContextDesc;
} SG_DEVICE_DESC;

typedef struct SG_MEMORY_USAGE
{
    SgU64 Budget;
    SgU64 UsedMemory;
} SG_MEMORY_USAGE;

typedef struct SG_QUEUE_CLOCK_CALIBRATION
{
    SgU64 CpuTimestamp;
    SgU64 GpuTimestamp;
} SG_QUEUE_CLOCK_CALIBRATION;

typedef struct SG_DISPLAY_MODE
{
    SgU32       Width;
    SgU32       Height;
    SG_FORMAT   Format;

    SgU32       RefreshNumerator;
    SgU32       RefreshDenomerator;
    SG_MODE_SCANLINE_ORDER ScanlineOrdering;
    SG_MODE_SCALING Scaling;
} SG_DISPLAY_MODE;

typedef struct SG_SWAP_CHAIN_DESC
{
    void*               OutputWindow;
    SgU32               Width;
    SgU32               Height;
    SG_FORMAT           Format;
    SgU32               BufferCount;
    SG_SWAP_CHAIN_FLAGS Flags;
    SG_SWAP_EFFECTS     SwapEffect;
} SG_SWAP_CHAIN_DESC;

typedef struct SG_VIEWPORT
{
    float Left;
    float Top;
    float Width;
    float Height;
    float MinDepth;
    float MaxDepth;
} SG_VIEWPORT;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef C_STYLE
SG_INTERFACE SG_API ISGAdapter : public ISGObject
{
public:
    virtual SG_RESULT SG_CALL EnumOutputs(
        SgU32 index,
        ISGOutput** ppOutput) = 0;

    virtual SG_RESULT SG_CALL GetInfo(
        SG_ADAPTER_INFO_CATEGORIES category,
        void* pOutInfo) = 0;
};

#else
typedef struct ISGAdapterVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGAdapter* pThis);

    SgU32 (SG_CALL *Release)(
        ISGAdapter* pThis);

    // ISGAdapter
    SG_RESULT (SG_CALL *EnumOutputs)(
        ISGAdapter* pThis,
        SgU32 index,
        ISGOutput** ppOutput);

    SG_RESULT (SG_CALL *GetInfo)(
        ISGAdapter* pThis,
        SG_ADAPTER_INFO_CATEGORIES category,
        void* pOutInfo);
} ISGAdapterVtbl;

SG_INTERFACE ISGAdapter
{
    ISGAdapterVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGSwapChain : public ISGObject
{
public:
    virtual SG_RESULT SG_CALL SetFullScreenState(
        SgBool fullscreen,
        SG_DISPLAY_MODE const* pDisplayMode,
        ISGOutput* pOutput) = 0;

    virtual SG_RESULT SG_CALL Resize(
        SgU32 width,
        SgU32 height) = 0;

    virtual void SG_CALL SetSyncInterval(
        SgU32 syncInterval) = 0;

    virtual SgU32 SG_CALL GetBackBufferWidth() = 0;

    virtual SgU32 SG_CALL GetBackBufferHeight() = 0;

    virtual ISGTexture* SG_CALL GetCurrentTexture() = 0;

    virtual ISGRenderTargetView* SG_CALL GetCurrentRTV() = 0;

    virtual ISGShaderResourceView* SG_CALL GetCurrentSRV() = 0;
};

#else
typedef struct ISGSwapChainVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGSwapChain* pThis);

    SgU32 (SG_CALL *Release)(
        ISGSwapChain* pThis);

    // ISGSwapChain
    SG_RESULT (SG_CALL *SetFullScreenState)(
        ISGSwapChain* pThis,
        SgBool fullscreen,
        SG_DISPLAY_MODE const* pDisplayMode,
        ISGOutput* pOutput);

    SG_RESULT (SG_CALL *Resize)(
        ISGSwapChain* pThis,
        SgU32 width,
        SgU32 height);

    void (SG_CALL *SetSyncInterval)(
        ISGSwapChain* pThis,
        SgU32 syncInterval);

    SgU32 (SG_CALL *GetBackBufferWidth)(
        ISGSwapChain* pThis);

    SgU32 (SG_CALL *GetBackBufferHeight)(
        ISGSwapChain* pThis);

    ISGTexture* (SG_CALL *GetCurrentTexture)(
        ISGSwapChain* pThis);

    ISGRenderTargetView* (SG_CALL *GetCurrentRTV)(
        ISGSwapChain* pThis);

    ISGShaderResourceView* (SG_CALL *GetCurrentSRV)(
        ISGSwapChain* pThis);
} ISGSwapChainVtbl;

SG_INTERFACE ISGSwapChain
{
    ISGSwapChainVtbl * pVtbl;
};
#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGOutput : public ISGObject
{
public:
    virtual char const* SG_CALL GetDeviceName() = 0;

    virtual SG_RECT SG_CALL GetDesktopCoords() = 0;

    virtual SgU32 SG_CALL GetDisplayModesCount(
        SG_FORMAT format) = 0;

    virtual SG_RESULT SG_CALL GetDisplayModes(
        SG_FORMAT format,
        SG_DISPLAY_MODE* pOutModes) = 0;
};

#else
typedef struct ISGOutputVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGOutput* pThis);

    SgU32 (SG_CALL *Release)(
        ISGOutput* pThis);

    // ISGOutput
    char const* (SG_CALL *GetDeviceName)(
        ISGOutput* pThis);

    SG_RECT (SG_CALL *GetDesktopCoords)(
        ISGOutput* pThis);

    SgU32 (SG_CALL *GetDisplayModesCount)(
        ISGOutput* pThis,
        SG_FORMAT format);

    SG_RESULT (SG_CALL *GetDisplayModes)(ISGOutput* pThis,
        SG_FORMAT format,
        SG_DISPLAY_MODE* pOutModes);

} ISGOutputVtbl;

SG_INTERFACE ISGOutput
{
    ISGOutputVtbl * pVtbl;
};
#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGDevice : public ISGObject
{
public:
    virtual SG_RESULT SG_CALL GetExecutionContext(
        ISGExecutionContext** ppExecutionContext) = 0;

    virtual SG_RESULT SG_CALL GetMemoryUsage(
        SG_MEMORY_SEGMENT_GROUP memSegment,
        SG_MEMORY_USAGE* pMemoryUsage) = 0;

    virtual SG_RESULT SG_CALL CreateBuffer(
        SG_BUFFER_DESC const* pDesc,
        ISGBuffer** ppBuffer) = 0;

    virtual SG_RESULT SG_CALL CreateTexture(
        SG_TEXTURE_DESC const* pDesc,
        ISGTexture** ppTexture) = 0;

    virtual SG_RESULT SG_CALL CreateBottomLevelAccelerationStructure(
        SG_AS_BOTTOM_LEVEL_DESC const* pDesc,
        ISGBottomLevelAS** ppBLAS) = 0;

    virtual SG_RESULT SG_CALL CreateTopLevelAccelerationStructure(
        SG_AS_TOP_LEVEL_DESC const* pDesc,
        ISGTopLevelAS** ppTLAS) = 0;

    virtual SG_RESULT SG_CALL CreateShaderResourceView(
        ISGResource* pResource,
        SG_SHADER_RESOURCE_VIEW_DESC const* pViewDesc,
        ISGShaderResourceView** ppShaderResourceView) = 0;

    virtual SG_RESULT SG_CALL CreateUnorderedAccessView(
        ISGResource* pResource,
        SG_UNORDERED_ACCESS_VIEW_DESC const* pViewDesc,
        ISGUnorderedAccessView** ppUnorderedAccessView) = 0;

    virtual SG_RESULT SG_CALL CreateRenderTargetView(
        ISGResource* pResource,
        SG_RENDER_TARGET_VIEW_DESC const* pViewDesc,
        ISGRenderTargetView** ppRenderTargetView) = 0;

    virtual SG_RESULT SG_CALL CreateDepthStencilView(
        ISGResource* pResource,
        SG_DEPTH_STENCIL_VIEW_DESC const* pViewDesc,
        ISGDepthStencilView** ppDepthStencilView) = 0;

    virtual SG_RESULT SG_CALL CreateSampler(
        SG_SAMPLER_DESC const* pDesc,
        ISGSampler** ppSampler) = 0;

    virtual SG_RESULT SG_CALL CreateQuery(
        SG_QUERY_TYPE type,
        ISGQuery** ppQuery) = 0;

    virtual SG_RESULT SG_CALL CreatePredicate(
        SG_QUERY_TYPE type,
        ISGPredicate** ppPredicate) = 0;

    virtual SG_RESULT SG_CALL CreateInputLayout(
        SgU32 numElements,
        SG_INPUT_ELEMENT_DESC const* pInputElements,
        ISGInputLayout** ppInputLayout) = 0;

    virtual SG_RESULT SG_CALL CreateGraphicsPipelineState(
        SG_GRAPHICS_PIPELINE_STATE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState) = 0;

    virtual SG_RESULT SG_CALL CreateMeshPipelineState(
        SG_MESH_PIPELINE_STATE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState) = 0;

    virtual SG_RESULT SG_CALL CreateComputePipelineState(
        SG_COMPUTE_PIPELINE_STATE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState) = 0;

    virtual SG_RESULT SG_CALL CreateRaytracingPipelineState(
        SG_RAYTRACING_PIPELINE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState) = 0;

    virtual SG_RESULT SG_CALL CreateBlendState(
        SG_BLEND_STATE_DESC const* pDesc,
        ISGBlendState** ppBlendState) = 0;

    virtual SG_RESULT SG_CALL CreateDepthStencilState(
        SG_DEPTH_STENCIL_STATE_DESC const* pDesc,
        ISGDepthStencilState** ppDepthStencilState) = 0;

    virtual SG_RESULT SG_CALL CreateRasterizerState(
        SG_RASTERIZER_STATE_DESC const* pDesc,
        ISGRasterizerState** ppRasterizerState) = 0;
};

#else
typedef struct ISGDeviceVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGDevice* pThis);

    SgU32 (SG_CALL *Release)(
        ISGDevice* pThis);

    // ISGDevice
    SG_RESULT (SG_CALL *GetExecutionContext)(
        ISGDevice* pThis,
        ISGExecutionContext** ppExecutionContext);

    SG_RESULT (SG_CALL *GetMemoryUsage)(
        ISGDevice* pThis,
        SG_MEMORY_SEGMENT_GROUP memSegment,
        SG_MEMORY_USAGE* pMemoryUsage);

    SG_RESULT (SG_CALL *CreateBuffer)(
        ISGDevice* pThis,
        SG_BUFFER_DESC const* pDesc,
        ISGBuffer** ppBuffer);

    SG_RESULT (SG_CALL *CreateTexture)(
        ISGDevice* pThis,
        SG_TEXTURE_DESC const* pDesc,
        ISGTexture** ppTexture);

    SG_RESULT (SG_CALL *CreateBottomLevelAccelerationStructure)(
        ISGDevice* pThis,
        SG_AS_BOTTOM_LEVEL_DESC const* pDesc,
        ISGBottomLevelAS** ppBLAS);

    SG_RESULT (SG_CALL *CreateTopLevelAccelerationStructure)(
        ISGDevice* pThis,
        SG_AS_TOP_LEVEL_DESC const* pDesc,
        ISGTopLevelAS** ppTLAS);

    SG_RESULT (SG_CALL *CreateShaderResourceView)(
        ISGDevice* pThis,
        ISGResource* pResource,
        SG_SHADER_RESOURCE_VIEW_DESC const* pViewDesc,
        ISGShaderResourceView** ppShaderResourceView);

    SG_RESULT (SG_CALL *CreateUnorderedAccessView)(
        ISGDevice* pThis,
        ISGResource* pResource,
        SG_UNORDERED_ACCESS_VIEW_DESC const* pViewDesc,
        ISGUnorderedAccessView** ppUnorderedAccessView);

    SG_RESULT (SG_CALL *CreateRenderTargetView)(
        ISGDevice* pThis,
        ISGResource* pResource,
        SG_RENDER_TARGET_VIEW_DESC const* pViewDesc,
        ISGRenderTargetView** ppRenderTargetView);

    SG_RESULT (SG_CALL *CreateDepthStencilView)(
        ISGDevice* pThis,
        ISGResource* pResource,
        SG_DEPTH_STENCIL_VIEW_DESC const* pViewDesc,
        ISGDepthStencilView** ppDepthStencilView);

    SG_RESULT (SG_CALL *CreateSampler)(
        ISGDevice* pThis,
        SG_SAMPLER_DESC const* pDesc,
        ISGSampler** ppSamplerState);

    SG_RESULT (SG_CALL *CreateQuery)(
        ISGDevice* pThis,
        SG_QUERY_TYPE type,
        ISGQuery** ppQuery);

    SG_RESULT (SG_CALL *CreatePredicate)(
        ISGDevice* pThis,
        SG_QUERY_TYPE type,
        ISGPredicate** ppPredicate);

    SG_RESULT (SG_CALL *CreateInputLayout)(
        ISGDevice* pThis,
        SgU32 numElements,
        SG_INPUT_ELEMENT_DESC const* pInputElements,
        ISGInputLayout** ppInputLayout);

    SG_RESULT (SG_CALL *CreateGraphicsPipelineState)(
        ISGDevice* pThis,
        SG_GRAPHICS_PIPELINE_STATE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState);

    SG_RESULT (SG_CALL *CreateMeshPipelineState)(
        ISGDevice* pThis,
        SG_MESH_PIPELINE_STATE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState);

    SG_RESULT (SG_CALL *CreateComputePipelineState)(
        ISGDevice* pThis,
        SG_COMPUTE_PIPELINE_STATE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState);

    SG_RESULT (SG_CALL *CreateRaytracingPipelineState)(
        ISGDevice* pThis,
        SG_RAYTRACING_PIPELINE_DESC const* pDesc,
        ISGPipelineState** ppPipelineState);

    SG_RESULT (SG_CALL *CreateBlendState)(
        ISGDevice* pThis,
        SG_BLEND_STATE_DESC const* pDesc,
        ISGBlendState** ppBlendState);

    SG_RESULT (SG_CALL *CreateDepthStencilState)(
        ISGDevice* pThis,
        SG_DEPTH_STENCIL_STATE_DESC const* pDesc,
        ISGDepthStencilState** ppDepthStencilState);

    SG_RESULT (SG_CALL *CreateRasterizerState)(
        ISGDevice* pThis,
        SG_RASTERIZER_STATE_DESC const* pDesc,
        ISGRasterizerState** ppRasterizerState);
} ISGDeviceVtbl;

SG_INTERFACE ISGDevice
{
    ISGDeviceVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGExecutionContext : public ISGObject
{
public:
    virtual void SG_CALL BeginFrame() = 0;
    virtual void SG_CALL EndFrame() = 0;

    virtual void SG_CALL EndFrame1(
        SgU8 numSwapChains,
        ISGSwapChain* const* ppSwapChains) = 0;

    virtual SG_RESULT SG_CALL WaitForIdle() = 0;

    virtual SG_RESULT SG_CALL GetData(
        ISGQuery* pQuery,
        void** ppData,
        SgU32 dataSize) = 0;

    virtual SG_RESULT SG_CALL ScheduleCommandList(
        SgU8 queueIndex,
        SgU16 timeIndex,
        ISGCommandList** ppOutCommandList) = 0;

    virtual SG_RESULT SG_CALL FinishCommandList(
        ISGCommandList* pCommandList) = 0;

    virtual SG_RESULT SG_CALL GetTimestampFrequency(
        SgU8 queueIndex,
        SgU64* pOutFrequency) = 0;

    virtual SG_RESULT SG_CALL GetClockCalibration(
        SgU8 queueIndex,
        SG_QUEUE_CLOCK_CALIBRATION* pOutClockCalibration) = 0;
};

#else
typedef struct ISGExecutionContextVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGExecutionContext* pThis);

    SgU32 (SG_CALL *Release)(
        ISGExecutionContext* pThis);

    // ISGExecutionContext
    void (SG_CALL *BeginFrame)(
        ISGExecutionContext* pThis);

    void (SG_CALL *EndFrame)(
        ISGExecutionContext* pThis);

    void (SG_CALL *EndFrame1)(
        ISGExecutionContext* pThis,
        SgU8 numSwapChains,
        ISGSwapChain* const* ppSwapChains);

    SG_RESULT (SG_CALL *WaitForIdle)(
        ISGExecutionContext* pThis);

    SG_RESULT (SG_CALL *GetData)(
        ISGExecutionContext* pThis,
        ISGQuery* pQuery,
        void** ppData,
        SgU32 dataSize);

    SG_RESULT (SG_CALL *ScheduleCommandList)(
        ISGExecutionContext* pThis,
        SgU8 queueIndex,
        SgU16 timeIndex,
        ISGCommandList** ppOutCommandList);

    SG_RESULT (SG_CALL *FinishCommandList)(
        ISGExecutionContext* pThis,
        ISGCommandList* pPassContext);

    SG_RESULT (SG_CALL *GetTimestampFrequency)(
        ISGExecutionContext* pThis,
        SgU8 queueIndex,
        SgU64* pOutFrequency);

    SG_RESULT (SG_CALL *GetClockCalibration)(
        ISGExecutionContext* pThis,
        SgU8 queueIndex,
        SG_QUEUE_CLOCK_CALIBRATION* pOutClockCalibration);
} ISGExecutionContextVtbl;

SG_INTERFACE ISGExecutionContext
{
    ISGExecutionContextVtbl *pVtbl;
};
#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGCommandList : public ISGObject
{
public:
    virtual SG_QUEUE_TYPE SG_CALL GetType() = 0;

    // Markers
    virtual void SG_CALL BeginEvent(
        SG_COLOR_3I color,
        char const* pEventName) = 0;

    virtual void SG_CALL EndEvent() = 0;

    // Queries
    virtual void SG_CALL BeginQuery(
        ISGQuery* pQuery) = 0;

    virtual void SG_CALL EndQuery(
        ISGQuery* pQuery) = 0;

    virtual void SG_CALL TimeStamp(
        ISGQuery* ppQuery) = 0;

    virtual void SG_CALL SetPredication(
        ISGPredicate* pPredicate, 
        SG_PREDICATION_OP predication) = 0;

    // Render Target and Depth Stencil
    virtual void SG_CALL SetRenderTarget(
        SgU32 rtIndex, 
        ISGRenderTargetView* pView) = 0;

    virtual void SG_CALL SetDepthStencil(
        ISGDepthStencilView* pView) = 0;

    virtual void SG_CALL ClearRenderTargetDefault(
        ISGRenderTargetView* pRTV) = 0;

    virtual void SG_CALL ClearRenderTarget(
        ISGRenderTargetView* pRTV, 
        SG_COLOR_4F const* pColor) = 0;

    virtual void SG_CALL ClearDepthStencilDefault(
        ISGDepthStencilView* pDSV, 
        SG_CLEAR_FLAGS flags) = 0;

    virtual void SG_CALL ClearDepthStencil(
        ISGDepthStencilView* pDSV, 
        SG_CLEAR_FLAGS flags, 
        float depthValue, 
        SgU8 stencilValue) = 0;

    virtual void SG_CALL SetStencilRef(
        SgU8 stencilRef) = 0;

    virtual void SG_CALL SetViewports(
        SgU32 numViewports,
        SG_VIEWPORT const* pViewports) = 0;

    virtual void SG_CALL SetScissorRects(
        SgU32 numRects,
        SG_RECT const* pRects) = 0;

    // Pipeline State
    virtual void SG_CALL SetPipelineState(
        ISGPipelineState* pPipelineState) = 0;

    virtual void SG_CALL SetShadingRate(
        SG_SHADING_RATE baseShadingRate,
        SG_SHADING_RATE_COMBINER const* pCombiners) = 0;

    virtual void SG_CALL SetShadingRateImage(
        ISGTexture* pImage) = 0;

    virtual void SG_CALL SetBlendState(
        ISGBlendState* pBlendState,
        SgU32 sampleMask) = 0;

    virtual void SG_CALL SetBlendFactor(
        SG_COLOR_4F blendFactor) = 0;

    virtual void SG_CALL SetDepthStencilState(
        ISGDepthStencilState* pDepthStencilState) = 0;

    virtual void SG_CALL SetRasterizerState(
        ISGRasterizerState* pRasterizerState) = 0;

    // Binding
    virtual void SG_CALL SetConstantBuffer(
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGResource* pBuffer) = 0;

    virtual void SG_CALL SetConstantBuffers(
        SgU32 paramIdx,
        SgU32 offset,
        SgU32 count,
        ISGResource** ppBuffers) = 0;

    virtual void SG_CALL SetShaderResource(
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGShaderResourceView* pView) = 0;

    virtual void SG_CALL SetShaderResources(
        SgU32 paramIdx,
        SgU32 offset,
        SgU32 count,
        ISGShaderResourceView** ppViews) = 0;

    virtual void SG_CALL SetUnorderedAccessView(
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGUnorderedAccessView * pView) = 0;

    virtual void SG_CALL SetUnorderedAccessViews(
        SgU32 paramIdx,
        SgU32 offset,
        SgU32 count,
        ISGUnorderedAccessView** ppViews) = 0;

    virtual void SG_CALL SetAccelerationStructure(
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGTopLevelAS* pTLAS) = 0;

    virtual void SG_CALL SetAccelerationStructures(
        SgU32 paramIdx, 
        SgU32 offset,
        SgU32 count,
        ISGTopLevelAS** pTLASes) = 0;

    virtual void SG_CALL SetSampler(
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGSampler* pState) = 0;

    virtual void SG_CALL SetSamplers(
        SgU32 paramIdx, 
        SgU32 offset,
        SgU32 count,
        ISGSampler** ppStates) = 0;

    // Geometry
    virtual void SG_CALL SetVertexBuffer(
        SgU32 slot,
        ISGBuffer* pVertexBuffer,
        SgU32 offset,
        SgU32 stride) = 0;

    virtual void SG_CALL SetVertexBuffers(
        SgU32 startSlot,
        SgU32 numBuffers,
        ISGBuffer** ppVertexBuffers,
        SgU32* pOffsets,
        SgU32* pStrides) = 0;

    virtual void SG_CALL SetIndexBuffer(
        ISGBuffer* pIndexBuffer, 
        SgU32 offset, 
        SG_FORMAT format) = 0;

    virtual void SG_CALL SetPrimitiveTopology(
        SG_PRIMITIVE_TOPOLOGY primitiveTopology) = 0;

    // Graphics context
    virtual void SG_CALL DrawInstanced(
        SgU32 vertexCount,
        SgU32 instanceCount,
        SgU32 startVertexLocation,
        SgU32 startInstanceLocation) = 0;

    virtual void SG_CALL DrawIndexedInstanced(
        SgU32 indexCountPerInstance,
        SgU32 instanceCount,
        SgU32 startIndexLocation,
        int baseVertexLocation,
        SgU32 startInstanceLocation) = 0;

    virtual void SG_CALL DispatchMesh(
        SgU32 threadGroupCountX,
        SgU32 threadGroupCountY,
        SgU32 threadGroupCountZ) = 0;

    // Compute context
    virtual void SG_CALL Dispatch(
        SgU32 threadGroupCountX,
        SgU32 threadGroupCountY,
        SgU32 threadGroupCountZ) = 0;

    // Indirect calls
    virtual void SG_CALL DrawInstancedIndirect(
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset) = 0;

    virtual void SG_CALL DrawIndexedInstancedIndirect(
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset) = 0;

    virtual void SG_CALL DispatchMeshIndirect(
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset) = 0;

    virtual void SG_CALL DispatchIndirect(
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset) = 0;

    // Copy context
    virtual void SG_CALL CopyResource(
        ISGResource* pDstResource, 
        ISGResource* pSrcResource) = 0;

    virtual void SG_CALL CopySubresource(
        ISGSubresource* pDstSubresource, 
        ISGSubresource* pSrcSubresource) = 0;

    virtual void SG_CALL ResolveSubresource(
        ISGSubresource* pDstSubresource,
        ISGSubresource* pSrcSubresource,
        SG_FORMAT format) = 0;

    virtual void SG_CALL CopyBufferRegion(
        ISGBuffer* pDstBuffer, 
        SgU64 destOffset, 
        ISGBuffer* pSrcBuffer, 
        SgU64 srcOffset, 
        SgU64 numBytes) = 0;

    virtual void SG_CALL CopyTextureRegion(
        ISGTexture* pDstTexture, 
        SG_TEXTURE_COPY_DESTINATION const* pDestRegion, 
        ISGTexture* pSrcTexture, 
        SG_TEXTURE_COPY_SOURCE const* pSrcRegion) = 0;

    // Ray tracing
    virtual void SG_CALL BuildBottomLevelAS(
        ISGBottomLevelAS* pBLAS) = 0;

    virtual void SG_CALL BuildTopLevelAS(
        ISGTopLevelAS* pTLAS) = 0;

    virtual void SG_CALL DispatchRays(
        SgU32 width, 
        SgU32 height,
        SgU32 depth) = 0;
};

#else
typedef struct ISGCommandListVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGCommandList* pThis);

    SgU32 (SG_CALL *Release)(
        ISGCommandList* pThis);

    // ISGCommandList
    SG_QUEUE_TYPE (SG_CALL *GetType)(
        ISGCommandList* pThis);

    // Markers
    void (SG_CALL *BeginEvent)(
        ISGCommandList* pThis,
        SG_COLOR_3I color,
        char const* pEventName);

    void (SG_CALL *EndEvent)(
        ISGCommandList* pThis);

    // Queries
    void (SG_CALL *BeginQuery)(
        ISGCommandList* pThis,
        ISGQuery* pQuery);

    void (SG_CALL *EndQuery)(
        ISGCommandList* pThis,
        ISGQuery* pQuery);

    void (SG_CALL *TimeStamp)(
        ISGCommandList* pThis,
        ISGQuery* ppQuery);

    void (SG_CALL *SetPredication)(
        ISGCommandList* pThis,
        ISGPredicate* pPredicate, 
        SG_PREDICATION_OP predication);

    // Render Target and Depth Stencil
    void (SG_CALL *SetRenderTarget)(
        ISGCommandList* pThis,
        SgU32 rtIndex, 
        ISGRenderTargetView* pView);

    void (SG_CALL *SetDepthStencil)(
        ISGCommandList* pThis,
        ISGDepthStencilView* pView);

    void (SG_CALL *ClearRenderTargetDefault)(
        ISGCommandList* pThis,
        ISGRenderTargetView* pRTV);

    void (SG_CALL *ClearRenderTarget)(
        ISGCommandList* pThis,
        ISGRenderTargetView* pRTV, 
        SG_COLOR_4F const* pColor);

    void (SG_CALL *ClearDepthStencilDefault)(
        ISGCommandList* pThis,
        ISGDepthStencilView* pDSV, 
        SG_CLEAR_FLAGS flags);

    void (SG_CALL *ClearDepthStencil)(
        ISGCommandList* pThis,
        ISGDepthStencilView* pDSV, 
        SG_CLEAR_FLAGS flags, 
        float depthValue, 
        SgU8 stencilValue);

    void (SG_CALL *SetStencilRef)(
        ISGCommandList* pThis,
        SgU8 stencilRef);

    void (SG_CALL *SetViewports)(
        ISGCommandList* pThis,
        SgU32 numViewports,
        SG_VIEWPORT const* pViewports);

    void (SG_CALL *SetScissorRects)(
        ISGCommandList* pThis,
        SgU32 numRects,
        SG_RECT const* pRects);

    // Pipeline State
    void (SG_CALL *SetPipelineState)(
        ISGCommandList* pThis,
        ISGPipelineState* pPipelineState);

    void (SG_CALL *SetShadingRate)(
        SG_SHADING_RATE baseShadingRate,
        SG_SHADING_RATE_COMBINER const* pCombiners);

    void (SG_CALL *SetShadingRateImage)(
        ISGTexture* pImage);

    void (SG_CALL *SetBlendState)(
        ISGCommandList* pThis,
        ISGBlendState* pBlendState,
        SgU32 sampleMask);

    void (SG_CALL *SetBlendFactor)(
        ISGCommandList* pThis,
        SG_COLOR_4F blendFactor);

    void (SG_CALL *SetDepthStencilState)(
        ISGCommandList* pThis,
        ISGDepthStencilState* pDepthStencilState);

    void (SG_CALL *SetRasterizerState)(
        ISGCommandList* pThis,
        ISGRasterizerState* pRasterizerState);

    // Binding
    void (SG_CALL *SetConstantBuffer)(
        ISGCommandList* pThis,
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGResource* pBuffer);

    void (SG_CALL *SetConstantBuffers)(
        ISGCommandList* pThis,
        SgU32 paramIdx,
        SgU32 offset,
        SgU32 count,
        ISGResource** ppBuffers);

    void (SG_CALL *SetShaderResource)(
        ISGCommandList* pThis,
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGShaderResourceView* pView);

    void (SG_CALL *SetShaderResources)(
        ISGCommandList* pThis,
        SgU32 paramIdx,
        SgU32 offset,
        SgU32 count,
        ISGShaderResourceView** ppViews);

    void (SG_CALL *SetUnorderedAccessView)(
        ISGCommandList* pThis,
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGUnorderedAccessView * pView);

    void (SG_CALL *SetUnorderedAccessViews)(
        ISGCommandList* pThis,
        SgU32 paramIdx,
        SgU32 offset,
        SgU32 count,
        ISGUnorderedAccessView** ppViews);

    void (SG_CALL *SetAccelerationStructure)(
        ISGCommandList* pThis,
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGTopLevelAS* pTLAS);

    void (SG_CALL *SetAccelerationStructures)(
        ISGCommandList* pThis,
        SgU32 paramIdx, 
        SgU32 offset,
        SgU32 count,
        ISGTopLevelAS** pTLASes);

    void (SG_CALL *SetSampler)(
        ISGCommandList* pThis,
        SgU32 paramIdx, 
        SgU32 bindPoint, 
        ISGSampler* pState);

    void (SG_CALL *SetSamplers)(
        ISGCommandList* pThis,
        SgU32 paramIdx, 
        SgU32 offset,
        SgU32 count,
        ISGSampler** ppStates);

    // Geometry
    void (SG_CALL *SetVertexBuffer)(
        ISGCommandList* pThis,
        SgU32 slot,
        ISGBuffer* pVertexBuffer,
        SgU32 offset,
        SgU32 stride);

    void (SG_CALL *SetVertexBuffers)(
        ISGCommandList* pThis,
        SgU32 startSlot,
        SgU32 numBuffers,
        ISGBuffer** ppVertexBuffers,
        SgU32* pOffsets,
        SgU32* pStrides);

    void (SG_CALL *SetIndexBuffer)(
        ISGCommandList* pThis,
        ISGBuffer* pIndexBuffer, 
        SgU32 offset, 
        SG_FORMAT format);

    void (SG_CALL *SetPrimitiveTopology)(
        ISGCommandList* pThis,
        SG_PRIMITIVE_TOPOLOGY primitiveTopology);

    // Graphics context
    void (SG_CALL *DrawInstanced)(
        ISGCommandList* pThis,
        SgU32 vertexCount,
        SgU32 instanceCount,
        SgU32 startVertexLocation,
        SgU32 startInstanceLocation);

    void (SG_CALL *DrawIndexedInstanced)(
        ISGCommandList* pThis,
        SgU32 indexCountPerInstance,
        SgU32 instanceCount,
        SgU32 startIndexLocation,
        int baseVertexLocation,
        SgU32 startInstanceLocation);

    void (SG_CALL *DispatchMesh)(
        ISGCommandList* pThis,
        SgU32 threadGroupCountX,
        SgU32 threadGroupCountY,
        SgU32 threadGroupCountZ);

    // Compute context
    void (SG_CALL *Dispatch)(
        ISGCommandList* pThis,
        SgU32 threadGroupCountX,
        SgU32 threadGroupCountY,
        SgU32 threadGroupCountZ);

    // Indirect calls
    void (SG_CALL *DrawInstancedIndirect)(
        ISGCommandList* pThis,
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset);

    void (SG_CALL *DrawIndexedInstancedIndirect)(
        ISGCommandList* pThis,
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset);

    void (SG_CALL *DispatchMeshIndirect)(
        ISGCommandList* pThis,
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset);

    void (SG_CALL *DispatchIndirect)(
        ISGCommandList* pThis,
        SgU32 maxCommandCount, 
        ISGBuffer* pArgBuffer, 
        SgU32 argBufferOffset);

    // Copy context
    void (SG_CALL *CopyResource)(
        ISGCommandList* pThis,
        ISGResource* pDstResource, 
        ISGResource* pSrcResource);

    void (SG_CALL *CopySubresource)(
        ISGCommandList* pThis,
        ISGSubresource* pDstSubresource, 
        ISGSubresource* pSrcSubresource);

    void (SG_CALL *ResolveSubresource)(
        ISGCommandList* pThis,
        ISGSubresource* pDstSubresource,
        ISGSubresource* pSrcSubresource,
        SG_FORMAT format);

    void (SG_CALL *CopyBufferRegion)(
        ISGCommandList* pThis,
        ISGBuffer* pDstBuffer, 
        SgU64 destOffset, 
        ISGBuffer* pSrcBuffer, 
        SgU64 srcOffset, 
        SgU64 numBytes);

    void (SG_CALL *CopyTextureRegion)(
        ISGCommandList* pThis,
        ISGTexture* pDstTexture, 
        SG_TEXTURE_COPY_DESTINATION const* pDestRegion, 
        ISGTexture* pSrcTexture, 
        SG_TEXTURE_COPY_SOURCE const* pSrcRegion);

    // Ray tracing
    void (SG_CALL *BuildBottomLevelAS)(
        ISGCommandList* pThis,
        ISGBottomLevelAS* pBLAS);

    void (SG_CALL *BuildTopLevelAS)(
        ISGCommandList* pThis,
        ISGTopLevelAS* pTLAS);

    void (SG_CALL *DispatchRays)(
        ISGCommandList* pThis,
        SgU32 width, 
        SgU32 height,
        SgU32 depth);
} ISGCommandListVtbl;

SG_INTERFACE ISGCommandList
{
    ISGCommandListVtbl * pVtbl;
};

#endif

typedef SG_RESULT (SG_CALL* PFN_SGLIB_INITIALIZE)(SG_DEBUG_CONFIG const* pDesc);
typedef SG_RESULT (SG_CALL* PFN_SGLIB_DESTROY)();

typedef SG_RESULT (SG_CALL* PFN_SG_ENUM_ADAPTERS)(SgU32 index, ISGAdapter** ppAdapter);

typedef SG_RESULT (SG_CALL* PFN_SG_CREATE_DEVICE)(
    ISGAdapter*                 pAdapter,
    SG_DEVICE_DESC const*       pDesc,
    ISGDevice**                 ppDevice);

typedef SG_RESULT (SG_CALL* PFN_SG_CREATE_SWAP_CHAIN)(
    ISGExecutionContext*        pExecutionContext,
    SgU8                        queueIndex,
    SG_SWAP_CHAIN_DESC const*   pDesc,
    ISGSwapChain**              ppSwapChain);

typedef SgU32 (SG_CALL* PFN_SG_GET_FORMAT_SIZE)(SG_FORMAT format);

SG_API_FUNC SG_RESULT SG_CALL SGLibInitialize(SG_DEBUG_CONFIG const* pDesc);
SG_API_FUNC SG_RESULT SG_CALL SGLibDestroy();

SG_API_FUNC SG_RESULT SG_CALL SgEnumAdapters(SgU32 index, ISGAdapter** ppAdapter);

SG_API_FUNC SG_RESULT SG_CALL SgCreateDevice(
    ISGAdapter*                 pAdapter,
    SG_DEVICE_DESC const*       pDesc,
    ISGDevice**                 ppDevice);

SG_API_FUNC SG_RESULT SG_CALL SgCreateSwapChain(
    ISGExecutionContext*        pExecutionContext,
    SgU8                        queueIndex,
    SG_SWAP_CHAIN_DESC const*   pDesc,
    ISGSwapChain**              ppSwapChain);

SG_API_FUNC SgU32 SG_CALL SgGetFormatSize(SG_FORMAT format);

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
