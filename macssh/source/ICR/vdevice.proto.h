//vdevice.proto.h

int InitVDevice(VDevicePtr vdev, PaletteHandle initalPalette);
int SetVDevice(VDevicePtr vdev);
void UnsetVDevice(void);
void TrashVDevice(VDevicePtr vdev);
void ColorVDevice(VDevicePtr vdev, PaletteHandle pal);
