# Result codes

Result codes are presented by ```SG_RESULT``` costants and separated by several categories depending on the context.

## Common category
| Constant | Description |
|----------|-------------|
|```SG_OK``` | Call executed successfully |
|```SG_MESSAGE_ALREADY_DONE``` | Means that this call was already performed, repeated call is ignored
|```SG_ERROR_API``` | Unhandled API error, use debug layers to get more info
|```SG_ERROR_INTERNAL``` | Internal unhandled error, refer to the author to get some help
|```SG_ERROR_INVALID_ARG``` | One or more arguments is not valid to perform the call
|```SG_ERROR_UNSUPPORTED_FEATURE``` | Feature is not supported by the hardware or version of SGLib
|```SG_ERROR_OUT_OF_MEMORY``` | Attempt to go out of memory during allocation
|```SG_ERROR_NOT_FOUND``` | Attempt to get an adapter (by ```SgEnumAdapters```) by index that out of bounds of the adapter array
|```SG_ERROR_UNRELEASED_RESOURCES``` | Attempt to resize swap chain buffers while at least one of them is used
|```SG_ERROR_OUT_OF_BOUNDS``` | Reserved error code |
|```SG_ERROR_OVERLAPPING``` | Reserved error code |

## Device category
| Constant | Description |
|----------|-------------|
|```SG_ERROR_NO_EXEC_CTX``` | Attempt to get execution context of device object that was created without it
|```SG_ERROR_INVALID_QUERY_TYPE``` | Attempt to create query or predicate of invalid/unsupported type

## Execution context category
| Constant | Description |
|----------|-------------|
|```SG_ERROR_NO_ACTIVE_FRAME``` | Attempt to schedule a command list out an active frame (use ```ISGExecutionContext::BeginFrame``` and ```ISGExecutionContext::EndFrame``` to declare frame bounds)
|```SG_ERROR_INVALID_TIME_INDEX``` | Attempt to schedule a command list on invalid time index (Time index must be in the range **[1; 65520]**. Index **0** and indexes after **65520** are reserved for internal use)
|```SG_ERROR_CMD_LIST_ALREADY_FINISHED``` | Attempt to finish of an already finished command list
|```SG_ERROR_LOCKED_EXEC_CTX``` | Attempt to wait for finish of execution non-completed frame (occurs when application calls ```WaitForIdle``` between the ```BeginFrame``` and ```EndFrame``` calls)
|```SG_ERROR_QUERY_HAS_NOT_BEEN_EXECUTED``` | Attempt to get results of non-executed query
|```SG_ERROR_COPY_QUEUE_TIMESTAMPS_NOT_SUPPORTED``` | Attempt to query timestamp on copy queue when this feature is not supported (use ```SG_ADAPTER_INFO_EXECUTION::CopyQueueTimeStamps``` to check this feature)
|```SG_ERROR_IMPROPER_QUERY_OBJECT_TYPE``` | Attempt to get results of predicate. Predicates only supports GPU access.

## Resource common category
| Constant | Description |
|----------|-------------|
|```SG_ERROR_INVALID_RESOURCE_TYPE``` | Attempt to create a view whose type does not match the resource type
|```SG_ERROR_INCOMPATIBLE_ACCESS_TYPE``` | Attempt to map a resource whose type is neither upload nor readback
|```SG_ERROR_OUT_OF_RESOURCE_BOUNDS``` | Attempt to create view that outs of resource bounds
|```SG_ERROR_INVALID_FORMAT``` | Attempt to create resource, view or swap chain buffers using unsupported (or ```SG_FORMAT_UNKNOWN```) format
|```SG_ERROR_BUFFER_INVALID_SIZE``` | Attempt to create buffer with zero size
|```SG_ERROR_BUFFER_INCOMPATIBLE_BIND_FLAG``` | Attempt to create buffer with not supported bind flags
|```SG_ERROR_BUFFER_INVALID_STRIDE``` | Attempt to create a structured buffer view with zero stride
|```SG_ERROR_TEXTURE_INVALID_DIMENSION``` | Attempt to create texture with invalid dimension
|```SG_ERROR_TEXTURE_INVALID_SIZE``` | Attempt to create texture with zero size (multiplication **Width * Height * DepthOrArraySize * MipLevels** must not be equal **0**)
|```SG_ERROR_TEXTURE_INCOMPATIBLE_BIND_FLAG``` | Attempt to create texture with not supported bind flags

## Resource view category
| Constant | Description |
|----------|-------------|
|```SG_ERROR_UNKNOWN_VIEW_TYPE``` | Attempt to create resource view with unsupported/undefined type
|```SG_ERROR_INCONSISTENT_VIEW``` | Attempt to create a view for resource that doesn't contain an appropriate binding flag
|```SG_ERROR_RES_VIEW_TYPES_MISMATCH``` | Attempt to create a texture view for buffer resource or vice versa

## Pipeline states category
| Constant | Description |
|----------|-------------|
|```SG_ERROR_INVALID_SHADER``` | Attempt to create pipeline state object without required shader (graphics PSO must contain vertex shader, meshlet PSO must contain mesh shader, compute PSO must contain compute shader)
|```SG_ERROR_INVALID_ROOT_SIGNATURE_TYPE``` | Attempt to use unsupported/undefined root signature type
