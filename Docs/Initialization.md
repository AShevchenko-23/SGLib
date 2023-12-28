# Initialization SGLib

Just like work with native APIs, work with SGLib begins with initializing some global object (Factory in DXGI or Instance in Vulkan), researching the system adapter list, creating a device object and (for graphical applications) swap chain.

SGLib has a global state object containing a common information about adapter list, debug layers, system environment, runtime helpers etc.

Start your render system with ```SGLibInitialize``` call to initialize SGLib global context:
```cpp 
if (SGLibInitialize(SG_NULL) != SG_OK)
    throw std::exception("SG initializing failed");
```

When the application is finishing, destroy the SGLib singleton:
```cpp 
SGLibDestroy();
```

Optionally, ```SGLibInitialize``` receives a pointer to ```SG_DEBUG_CONFIG``` struct to config debug layers and tools.

```cpp
typedef struct SG_DEBUG_CONFIG
{
    SG_DEBUG_LEVELS DebugLevel;
    SG_DEBUG_TOOLS DebugTools;
} SG_DEBUG_CONFIG;
```

```DebugLevel``` enables native debug layers for tracking API messages and ```DebugTools``` allows your application to use third party tools' features (see [Debug and development tips](DebugAndDev.md)).

## Enumerate adapters

Enumerating adapters occurs in SGLib the same as in DXGI. Use ```SgEnumAdapters``` function to get an adapter from the system list at certain index. It's a regular practice to parse the whole list until the next adapter meets the app's requirements or the function returns SG_ERROR_NOT_FOUND that means the end of the list.

Follow part of code illustrates an approach to display the whole list of available adapters:
```cpp 
int index = 0;

ISGAdapter* pAdapter = SG_NULL;
while (SgEnumAdapters(index, &pAdapter) != SG_ERROR_NOT_FOUND)
{
    std::cout << index << ". ";
    index++;

    if (pAdapter == SG_NULL)
    {
        std::cout << "Unsupported adapter" << std::endl;
        continue;
    }

    // Here you get info about adapter like memory amount, supported features etc.
    SG_ADAPTER_INFO_COMMON commonInfo;
    pAdapter->GetInfo(SG_ADAPTER_INFO_CAT_COMMON, &commonInfo);

    SG_ADAPTER_INFO_MEMORY memoryInfo;
    pAdapter->GetInfo(SG_ADAPTER_INFO_CAT_MEMORY, &memoryInfo);

    uint64_t memMB = memoryInfo.LocalMemory >> 20;
    std::cout << commonInfo.Description << " (" << memMB << "MB)" << std::endl;

    // SgEnumAdapters increments adapter's ref counter, if it won't be used, need to release
    pAdapter->Release();
}
```
> Pay attention code releases every gotten adapter. If the application found fit adapter, it must keep it until device object created.

## Device
An adapter is a base of a device object. Use ```SgCreateDevice``` function to create device object:
```cpp
SG_DEVICE_DESC deviceDesc;
deviceDesc.pDeviceRemovalCallback = SG_NULL;
deviceDesc.pMemoryDebugConfig = SG_NULL;
deviceDesc.pExecutionContextDesc = SG_NULL;

ISGDevice* pDevice = SG_NULL;
if (SgCreateDevice(pAdapter, &deviceDesc, &pDevice) != SG_OK)
    throw std::exception("Failed to create device object");
```

The code demostrated above creates device that doesn't have an execution context that mean it's can not be used to schedule any commands for executing on GPU-side.
Application should pass a pointer to ```SG_EXECUTION_CONTEXT_DESC``` struct in ```SG_DEVICE_DESC::pExecutionContextDesc``` to create device with an execution context.
The follow description requires device object with the execution context containing the **1** frame buffer and the **1** graphics queue (see [Scheduler](Scheduler.md) for more details):
```cpp
SG_EXECUTION_CONTEXT_DESC contextDesc;
contextDesc.FrameBuffers = 1; 
contextDesc.QueueCount = 1;
contextDesc.QueueTypes[0] = SG_QUEUE_TYPE_GRAPHICS;

... 
deviceDesc.pExecutionContextDesc = &contextDesc;
...
```

```SG_DEVICE_DESC``` also contains two members ```pDeviceRemovalCallback``` and ```pMemoryDebugConfig``` that provide additional debugging functionality (see [Debug and development](DebugAndDev.md#debug-additional-functionality) for more info).

## Swap chain
Create swap chain to bring results of render on the screen:
```cpp
SG_SWAP_CHAIN_DESC swapChainDesc;
swapChainDesc.OutputWindow = hWnd;                      // Pass window handle
swapChainDesc.Width = 0;                                // Leave back buffers' size 0
swapChainDesc.Height = 0;                               // to adjust them to client rect
swapChainDesc.Format = SG_FORMAT_R8G8B8A8_UNORM;
swapChainDesc.BufferCount = 2;                          // There should be at least 2 buffer
swapChainDesc.Flags = SG_SWAP_CHAIN_FLAG_ALLOW_TEARING;
swapChainDesc.SwapEffect = SG_SWAP_EFFECT_SEQUENTIAL;

ISGSwapChain* pSwapChain = SG_NULL;

// Set the execution context and an index of the queue slot to perform the swap chain on it
// Look at execution context description, we required a graphics queue in slot #0
if (SgCreateSwapChain(m_pExecutionContext, 0, &swapChainDesc, &pSwapChain) != SG_OK)
    throw std::exception("Failed to create swap chain");
```

> Use ```pSwapChain->SetSyncInterval()``` to set a sync interval for all next presents until another.

### Fullscreen
Swap chains support two scenaries to toggle fullscreen using ```ISGSwapChain::SetFullScreenState```:
* Native - switching to fullscreen mode it switchs a video mode as well. To perform this scenario SG_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH flag must be set upon creation. It is also recommended to use available output display modes to prevent your application from hanging in an unsupported mode.
* Borderless - more preferred and modern scenario where your application stretching to a fullscreen borderless window. This way you save a control over the application even in unexcpected cases.
> You don't need to change window size manually, it's DXGI responcibility.

### Resize chain buffers
Use ```ISGSwapChain::Resize``` to resize you swap chain buffers (e.g. on window resize event).

> Pay attention, ```ISGSwapChain::SetFullScreenState``` and ```ISGSwapChain::Resize``` APIs must not be called withing active frame (between ```ISGExecutionContext::BeginFrame``` and  ```ISGExecutionContext::End``` calls).

