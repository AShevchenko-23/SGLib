# Debugging and development tips

It is good practice to develop an application based on __\*dev__ builds. These builds may be slower than release builds in certain cases, but can provide more debugging information and help avoid early bugs.

## Debug layers
Use ```SG_DEBUG_CONFIG::DebugLevel``` to enable native debug layers.
```cpp 
SG_DEBUG_CONFIG debugConfig{};
debugConfig.DebugLevel = SG_DEBUG_L1;

if (SGLibInitialize(&debugConfig) != SG_OK)
    throw std::exception("SG initializing failed");
```

## Debug additional functionality

```SG_DEVICE_DESC``` allows an application to get some additional features to debug its runtime:
```cpp
typedef struct SG_DEVICE_DESC
{
    PFN_SG_DEVICE_REMOVAL_CALLBACK pDeviceRemovalCallback;
    SG_MEMORY_DEBUG_CONFIG* pMemoryDebugConfig;
    SG_EXECUTION_CONTEXT_DESC* pExecutionContextDesc;
} SG_DEVICE_DESC;
```

```pDeviceRemovalCallback``` optionally may pointer to callback function that will handle errors when device has been lost:
```cpp
void DeviceRemovalCallback(U32 messageSize, char const* pErrorMessage)
{
    std::cout << pErrorMessage << std::endl;
}

...

SG_DEVICE_DESC deviceDesc{};
deviceDesc.pDeviceRemovalCallback = DeviceRemovalCallback;
```

This is an extremely rare case that may accompany errors ```SG_ERROR_API``` and ```SG_ERROR_INTERNAL``` (see [Result codes](ResultCodes.md)). It rather is to help me to improve SGLib.

Another feature is an artificial memory limit that you can set to test the application's behavior on systems with low memory:
```cpp
SG_MEMORY_DEBUG_CONFIG memDebug{};
memDebug.LocalBudgetLimit = 4 << 30; // Set memory limit to 4 GB, 0 means no limits
memDebug.AllowOutOfBounds = false;   // Allows application to out of VRAM bounds. If LocalBudgetLimit is not 0, this member is ignored

...

SG_DEVICE_DESC deviceDesc{};
deviceDesc.pMemoryDebugConfig = &memDebug;
```

> ```LocalBudgetLimit``` is only supported by __\*dev__ SGLib builds.\
> ```AllowOutOfBounds``` is supported by any builds, but is not recommended for release as may works incorrect on some hardware.


## Debug tools

SGLib supports two third party tools atm: Microsoft PIX and NVidia NSight Aftermath.
Use ```SG_DEBUG_CONFIG::DebugTools``` flags to declare used third party tools.

### Microsoft PIX

SGLib __\*dev__ builds support PIX event markers, use ```SG_DEBUG_TOOL_MS_PIX``` to involve this feature.
> Pay attention the execution folder must contain **WinPixEventRuntime.dll** file to enable this feature.

```cpp 
SG_DEBUG_CONFIG debugConfig{};
debugConfig.DebugLevel = SG_DEBUG_L1;
debugConfig.DebugTools |= SG_DEBUG_TOOL_MS_PIX;

if (SGLibInitialize(&debugConfig) != SG_OK)
    throw std::exception("SG initializing failed");

...

// Now you can use BeginEvent and EndEvent to set markers:
pCommandList->BeginEvent(markerColor, "RenderPass");
pCommandList->DrawInstanced(...);
pCommandList->EndEvent();

```

### NVidia NSight Aftermath
Use ```SG_DEBUG_TOOL_NV_NSIGHT_AFTERMATH``` flag in __\*dev__ builds to enable crash breadcrumbs on device lost cases.
> Pay attention the execution folder must contain **GFSDK_Aftermath_Lib.x64.dll** file to enable this feature.\
> NVidia NSight doesn't support native debug layers, ```SG_DEBUG_CONFIG::DebugLevel``` must be ```SG_DEBUG_DISABLED```.

