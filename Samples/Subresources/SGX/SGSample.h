
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

#include <atomic>
#include <Windows.h>
#include <iostream>
#include "SGHelpers.h"

class ISGSample
{
public:
    ISGSample(U32 width, U32 height, wchar_t const* windowName);
    ISGSample(ISGSample const& other) = delete;

    virtual void    OnInit() = 0;
    virtual void    OnDestroy() = 0;

    virtual void    OnKeyDown(uint8_t key) {}
    virtual void    OnKeyUp(uint8_t key) {}

    virtual void    OnUpdate() = 0;
    virtual void    OnRender() = 0;

    int             Run(int nCmdShow, SG_DEBUG_LEVELS debugLevel, SG_DEBUG_TOOLS debugTools);

    U32             GetWidth() const { return m_Width; }
    U32             GetHeight() const { return m_Height; }
    wchar_t const*  GetName() const { return m_Name.c_str(); }

protected:
    SG_RESULT       GetVideoAdapter(ISGAdapter** ppAdapter);

protected:
    HWND            m_hWnd;
    U32             m_Width;
    U32             m_Height;
    std::wstring    m_Name;
};

#define SetWindowTitle(hWnd, format, ...) \
    { \
        char textBuffer[24]; \
        sprintf_s(textBuffer, _countof(textBuffer), format, __VA_ARGS__); \
        SetWindowTextA(hWnd, textBuffer); \
    }
