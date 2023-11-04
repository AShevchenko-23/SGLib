
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
// Declarations
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SG_DECLARE_CLASS(ISGQuery);
SG_DECLARE_CLASS(ISGPredicate);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum SG_QUERY_TYPE
{
    SG_QUERY_TYPE_OCCLUSION = 0,
    SG_QUERY_TYPE_BINARY_OCCLUSION = 1,
    SG_QUERY_TYPE_TIMESTAMP = 2,
    SG_QUERY_TYPE_PIPELINE_STATISTICS = 3,
} SG_QUERY_TYPE;

typedef enum SG_PREDICATION_OP
{
    SG_PREDICATION_OP_EQUAL_ZERO = 0,
    SG_PREDICATION_OP_NOT_EQUAL_ZERO = 1,
} SG_PREDICATION_OP;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Classes
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef C_STYLE
SG_INTERFACE SG_API ISGQuery : public ISGObject
{
public:
};

#else
typedef struct ISGQueryVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGQuery* pThis);

    SgU32 (SG_CALL *Release)(
        ISGQuery* pThis);

} ISGQueryVtbl;

SG_INTERFACE ISGQuery
{
    ISGQueryVtbl* pVtbl;
};

#endif

#ifndef C_STYLE
SG_INTERFACE SG_API ISGPredicate : public ISGQuery
{
public:
};

#else
typedef struct ISGPredicateVtbl
{
    // ISGObject
    SgU32 (SG_CALL *AddRef)(
        ISGPredicate* pThis);

    SgU32 (SG_CALL *Release)(
        ISGPredicate* pThis);

} ISGPredicateVtbl;

SG_INTERFACE ISGPredicate
{
    ISGPredicateVtbl* pVtbl;
};

#endif

#if defined(__cplusplus) && !defined(CINTERFACE)
} // extern "C"
#endif
