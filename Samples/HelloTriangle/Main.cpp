
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

#include <iostream>
#include <vector>
#include <fstream>
#include <exception>
#include <Windows.h>
#include <Include/SGLib.h>

#ifdef _DEBUG
#pragma comment(lib, "../../Lib/SGLib_D3D12_dev.lib")
#else
#pragma comment(lib, "../../Lib/SGLib_D3D12.lib")
#endif

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

struct Vertex
{
    float x, y, z;
    float r, g, b;
};

HWND                    hWnd = NULL;
ISGAdapter*             pAdapter = SG_NULL;
ISGDevice*              pDevice = SG_NULL;
ISGExecutionContext*    pExecutionContext = SG_NULL;
ISGSwapChain*           pSwapChain = SG_NULL;
ISGBuffer*              pVertexBuffer = SG_NULL;
ISGPipelineState*       pPipelineState = SG_NULL;

void GetAdapter(ISGAdapter** ppAdapter);
void CreateDevice(ISGAdapter* pAdapter, ISGDevice** ppDevice, ISGExecutionContext** ppContext);
void CreateSwapChain(ISGExecutionContext* pContext, HWND hWnd, ISGSwapChain** ppSwapChain);
void InitVertexBuffer(ISGDevice* ppDevice, ISGBuffer** ppVertexBuffer);
void CreatePipelineState(ISGDevice* pDevice, ISGPipelineState** ppPipelineState);
void CreateForm(HWND* pHwnd, int width, int height);

