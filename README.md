# SGLib 0.0.8.5 (Alpha)

SGLib is a graphics middleware library for C/C++. Based on D3D12 it provides APIs close to D3D11 in a couple with feautures and capabilities of modern hardware, like ray tracing, async compute, async scheduling, meshlet shading, variable rate shading etc.
It uses best practices of graphics APIs to hide low-level management and make developers to focus on features and algorithms. See [Getting started](Docs/Tutorial.md) sections for more details.

## Key features
* Built-In frames resolving and works sync;
* Built-In resource states tracking;
* Support third party debug tools, like Microsoft PIX and NVidia NSight Aftermath;
* Asynchronous scheduling (thread-safe work with command lists, resource usage etc);
* Asynchronous execution (AsyncCompute);
* Ray tracing;
* Variable rate shading;
* Meshlet rendering;

## Release notes
### 0.0.8.5
* Fixed API errors on NVidia Ada adapters related to getting D3D12_GPU_DESCRIPTOR_HANDLE;
* Fixed crash when attempt to override input layout for PSO created without an input layout
* Perf-improvements

### 0.0.8.1
* Fixed sync interval when SG_SWAP_CHAIN_FLAG_ALLOW_TEARING flag is set;
* Minor optimizations;

### 0.0.8.0
* Overriding PSO's input layout - user is able to create pipeline state objects without any input layout and set it right before draw calls;
* Perfomance and stability improvements
* Disabled watermark

### 0.0.7.0
* Rework of ["ascending" and "descending" transfers](Docs/AsyncCompute.md#restrictions) (they're still complex, but much faster than before)
* Perf-improvements
* Increased stability
* [HelloTriangle tutorial](Docs/HelloTriangleTutorial.md)

### 0.0.6.0
* The first public version

## WIP/TODO
- HDR output
- Tiled resources
- Pipeline state caches

## Platforms
SGLib supports D3D12 on Windows only atm.
In the future, if there is demand for it, there are also plans to support game consoles, Vulkan for Android and Linux.

## Package
SGLib package contains C/C++ headers, binaries, documentation and samples.
Every back-end has two builds implementing developer and release version:
* SGLib\_<*API_NAME*>\_dev.lib
* SGLib\_<*API_NAME*>\_dev.dll
* SGLib\_<*API_NAME*>.lib
* SGLib\_<*API_NAME*>.dll

Binary pairs that have **_dev** ending in the name are optimized for developing and debugging workflows, perform more detailed validation, support third-party debug tools such as Microsoft PIX and NVidia NSight Aftermath.

## Contacts
For questions, feedback and suggestions, please email ashevchenko.sglib@gmail.com

## License
SGLib is licensed under the MIT License, see [LICENSE.txt](LICENSE.txt) for more details.
