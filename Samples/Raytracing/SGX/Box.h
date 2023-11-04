
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

#include <DirectXMath.h>

struct Vertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT2 TextureUV;
};

const Vertex g_pBoxVertices[] =
{
    // Bottom
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
    { { 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f } },
    { { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f } },
    { { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f } },

    // Top
    { { -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f } },
    { { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
    { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },

    // Left
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f } },
    { { -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f } },
    { { -1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f } },
    { { -1.0f,  1.0f, 1.0f }, { 1.0f, 0.0f } },

    // Right
    { { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
    { { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f } },
    { { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f } },
    { { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

    // Front
    { { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f } },
    { { 1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f } },
    { { -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f } },
    { { 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f } },

    // Back
    { { -1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f } },
    { { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f } },
    { { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
    { { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
};

const uint16_t g_pBoxIndices[] =
{
    0, 2, 1,
    1, 2, 3,

    4, 5, 6,
    7, 6, 5,

    8, 10, 9,
    11, 9, 10,

    12, 13, 14,
    13, 15, 14,

    16, 17, 18,
    17, 19, 18,

    20, 22, 21,
    23, 21, 22,
};