int main()
{
    if (SGLibInitialize(SG_NULL) != SG_OK)
        throw std::exception("SG initializing failed");

    CreateForm(&hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

    GetAdapter(&pAdapter);
    CreateDevice(pAdapter, &pDevice, &pExecutionContext);
    CreateSwapChain(pExecutionContext, hWnd, &pSwapChain);

    InitVertexBuffer(pDevice, &pVertexBuffer);
    CreatePipelineState(pDevice, &pPipelineState);

    ShowWindow(hWnd, 1);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Wait all submitted frames
    pExecutionContext->WaitForIdle();

    // And destroy everything
    pPipelineState->Release();
    pVertexBuffer->Release();
    pSwapChain->Release();
    pExecutionContext->Release();
    pDevice->Release();
    pAdapter->Release();

    SGLibDestroy();
    return 0;
}

void GetAdapter(ISGAdapter** ppAdapter)
{
    ISGAdapter* pAdapter = SG_NULL;

    int index = 0;
    while (SgEnumAdapters(index, &pAdapter) != SG_ERROR_NOT_FOUND)
    {
        index++;

        if (pAdapter == SG_NULL) // Unsupported adapter
            continue;

        // Check adapter if it supports features that you need
        SG_ADAPTER_INFO_EXECUTION execInfo;
        pAdapter->GetInfo(SG_ADAPTER_INFO_CAT_EXECUTION, &execInfo);

        // Adapter must support at least one graphics queue
        if (execInfo.GraphicsQueues <= 1)
            continue;

        *ppAdapter = pAdapter;
        return;
    }

    throw std::exception("Failed to get adapter");
}

void CreateDevice(ISGAdapter* pAdapter, ISGDevice** ppDevice, ISGExecutionContext** ppContext)
{
    // Configure execution context:
    //   - Request 1 graphics queue and 2 frame buffer
    //   - One frame buffer means you can't schedule a new frame while the previous one is being processed
    SG_EXECUTION_CONTEXT_DESC execCtxDesc;
    execCtxDesc.FrameBuffers = 2;
    execCtxDesc.QueueCount = 1;
    execCtxDesc.QueueTypes[0] = SG_QUEUE_TYPE_GRAPHICS;

    // Configure device 
    SG_DEVICE_DESC deviceDesc;
    deviceDesc.pDeviceRemovalCallback = SG_NULL;
    deviceDesc.pMemoryDebugConfig = SG_NULL;
    deviceDesc.pExecutionContextDesc = &execCtxDesc;

    if (SgCreateDevice(pAdapter, &deviceDesc, ppDevice) != SG_OK)
        throw std::exception("Failed to create device object");

    ISGExecutionContext* pExecutionContext = SG_NULL;
    if ((*ppDevice)->GetExecutionContext(ppContext) != SG_OK)
        throw std::exception("Failed to get execution context");
}

void CreateSwapChain(ISGExecutionContext* pContext, HWND hWnd, ISGSwapChain** ppSwapChain)
{
    SG_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.OutputWindow = hWnd;                      // Pass window handle
    swapChainDesc.Width = 0;                                // Leave back buffers' size 0
    swapChainDesc.Height = 0;                               // to adjust them to client rect
    swapChainDesc.Format = SG_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferCount = 2;                          // There should be at least 2 buffer
    swapChainDesc.Flags = SG_SWAP_CHAIN_FLAG_ALLOW_TEARING;
    swapChainDesc.SwapEffect = SG_SWAP_EFFECT_SEQUENTIAL;

    // Set the execution context and an index of the queue slot to perform the swap chain on it
    // Look at execution context description, we required a graphics queue in slot #0
    if (SgCreateSwapChain(pContext, 0, &swapChainDesc, ppSwapChain) != SG_OK)
        throw std::exception("Failed to create swap chain");
}

void InitVertexBuffer(ISGDevice* pDevice, ISGBuffer** ppVertexBuffer)
{
    // Init vertex array (first three floats is vertex position, second three is color
    Vertex vertices[3]
    {
        { 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, },
        { -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, },
        { 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, },
    };

    // Create upload buffer to fill it on CPU side
    SG_BUFFER_DESC vbDesc = {};
    vbDesc.Type = SG_BUFFER_TYPE_UPLOAD;
    vbDesc.BindFlags |= SG_BUFFER_BIND_FLAG_VERTEX_BUFFER;
    vbDesc.Size = sizeof(vertices);

    ISGBuffer* pVertexBuffer = SG_NULL;
    if (pDevice->CreateBuffer(&vbDesc, &pVertexBuffer) != SG_OK)
        throw std::exception("Failed to create vertex buffer");

    void* pVBData = nullptr;
    if (pVertexBuffer->Map(&pVBData) != SG_OK)
        throw std::exception("Failed to map vertex buffer data");

    memcpy(pVBData, vertices, sizeof(vertices));
    pVertexBuffer->Unmap();

    *ppVertexBuffer = pVertexBuffer;
}

void CreatePipelineState(ISGDevice* pDevice, ISGPipelineState** ppPipelineState)
{
    SG_INPUT_ELEMENT_DESC inputElements[]
    {
        
        { "POSITION", 0, SG_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(Vertex, x), SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
        { "COLOR", 0, SG_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, r), SG_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
    };

    ISGInputLayout* pInputLayout = SG_NULL;
    if (pDevice->CreateInputLayout(_countof(inputElements), inputElements, &pInputLayout) != SG_OK)
        throw std::exception("Failed to create input layout");

    auto LoadBinary = [](char const* pFilename, std::vector<uint8_t>& outBuffer)
        {
            std::ifstream file(pFilename, std::ios::binary | std::ios::ate);

            if (!file.is_open())
                return false;

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            outBuffer.resize(size);

            if (file.read((char*)outBuffer.data(), size))
            {
                return true;
            }

            return false;
        };

    std::vector<uint8_t> vsBufer;
    if (!LoadBinary("VertexShader.cso", vsBufer))
        throw std::exception("Failed to load vertex shader");

    std::vector<uint8_t> psBufer;
    if (!LoadBinary("PixelShader.cso", psBufer))
        throw std::exception("Failed to load pixel shader");

    SG_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
    psoDesc.pInputLayout = pInputLayout;
    psoDesc.VS = { vsBufer.data(), vsBufer.size() };
    psoDesc.PS = { psBufer.data(), psBufer.size() };

    // As there's no any to bind, init an empty root signature
    psoDesc.RootSignature.Type = SG_ROOT_SIGNATURE_TYPE_TABULAR;
    psoDesc.RootSignature.Tabular.NumTables = 0;
    psoDesc.RootSignature.Tabular.pTables = SG_NULL;

    if (pDevice->CreateGraphicsPipelineState(&psoDesc, ppPipelineState) != SG_OK)
        throw std::exception("Failed to create pipeline state");

    // No need to keep input layout any more
    pInputLayout->Release();
}

void RenderFrame()
{
    pExecutionContext->BeginFrame();

    ISGCommandList* pCommandList = SG_NULL;

    // Schedule command list on #0-queue (graphics) at TimeIndex #1 - the first available time index
    pExecutionContext->ScheduleCommandList(0, 1, &pCommandList);

    // Clear the current swap chain buffer and set it as render target
    SG_COLOR_4F clearColor = { 0.7f , 0.8f, 1.0f, 1.0f };
    pCommandList->ClearRenderTarget(pSwapChain->GetCurrentRTV(), &clearColor);
    pCommandList->SetRenderTarget(0, pSwapChain->GetCurrentRTV());

    // Set-up frame parameters (viewport and scissor rect)
    SG_VIEWPORT viewport{ 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f , 1.0f };
    pCommandList->SetViewports(1, &viewport);

    SG_RECT scissor{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    pCommandList->SetScissorRects(1, &scissor);

    // Set-up pipeline state, primitive topology and vertex buffer
    pCommandList->SetPipelineState(pPipelineState);
    pCommandList->SetPrimitiveTopology(SG_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pCommandList->SetVertexBuffer(0, pVertexBuffer, 0, sizeof(Vertex));

    pCommandList->DrawInstanced(3, 1, 0, 0);

    // Finish and apply recorded command list
    pExecutionContext->FinishCommandList(pCommandList);

    // Submit frame and involved swap chains (which need to be presented)
    // If there's no swap chains to present, just use EndFrame()
    pExecutionContext->EndFrame1(1, &pSwapChain);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
        RenderFrame();
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void CreateForm(HWND* pHwnd, int width, int height)
{
    // Register the window class.
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = { };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"SGSample";
    RegisterClass(&wc);

    RECT windowRect = { 0, 0, width, height };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    *pHwnd = CreateWindow(
        // Style & class
        wc.lpszClassName,       // Window class
        L"HelloTriangle",       // Window text
        WS_OVERLAPPEDWINDOW,    // Window style

        // Size and position
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,

        // Additions
        NULL, NULL, hInstance, NULL
    );
}
