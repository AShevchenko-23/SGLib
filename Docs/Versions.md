# Version History
### 0.0.9.1 (5/6/2025)
 * **SgGetFormatSizeBits** function;
 * Fixed border color setting for static samplers;
 * Fixed 3D textures creation;

### 0.0.9.0
 * **ClearUnorderedAccessViewUint** and **ClearUnorderedAccessViewFloat** features;
 * Devices created with the Debug Layer [reports about living resources](DebugAndDev.md#live-resource-reports) on releasing in Dev build;
 * Optimized creation of root signatures and pipeline state objects;
 * Stability improvements;

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
* Rework of ["ascending" and "descending" transfers](AsyncCompute.md#restrictions) (they're still complex, but much faster than before)
* Perf-improvements
* Increased stability
* [HelloTriangle tutorial](HelloTriangleTutorial.md)

### 0.0.6.0
* The first public version