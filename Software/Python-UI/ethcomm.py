import socket
import threading
import struct

############################################################
#
class EthComm:
    ############################################################
    #
    def __init__(self):
        self.socket = None
        self.connected = False
        self.read_loop = None

    ############################################################
    #
    def laser_on(self, pwm, init_pwm=-1):
        if not self.connected:
            return
        print "Laser on with PWM %d, init PWM %d" % (pwm, init_pwm)
        if init_pwm == -1:
            self.send_cmd("=" + str(pwm))
        else:
            self.send_cmd("=" + str(pwm) + ":" + str(init_pwm))

    ############################################################
    #
    def laser_off(self):
        if not self.connected:
            return
        print "Laser off"
        self.send_cmd("0")

    ############################################################
    #
    def laser_auto(self, pwm, init_pwm=-1):
        if not self.connected:
            return
        print "Laser on _auto_ with PWM %d, init PWM %d" % (pwm, init_pwm)

        if init_pwm == -1:
            self.send_cmd("A" + str(pwm))
        else:
            self.send_cmd("A" + str(pwm) + ":" + str(init_pwm))

    ############################################################
    #
    def reset_coordinates(self):
        if not self.connected:
            return
        print "Resetting board coordinates"
        self.send_cmd("z\n")

    ############################################################
    #
    def send_cmd(self, cmd):
        cmd = cmd + "\n"
        self.socket.sendall(cmd)

    ############################################################
    #
    def connect(self, host_port, xyz_callback):
        try:
            (host, port) = host_port.split(':')
            print "Connecting to %s:%d ..." % (host, int(port))
            self.socket = socket.create_connection((host, int(port)))
            self.laser_off()
            self.connected = True
            print "Connected!"
            self.read_loop = ReadLoop(self.socket, xyz_callback)
            threading.Thread(target=self.read_loop).start()
            return True
        except IOError:
            self.connected = False
            print "Couldn't connect"
            return False


    ############################################################
    #
    def disconnect(self):
        if not self.connected:
            return

        print "Disconnecting..."
        self.read_loop.cancel()
        self.socket.close()
        self.connected = False
        print "Disconnected!"


############################################################
#
class ReadLoop:
    def __init__(self, socket, callback):
        self.socket = socket
        self.callback = callback
        self.run = True

    def __call__(self, *args, **kwargs):
        print "Read loop thread started"

        buff = bytes()
        try:
            while self.run:
                data = self.socket.recv(32)
                # print "Received: ", data
                buff = buff + data
                cr_location = buff.rfind('\n')
                if cr_location >= 12:
                    # print "cr_location %d" % (cr_location)
                    x = struct.unpack('<i', buff[cr_location - 12: cr_location - 8])[0]
                    y = struct.unpack('<i', buff[cr_location - 8: cr_location - 4])[0]
                    z = struct.unpack('<i', buff[cr_location - 4: cr_location])[0]
                    if self.callback is not None:
                        self.callback(x, y, z)
                    buff = bytes()
        except IOError:
            # finita la comedia
            print "Error"
            self.cancel()

    def cancel(self):
        self.run = False
        self.socket = None
