
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
#include "Resource.h"

#if defined(__cplusplus) && !defined(CINTERFACE)
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SG_DECLARE_CLASS(ISGBottomLevelAS);
SG_DECLARE_CLASS(ISGTopLevelAS);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SG_AS_GEOMETRY_TYPE
{
    SG_AS_GEOMETRY_TYPE_TRIANGLES = 0,
    SG_AS_GEOMETRY_TYPE_AABBS = 1
} SG_AS_GEOMETRY_TYPE;

typedef enum SG_AS_GEOMETRY_FLAGS
{
    SG_AS_GEOMETRY_FLAG_NONE = 0,
    SG_AS_GEOMETRY_FLAG_OPAQUE = 0x1,
    SG_AS_GEOMETRY_FLAG_NO_DUPLICATE_ANY_HIT_INVOCATION = 0x2
} SG_AS_GEOMETRY_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_AS_GEOMETRY_FLAGS);

typedef enum SG_AS_INSTANCE_FLAGS
{
    SG_AS_INSTANCE_FLAG_NONE = 0,
    SG_AS_INSTANCE_FLAG_TRIANGLE_CULL_DISABLE = 0x1,
    SG_AS_INSTANCE_FLAG_TRIANGLE_FRONT_COUNTER_CLOCKWISE = 0x2,
    SG_AS_INSTANCE_FLAG_FORCE_OPAQUE = 0x4,
    SG_AS_INSTANCE_FLAG_FORCE_NON_OPAQUE = 0x8
} SG_AS_INSTANCE_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_AS_INSTANCE_FLAGS);

typedef enum SG_AS_BUILD_FLAGS
{
    SG_AS_BUILD_FLAG_NONE = 0,
    SG_AS_BUILD_FLAG_ALLOW_UPDATE = 0x1,
    SG_AS_BUILD_FLAG_ALLOW_COMPACTION = 0x2,
    SG_AS_BUILD_FLAG_MINIMIZE_MEMORY = 0x4,
} SG_AS_BUILD_FLAGS;

SG_DEFINE_ENUM_OPERATORS(SG_AS_BUILD_FLAGS);

typedef enum SG_AS_BUILD_PREFERENCES
{
    SG_AS_BUILD_PREFERENCE_NONE = 0,
    SG_AS_BUILD_PREFERENCE_FAST_TRACE = 0x1,
    SG_AS_BUILD_PREFERENCE_FAST_BUILD = 0x2,
} SG_AS_BUILD_PREFERENCES;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structs
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct SG_BUFFER_DATA_REF
{
    ISGBuffer* pBuffer;
    SgU32 ByteOffset;
} SG_BUFFER_DATA_REF;

typedef struct SG_BUFFER_DATA_REF_AND_STRIDE
{
    ISGBuffer* pBuffer;
    SgU32 ByteOffset;
    SgU32 StrideBytes;
} SG_BUFFER_DATA_REF_AND_STRIDE;

typedef struct SG_AS_GEOMETRY_TRIANGLE_DESC
{
    SG_BUFFER_DATA_REF IndexBuffer;
    SG_FORMAT IndexFormat;
    SgU32 IndexCount;

    SG_BUFFER_DATA_REF_AND_STRIDE VertexBuffer;
    SG_FORMAT VertexFormat;
    SgU32 VertexCount;

    SG_BUFFER_DATA_REF Transform3x4;
} SG_AS_GEOMETRY_TRIANGLE_DESC;

typedef struct SG_AS_GEOMETRY_AABBS_DESC
{
    SgU64 AABBCount;
    SG_BUFFER_DATA_REF_AND_STRIDE AABBs;
} SG_AS_GEOMETRY_AABBS_DESC;

typedef struct SG_AS_GEOMETRY_DESC
{
    SG_AS_GEOMETRY_TYPE Type;
    SG_AS_GEOMETRY_FLAGS Flags;
    union
    {
        SG_AS_GEOMETRY_TRIANGLE_DESC Triangles;
        SG_AS_GEOMETRY_AABBS_DESC AABBs;
    };
} SG_AS_GEOMETRY_DESC;

typedef struct SG_AS_INSTANCE_DESC
{
    float Transform[3][4];
    SgU32 InstanceID : 24;
    SgU32 InstanceMask : 8;
    SgU32 InstanceContributionToHitGroupIndex : 24;
    SgU32 Flags : 8;
    ISGBottomLevelAS* pAccelerationStructure;
} SG_AS_INSTANCE_DESC;

typedef struct SG_AS_BOTTOM_LEVEL_DESC
{
    SG_AS_BUILD_FLAGS Flags;
    SG_AS_BUILD_PREFERENCES Preferences;
    SgU32 NumDescs;
    const SG_AS_GEOMETRY_DESC* pGeometryDescs;
} SG_AS_BOTTOM_LEVEL_DESC;

typedef struct SG_AS_TOP_LEVEL_DESC
{
    SG_AS_BUILD_FLAGS Flags;
    SG_AS_BUILD_PREFERENCES Preferences;
    SgU32 NumDescs;
    const SG_AS_INSTANCE_DESC* pInstanceDescs;
} SG_AS_TOP_LEVEL_DESC;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef C_STYLE
SG_INTERFACE SG_API ISGBottomLevelAS : public ISGObject
{
public:
    virtual SG_RESULT SG_CALL Fill(
        SgU32 NumDescs,
        const SG_AS_GEOMETRY_DESC* pGeometryDescs,
        SgBool performUpdate,
        ISGBottomLevelAS* pSourceBLAS) = 0;
};

#else
typedef struct ISGBottomLevelASVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGTopLevelAS* pThis);

    SgU32 (SG_CALL *Release)(
        ISGTopLevelAS* pThis);

    // ISGBottomLevelAS
    SG_RESULT (SG_CALL *Fill)(
        ISGTopLevelAS* pThis,
        SgU32 NumDescs,
        const SG_AS_GEOMETRY_DESC* pGeometryDescs,
        SgBool performUpdate,
        ISGBottomLevelAS* pSourceBLAS);

} ISGBottomLevelASVtbl;

SG_INTERFACE ISGBottomLevelAS
{
    ISGBottomLevelASVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGTopLevelAS : public ISGObject
{
public:
    virtual SG_RESULT SG_CALL Fill(
        SgU32 NumDescs,
        const SG_AS_INSTANCE_DESC* pInstanceDescs,
        SgBool performUpdate,
        ISGTopLevelAS* pSourceTLAS) = 0;
};

#else
typedef struct ISGTopLevelASVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGTopLevelAS* pThis);
    SgU32 (SG_CALL *Release)(
        ISGTopLevelAS* pThis);

    // ISGTopLevelAS
    SG_RESULT (SG_CALL *Fill)(
        ISGTopLevelAS* pThis,
        SgU32 NumDescs,
        const SG_AS_INSTANCE_DESC* pInstanceDescs,
        SgBool performUpdate,
        ISGTopLevelAS* pSourceTLAS);

} ISGTopLevelASVtbl;

SG_INTERFACE ISGTopLevelAS
{
    ISGTopLevelASVtbl* pVtbl;
};

#endif

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
