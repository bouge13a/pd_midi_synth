import usb.core

#import usb.backend.libusb1
 
#backend = usb.backend.libusb1.get_backend(find_library=lambda x: "C:\\Users\\steph\\Documents\\Game_Controller_C\\scripts.libusb-1.0.dll")
#dev = usb.core.find(backend=backend)

dev = usb.core.find(idVendor=0x1cbe, idProduct=0x000F)
ep = dev[0].interfaces()[0].endpoints()[0]
i = dev[0].interfaces()[0].bInterfaceNumber
dev.reset()

if dev.is_kernel_driver_active(i):
	dev.detach_kernel_driver(i)

dev.set_configuration()
eaddr=ep.bEndpointAddress

r=dev.read(eaddr, 1024)
print(len(r))