//memory.proto.h

#ifdef __cplusplus
extern "C" {
#endif

void initMemoryBuffer(Size cushion, Size reserve);
Boolean RecoverReserveMemory (void);
Boolean MemoryAvailable (Size len);
static pascal long MyGrowZone (Size cbNeeded);
Handle myNewHandle(Size len);
Handle myNewHandleCritical(Size len);
Ptr myNewPtr(Size len);
Ptr myNewPtrCritical(Size len);
OSErr mySetHandleSize (Handle handle, Size len);
OSErr MySetHandleSizeCritical (Handle handle, Size len);

#ifdef __cplusplus
}
#endif
