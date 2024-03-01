
//*********************************************************
//
// Copyright (c) 2024 Aleksei Shevchenko.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once

#if !defined(__cplusplus) || defined(CINTERFACE)
    #define C_STYLE
#endif

#ifdef SG_EXPORTS
#define SG_API __declspec(novtable)
#define SG_API_FUNC __declspec(dllexport)
#else
#define SG_API __declspec(novtable)
#define SG_API_FUNC __declspec(dllimport)
#endif

#define SG_INTERFACE struct
#define SG_DECLARE_CLASS(ClassName) typedef struct ClassName ClassName
#define SG_CALL __stdcall

#if defined(__cplusplus) && !defined(CINTERFACE)
extern "C" {
#endif

typedef unsigned long long  SgU64;
typedef unsigned int        SgU32;
typedef unsigned short      SgU16;
typedef unsigned char       SgU8;
typedef signed long long    SgI64;
typedef signed int          SgI32;
typedef signed short        SgI16;
typedef signed char         SgI8;

#define SG_U64_MAX 0xFFFFFFFFFFFFFFFFui64
#define SG_U32_MAX 0xFFFFFFFFui32
#define SG_U16_MAX 0xFFFFui16
#define SG_U8_MAX 0xFFui8

#define SG_NULL 0x0ull

typedef SgU32 SgBool;
#define SG_TRUE 1
#define SG_FALSE 0

#if !defined(C_STYLE)
#define SG_DEFINE_ENUM_OPERATORS(ENUM_TYPE) \
    extern "C++" { \
    inline ENUM_TYPE operator&(ENUM_TYPE left, ENUM_TYPE right) { return ENUM_TYPE(SgU32(left) & SgU32(right)); } \
    inline ENUM_TYPE operator|(ENUM_TYPE left, ENUM_TYPE right) { return ENUM_TYPE(SgU32(left) | SgU32(right)); } \
    inline ENUM_TYPE& operator&=(ENUM_TYPE& left, ENUM_TYPE right) { return (ENUM_TYPE&)((SgU32&)left &= (SgU32)right); } \
    inline ENUM_TYPE operator|=(ENUM_TYPE& left, ENUM_TYPE right) { return (ENUM_TYPE&)((SgU32&)left |= (SgU32)right); } \
    inline bool operator==(ENUM_TYPE left, SgU32 right) { return SgU32(left) == right; } \
    inline bool operator!=(ENUM_TYPE left, SgU32 right) { return SgU32(left) != right; } \
    inline bool operator==(ENUM_TYPE left, ENUM_TYPE right) { return SgU32(left) == SgU32(right); } \
    inline bool operator!=(ENUM_TYPE left, ENUM_TYPE right) { return SgU32(left) != SgU32(right); } \
    inline ENUM_TYPE operator~(ENUM_TYPE left) { return ENUM_TYPE(~SgU32(left)); } \
    }

#else
#define SG_DEFINE_ENUM_OPERATORS(ENUM_TYPE)

#endif

#define SG_RESULT_CAT_COMMON    0
#define SG_RESULT_CAT_DEVICE    1
#define SG_RESULT_CAT_EXEC_CTX  2
#define SG_RESULT_CAT_RESOURCE  3
#define SG_RESULT_CAT_VIEW      4
#define SG_RESULT_CAT_PIPELINE  5

#define SG_DECLARE_WARNING_CODE( category, code ) ( ( category << 24 ) | ( code & 0xffff ) )
#define SG_DECLARE_ERROR_CODE( category, code ) -( SG_DECLARE_WARNING_CODE( category, code ) + 1)

typedef enum SG_RESULT
{
    SG_OK                                           = SG_DECLARE_WARNING_CODE( SG_RESULT_CAT_COMMON, 0 ),

    // WARNINGS
    SG_MESSAGE_ALREADY_DONE                         = SG_DECLARE_WARNING_CODE( SG_RESULT_CAT_COMMON, 1 ),

    // ERRORS
    SG_ERROR_API                                    = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 0 ),
    SG_ERROR_INTERNAL                               = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 1 ),
    SG_ERROR_INVALID_ARG                            = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 2 ),
    SG_ERROR_UNSUPPORTED_FEATURE                    = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 3 ),

    SG_ERROR_OUT_OF_MEMORY                          = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 4 ),

    SG_ERROR_NOT_FOUND                              = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 5 ),
    SG_ERROR_UNRELEASED_RESOURCES                   = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 6 ),
    SG_ERROR_OUT_OF_BOUNDS                          = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 7 ),
    SG_ERROR_OVERLAPPING                            = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_COMMON, 8 ),

    SG_ERROR_NO_EXEC_CTX                            = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_DEVICE, 0 ),
    SG_ERROR_INVALID_QUERY_TYPE                     = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_DEVICE, 1 ),

    SG_ERROR_NO_ACTIVE_FRAME                        = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_EXEC_CTX, 0 ),
    SG_ERROR_INVALID_TIME_INDEX                     = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_EXEC_CTX, 1 ),
    SG_ERROR_CMD_LIST_ALREADY_FINISHED              = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_EXEC_CTX, 2 ),
    SG_ERROR_LOCKED_EXEC_CTX                        = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_EXEC_CTX, 3 ),
    SG_ERROR_QUERY_HAS_NOT_BEEN_EXECUTED            = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_EXEC_CTX, 4 ),
    SG_ERROR_COPY_QUEUE_TIMESTAMPS_NOT_SUPPORTED    = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_EXEC_CTX, 5 ),
    SG_ERROR_IMPROPER_QUERY_OBJECT_TYPE             = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_EXEC_CTX, 6 ),

    SG_ERROR_INVALID_RESOURCE_TYPE                  = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 0 ),
    SG_ERROR_INCOMPATIBLE_ACCESS_TYPE               = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 1 ),
    SG_ERROR_OUT_OF_RESOURCE_BOUNDS                 = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 2 ),

    SG_ERROR_INVALID_FORMAT                         = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 3 ),
    SG_ERROR_BUFFER_INVALID_SIZE                    = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 4 ),
    SG_ERROR_BUFFER_INCOMPATIBLE_BIND_FLAG          = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 5 ),
    SG_ERROR_BUFFER_INVALID_STRIDE                  = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 6 ),

    SG_ERROR_TEXTURE_INVALID_DIMENSION              = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 7 ),
    SG_ERROR_TEXTURE_INVALID_SIZE                   = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 8 ),
    SG_ERROR_TEXTURE_INCOMPATIBLE_BIND_FLAG         = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_RESOURCE, 9 ),

    SG_ERROR_UNKNOWN_VIEW_TYPE                      = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_VIEW, 0 ),
    SG_ERROR_INCONSISTENT_VIEW                      = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_VIEW, 1 ),
    SG_ERROR_RES_VIEW_TYPES_MISMATCH                = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_VIEW, 2 ),

    SG_ERROR_INVALID_SHADER                         = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_PIPELINE, 0 ),
    SG_ERROR_INVALID_ROOT_SIGNATURE_TYPE            = SG_DECLARE_ERROR_CODE( SG_RESULT_CAT_PIPELINE, 1 ),
} SG_RESULT;

typedef struct SG_COLOR_3I
{
    SgU8 R;
    SgU8 G;
    SgU8 B;
} SG_COLOR_3I;

typedef struct SG_COLOR_4F
{
    float R;
    float G;
    float B;
    float A;
} SG_COLOR_4F;

typedef struct SG_RECT
{
    SgI32 Left;
    SgI32 Top;
    SgI32 Right;
    SgI32 Bottom;
} SG_RECT;

SG_DECLARE_CLASS(ISGObject);

#ifndef C_STYLE
SG_INTERFACE SG_API ISGObject
{
public:
    virtual SgU32 SG_CALL AddRef() = 0;
    virtual SgU32 SG_CALL Release() = 0;
};

#else
typedef struct ISGObjectVtbl
{
    SgU32 (SG_CALL *AddRef)(
        ISGObject* pThis);

    SgU32 (SG_CALL *Release)(
        ISGObject* pThis);
} ISGObjectVtbl;

SG_INTERFACE ISGObject
{
    ISGObjectVtbl pVtbl;
};

#endif

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
