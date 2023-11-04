# SGLib (Alpha version)

SGLib is a graphics middleware library for C/C++.

SGLib aims to save familiar concepts: many APIs fully or partially correspond to their analogues in native libraries such as D3D12 and Vulkan.
On the other hand it uses best practices of graphics APIs to hide low-level management and make developers to focus on features and algorithms. See [Tutorial](Docs/Tutorial.md) sections for more details.

## Key features
* Asynchronous scheduling (thread-safe work with command lists, resource usage etc)
* Asynchronous execution
* Ray tracing
* Variable rate shading
* Meshlet rendering

### WIP/TODO
- HDR output
- Tiled resources
- Pipeline state caches

## Platforms
At the moment, SGLib supports D3D12 on Windows only.
In the future, if there is demand for it, there are also plans to support game consoles, Vulkan for Android and Linux.

## Package
SGLib package contains C/C++ headers, binaries, documentation and samples.
Every back-end has two builds implementing developer and release version:
* SGLib\_<*API_NAME*>\_dev.lib
* SGLib\_<*API_NAME*>\_dev.dll
* SGLib\_<*API_NAME*>.lib
* SGLib\_<*API_NAME*>.dll

Binary pairs that have **_dev** ending in the name are optimized for developing and debugging workflows, perform more detailed validation, support third-party debug tools such as Microsoft PIX and NVidia NSight Aftermath.

## License
SGLib is licensed under the MIT License, see [LICENSE.txt](LICENSE.txt) for more details.
