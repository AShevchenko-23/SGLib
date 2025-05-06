# SGLib 0.0.9.0 (Alpha)

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
### Current 0.0.9.1 (5/6/2025)
 * **SgGetFormatSizeBits** function;
 * Fixed border color setting for static samplers;
 * Fixed 3D textures creation;

See also [version history](Docs/Versions.md).

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
