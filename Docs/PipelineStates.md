# Pipeline states
Pipeline states represent a set of shaders, input layout and root signature.
There is 4 types of pipeline states: graphics, mesh, compute and ray tracing.

## Graphics pipeline state
Describes regular graphics pipeline state containing vertex, pixel, geometry, hull and domain shader stages:
```cpp
typedef struct SG_GRAPHICS_PIPELINE_STATE_DESC
{
    SG_ROOT_SIGNATURE_DESC RootSignature;
    SG_SHADER_BYTECODE PS;
    SG_SHADER_BYTECODE VS;
    SG_SHADER_BYTECODE DS;
    SG_SHADER_BYTECODE HS;
    SG_SHADER_BYTECODE GS;
    ISGInputLayout*  pInputLayout;
} SG_GRAPHICS_PIPELINE_STATE_DESC;
```
* Members with type **SG_SHADER_BYTECODE** store shader bytecode in the format DXBC.

## Mesh pipeline state
Describes raster pipeline state based on mesh shaders:
```cpp
typedef struct SG_MESH_PIPELINE_STATE_DESC
{
    SG_ROOT_SIGNATURE_DESC RootSignature;
    SG_SHADER_BYTECODE PS;
    SG_SHADER_BYTECODE MS;
    SG_SHADER_BYTECODE AS;
} SG_MESH_PIPELINE_STATE_DESC;
```
* Members with type **SG_SHADER_BYTECODE** store shader bytecode in the format DXBC.

Before creating mesh pipeline states, check adapter on supporting the feature:
```cpp
SG_ADAPTER_INFO_EXECUTION execInfo;
pAdapter->GetInfo(SG_ADAPTER_INFO_CAT_EXECUTION, &execInfo);

if (execInfo.MeshShaderTier == SG_MESH_SHADER_TIER_NOT_SUPPORTED)
    throw std::exception("Adapter doesn't support mesh shading");
```

## Compute pipeline state
Describes compute pipeline state:
```cpp
typedef struct SG_COMPUTE_PIPELINE_STATE_DESC
{
    SG_ROOT_SIGNATURE_DESC RootSignature;
    SG_SHADER_BYTECODE CS;
} SG_COMPUTE_PIPELINE_STATE_DESC;
```
* Members with type **SG_SHADER_BYTECODE** store shader bytecode in the format DXBC.

### Ray tracing pipeline state
WIP
 > Ray tracing pipeline states are not described in this section so far (refer to [Ray tracing sample](../Samples/Raytracing/Readme.md) for more details).
