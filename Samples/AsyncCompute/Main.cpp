
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

#include "AsyncCompute.h"

#ifdef _DEBUG
    #pragma comment(lib, "../../Lib/SGLib_D3D12_dev.lib")
    #define DEBUG_LEVEL SG_DEBUG_L1
#else
    #pragma comment(lib, "../../Lib/SGLib_D3D12.lib")
    #define DEBUG_LEVEL SG_DEBUG_DISABLED
#endif

int main()
{
    AsyncComputeSample sample(1280, 720, L"Async compute sample");
    return sample.Run(1, DEBUG_LEVEL, SG_DEBUG_TOOL_NONE);
}
