
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

#include "RaytracingSample.h"

#ifdef _DEBUG
    #pragma comment(lib, "../../Lib/SGLib_D3D12_dev.lib")
#else
    #pragma comment(lib, "../../Lib/SGLib_D3D12.lib")
#endif

int main()
{
    RaytracingSample sample(1280, 720, L"Ray tracing sample");
    return sample.Run(1, SG_DEBUG_DISABLED, SG_DEBUG_TOOL_NONE);
}
