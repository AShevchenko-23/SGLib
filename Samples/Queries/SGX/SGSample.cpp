
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

#include "SGSample.h"

void CreateForm(ISGSample* pSample, HWND* pHwnd);

ISGSample::ISGSample(U32 width, U32 height, wchar_t const* windowName)
    : m_hWnd(NULL)
    , m_Width(width)
    , m_Height(height)
    , m_Name(windowName)
{
}

int ISGSample::Run(int nCmdShow, SG_DEBUG_LEVELS debugLevel, SG_DEBUG_TOOLS debugTools)
{
    SG_DEBUG_CONFIG debugConfig{};
    debugConfig.DebugLevel = debugLevel;
    debugConfig.DebugTools = debugTools;

    if (SGLibInitialize(&debugConfig) != SG_OK)
        throw std::exception("Failed to initialize SGLib");

    CreateForm(this, &m_hWnd);
    ShowWindow(m_hWnd, nCmdShow);

    OnInit();

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    OnDestroy();
    SGLibDestroy();

    return static_cast<int>(msg.wParam);
}

SG_RESULT ISGSample::GetVideoAdapter(ISGAdapter** ppAdapter)
{
    U32 index = 0;
    ISGAdapter* pCandidate = nullptr;

    while (SgEnumAdapters(index++, &pCandidate) != SG_ERROR_NOT_FOUND)
    {
        *ppAdapter = pCandidate;
        return SG_OK;
    }

    return SG_ERROR_INTERNAL;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ISGSample* pSample = reinterpret_cast<ISGSample*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (msg)
    {
    case WM_CREATE:
        {
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
        }
        return 0;
    case WM_KEYDOWN:
        if (pSample)
        {
            pSample->OnKeyUp(static_cast<UINT8>(wParam));
        }
        return 0;
    case WM_KEYUP:
        if (pSample)
        {
            pSample->OnKeyUp(static_cast<UINT8>(wParam));
        }
        return 0;
    case WM_PAINT:
        if (pSample)
        {
            pSample->OnUpdate();
            pSample->OnRender();
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CreateForm(ISGSample* pSample, HWND* pHwnd)
{
    // Register the window class.
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SGSample";
    RegisterClass(&wc);

    RECT windowRect = { 0, 0, static_cast<LONG>(pSample->GetWidth()), static_cast<LONG>(pSample->GetHeight()) };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    *pHwnd = CreateWindow(
        // Style & class
        wc.lpszClassName,       // Window class
        pSample->GetName(),     // Window text
        WS_OVERLAPPEDWINDOW,    // Window style

        // Size and position
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,

        // Additions
        NULL, NULL, hInstance, pSample
    );
}
