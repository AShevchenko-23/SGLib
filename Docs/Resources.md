# Resources
## Buffers
SGLib divides buffers by 3 types:
* **SG_BUFFER_TYPE_COMMON** - buffer placed in the video memory, allows fast reading and writing on GPU side, supports any bind flags (**SG_BUFFER_BIND_FLAGS**);
* **SG_BUFFER_TYPE_UPLOAD** - buffer placed in the shared memory, allows reading on GPU side and writing on CPU, doesn't support unordered access binding (**SG_BUFFER_BIND_FLAG_UNORDERED_ACCESS**)
* **SG_BUFFER_TYPE_READBACK** - buffer placed in the shared memory, allows writing on GPU side and reading on CPU, doesn't support any bindings;

Mostly, buffers defined as **SG_BUFFER_TYPE_UPLOAD** are used as constant buffers or for uploading data to high-performance common buffers.
Buffers defined as **SG_BUFFER_TYPE_READBACK** are used for transfering data from GPU to CPU.

Follow code creates a buffer that can be bound for unordered acccess in shader:
```cpp
SG_BUFFER_DESC desc;
desc.Type = SG_BUFFER_TYPE_COMMON;
desc.BindFlags = SG_BUFFER_BIND_FLAG_UNORDERED_ACCESS;
desc.Size = 256;

ISGBuffer* pBuffer = SG_NULL;
if (pDevice->CreateBuffer(&desc, &pBuffer) != SG_OK)
    throw std::exception("Failed to create buffer");
```

## Textures
SGLib divides textures by 4 types:
* **SG_TEXTURE_TYPE_COMMON** - textures placed in the video memory, allows fast reading and writing on GPU side, supports any bind flags (**SG_TEXTURE_BIND_FLAGS**);
* **SG_TEXTURE_TYPE_DEPTH_STENCIL** - textures placed in the video memory and optimized for storing depth data, supports binding as shader resource (**SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE** and **SG_TEXTURE_BIND_FLAG_TEXTURE_CUBE**)
* **SG_TEXTURE_TYPE_UPLOAD** - textures placed in the shared memory, allows reading on GPU side and writing on CPU, doesn't support any bindings;
* **SG_TEXTURE_TYPE_READBACK** - textures placed in the shared memory, allows writing on GPU side and reading on CPU, doesn't support any bindings;

Textures that defined as **SG_TEXTURE_TYPE_UPLOAD** or **SG_TEXTURE_TYPE_READBACK** used only for transfering data between CPU and GPU.

> Textures defined as **SG_TEXTURE_TYPE_UPLOAD** or **SG_TEXTURE_TYPE_READBACK** although contain footprints that corespond to the texture description, but are buffers. Due this, such textures can't be represented for shader access, **BindFlags** must be equal **SG_TEXTURE_BIND_FLAG_NONE**.

Follow code creates a texture that can be bound as shader resource:
```cpp
SG_TEXTURE_DESC desc;
desc.Type = SG_TEXTURE_TYPE_COMMON;
desc.BindFlags = SG_TEXTURE_BIND_FLAG_SHADER_RESOURCE;

desc.Dimension = SG_TEXTURE_DIMENSION_2D;
desc.Format = SG_FORMAT_R8G8B8A8_UNORM;
desc.Width = 128;
desc.Height = 128;
desc.DepthOrArraySize = 1;
desc.MipLevels = 1;

// If you don't need MSAA, set {SampleCount = 0 or 1; SampleQuality = 0 }
desc.SampleCount = 0;
desc.SampleQuality = 0;

// If it is not a render target or depth stencil, default value doesn't matter
desc.DefaultValue.Format = SG_FORMAT_UNKNOWN;
desc.DefaultValue.Color = {};

ISGTexture* pTexture = SG_NULL;
if (pDevice->CreateTexture(&desc, &pTexture) != SG_OK)
    throw std::exception("Failed to create texture");
```
> **SG_TEXTURE_DESC::DefaultValue** specifies a default clearing values for render targets or depth stencil buffers to perform optimized clearing by **ISGCommandList::ClearRenderTargetDefault** and **ISGCommandList::ClearDepthStencilDefault** 

### Subresources
SGLib divides resources into subresources. Buffers always consist of only one subresource and **ISGBuffer** has a direct API to access buffer data:
```cpp
void* pData = nullptr;

// Mapping works only for upload or readback resources
if (pUploadBuffer->Map(&pData) != SG_OK)
    throw std::exception("Failed to map buffer");

// Here you have an access to buffer data using pData

pUploadBuffer->Unmap();
```

Textures may be divided into several subresources (by MIP levels, array slices or plane slices for certain formats). See [DirectX 12 subreosurces](https://learn.microsoft.com/en-us/windows/win32/direct3d12/subresources) for more details.\
Thus to perform an access to textures data use **ISGTexture::GetSubresource** at first:
```cpp
ISGSubresource* pSubresource = nullptr;

// Request a subresource at (mip, array slice, plane slice)
if (pTexture->GetSubresource(0, 0, 0, &pSubresource) == SG_OK)
{
    SG_MAPPED_SUBRESOURCE mappedSubresource;

    // Mapping works only for upload or readback resources
    if (pSubresource->Map(&mappedSubresource) == SG_OK)
    {
        // Here you have an access to texture data using mappedSubresource.pData

        // After all don't forget unmap subresource
        pSubresource->Unmap();
    }

    // And release it
    pSubresource->Release();
}
```

It also works if you need to copy texture by subresources:
```cpp
ISGSubresource* pDstSubres = nullptr;
ISGSubresource* pSrcSubres = nullptr;

// Let's copy slice #1 to slice #0
if (pTexture->GetSubresource(0, 0, 0, &pDstSubres) != SG_OK)
    throw std::exception("Failed to get dest subresource");

if (pTexture->GetSubresource(0, 1, 0, &pSrcSubres) != SG_OK)
    throw std::exception("Failed to get src subresource");

pCommandList->CopySubresource(pDstSubres, pSrcSubres);

pDstSubres->Release();
pSrcSubres->Release();
```
