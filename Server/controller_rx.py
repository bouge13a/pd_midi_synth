import pywinusb.hid as hid
from queue import Queue

class ControllerRX(object):

	def __init__(self, controller_q) :
		self.controller_q = controller_q
		self.controller_connected = False
		self.device = None

	def init_usb (self):

		try :

			# VID and PID customization changes here...
			filter = hid.HidDeviceFilter(vendor_id = 0x1cbe, product_id = 0x000f)
			hid_device = filter.get_devices()
			self.device = hid_device[0]
			self.device.open()
			print(hid_device)

			self.device.set_raw_data_handler(self.read_controller)

			self.controller_connected = True
			return True

		except :

			return False

	def read_controller (self, data):
		
		data

		message = {}
		message["accel x"]  = data[1]
		message["accel y"]  = data[2]
		message["accel z"]  = data[3]
		message["js x"]     = data[4]
		message["js y"]     = data[5]
		message["button a"] = data[6] & 0x01
		message["button b"] = (data[6] & 0x02) >> 1
		message["start"]    = (data[6] & 0x04) >> 2
		message["select"]   = (data[6] & 0x08) >> 3
		message["js OK"]    = (data[6] & 0x10) >> 4 

		if self.controller_q.empty() :
			self.controller_q.put_nowait(message)
		else:
			self.controller_q.get_nowait()
			self.controller_q.put_nowait(message)

		#print("data: {}".format(data))

		return None

if __name__ == "__main__":

	control_q = Queue(maxsize=1)
	controller = ControllerRX(control_q)
	controller.init_usb()

		

