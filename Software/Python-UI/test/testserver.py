import SocketServer
from SocketServer import StreamRequestHandler
import struct
import time


class Handler(StreamRequestHandler):
    def __init__(self, request, client_address, server):
        print "Initializing"
        self.x = 0
        self.y = 0
        self.z = 0
        StreamRequestHandler.__init__(self, request, client_address, server)

    def handle(self):
        print "Handling"
        while True:
            self.wfile.write(struct.pack("<iii", self.x, self.y, self.z))
            self.wfile.write("\n")
            self.x += 100
            self.y += 100
            time.sleep(0.001)


server = SocketServer.TCPServer(('0.0.0.0', 4004), Handler, bind_and_activate=True)
while True:
    try:
        server.handle_request()
    except:
        print "Ooops"
