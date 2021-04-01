import tornado.ioloop
import tornado.web
import tornado.websocket
import tornado.options
import json
import webbrowser
import signal
import logging
from queue import Queue
from controller_rx import ControllerRX

class DatatypeEncoder(json.JSONEncoder):
    '''Special json encoder for numpy types'''

    def default(self, obj):
        if isinstance(obj, (np.int_, np.intc, np.intp, np.int8,
            np.int16, np.int32, np.int64, np.uint8,
            np.uint16, np.uint32, np.uint64)):
            return int(obj)

        elif isinstance(obj, (np.float_, np.float16, np.float32, np.float64)):
            return float(obj)

        elif isinstance(obj,(np.ndarray,)):
            return obj.tolist()

        elif isinstance(obj,(datetime.datetime,)):
            return obj.strftime("%Y%m%d_%H%M%S")

        return json.JSONEncoder.default(self, obj)


class GameControllerWS(tornado.websocket.WebSocketHandler):

	def check_origin(self, origin):
        # Authenticates connections made from places other then "localhost"
		return True

	def open(self):
		self.ws_stream = tornado.ioloop.PeriodicCallback(self.stream_controls, 20)
		self.ws_stream.start()
		print("Websocket open")

	def on_close(self):
    # Stop sending data if the websocket closes

		self.ws_stream.stop()
		print("WebSocket closed")

	def stream_controls(self):
		global global_controller
		global global_controller_q

		if not global_controller_q.empty():
			message = global_controller_q.get_nowait()
			self.write_message(json.dumps(message, cls=DatatypeEncoder)) 
		else :
			global_controller.init_usb()
    
class MainHandler(tornado.web.RequestHandler):
    def get(self):
    	self.render("static\\index.html")

class GameApplication(tornado.web.Application):
	is_closing = False

	def signal_handler(self, signum, frame):
		logging.info("exiting...")
		self.is_closing = True

	def try_exit(self):
		if self.is_closing:
			tornado.ioloop.IOLoop.instance().stop()
			logging.info("exit success")

application = GameApplication([
	(r"/", MainHandler),
	(r"/game_controller_ws", GameControllerWS),
])

if __name__ == "__main__":

	global global_controller_q 
	global global_controller

	global_controller_q = Queue(maxsize=1)

	global_controller = ControllerRX(global_controller_q)
	global_controller.init_usb()

	webbrowser.get("C:/Program Files (x86)/Google/Chrome/Application/chrome.exe %s").open("http://localhost:8888")

	print("Press ctrl + c to close")

	tornado.options.parse_command_line()
	signal.signal(signal.SIGINT, application.signal_handler)
	application.listen(8888)
	tornado.ioloop.PeriodicCallback(application.try_exit, 100).start()
	tornado.ioloop.IOLoop.current().start()