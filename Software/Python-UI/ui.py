from Tkinter import *
import Tkinter, Tkconstants, tkFileDialog, tkMessageBox
from ethcomm import EthComm
from mqttcomm import MqttComm
from imagehandler import ImageHandler
from os import path


class UI:
    def __init__(self):
        self.eth_comm = EthComm()
        self.img_handler = ImageHandler()
        self.mqtt_comm = MqttComm(self)

        self.laser_mode = 0
        self.laser_state = 0

        self.last_used_dir = "/"

        self.x = 0.0
        self.y = 0.0


        ##################################################################
        # create UI
        self.root = Tk()
        row = 0
        irow = 0

        f = LabelFrame(self.root, text="Control board")
        f.grid(row=row, column=0, columnspan=1, sticky=W+E, padx=5, pady=5, ipady=5)

        label_address = Label(f, text="Host:port")
        label_address.grid(row=irow, column=0, sticky=E)

        self.entry_board_address = Entry(f)
        self.entry_board_address.grid(row=irow, column=1, columnspan=2, sticky=W+E, padx=5)
        self.entry_board_address.insert(0, "10.9.9.101:4004")

        self.button_connect = Button(f, text="Connect", command=lambda: self.on_connect_clicked())
        self.button_connect.grid(row=irow, column=3, pady=10)

        irow += 1
        row += 1

        label_address = Label(f, text="Laser on/off")
        label_address.grid(row=irow, column=0, sticky=E)

        fIn = Tkinter.Frame(f)
        fIn.grid(row=irow, column=1, columnspan=4, sticky=W+E)
		
        button_ctrl_off = Button(fIn, text="Off", width=5, command=lambda: self.on_laser_level_clicked(0))
        button_ctrl_off.grid(row=0, column=0, padx=6)

        button_ctrl_on = Button(fIn, text="On", width=5, command=lambda: self.on_laser_level_clicked(1))
        button_ctrl_on.grid(row=0, column=1, padx=6)

        button_ctrl_auto = Button(fIn, text="Auto", width=5, command=lambda: self.on_laser_level_clicked(2))
        button_ctrl_auto.grid(row=0, column=2, padx=6)

        button_ctrl_auto = Button(fIn, text="Pilot", width=5, command=lambda: self.on_laser_level_clicked(3))
        button_ctrl_auto.grid(row=0, column=3, padx=6)

        irow += 1

        label_slider_pwm = Label(f, text="PWM value")
        label_slider_pwm.grid(row=irow, column=0, sticky=S+E)

        self.slider_pwm = Scale(f, from_=0, to=255, orient=HORIZONTAL, variable="pwm")
        self.slider_pwm.grid(row=irow, column=1, columnspan=3, sticky=W+E)

        self.pwm_value = Tkinter.IntVar(f, 0, "pwm")
        self.pwm_value.trace("w", lambda *args: self.on_pwm_slider_changed())

        irow += 1
        label_slider_init_pwm = Label(f, text="Initial PWM value")
        label_slider_init_pwm.grid(row=irow, column=0, sticky=S+E)

        self.slider_init_pwm = Scale(f, from_=0, to=255, orient=HORIZONTAL, variable="init_pwm")
        self.slider_init_pwm.grid(row=irow, column=1, columnspan=3, sticky=W+E)

        self.init_pwm_value = Tkinter.IntVar(f, 0, "init_pwm")
        self.init_pwm_value.trace("w", lambda *args: self.on_pwm_slider_changed())

        self.check_use_init_pwm = Checkbutton(f, text="Use?", onvalue="1", offvalue="0", variable="use_init_pwm")
        self.check_use_init_pwm.grid(row=irow, column=4, sticky=S+E)

        self.use_init_pwm_value = Tkinter.IntVar(f, 0, "use_init_pwm")
        self.use_init_pwm_value.trace("w", lambda *args: self.on_pwm_slider_changed())

        row += 1
        irow = 0

        f = LabelFrame(self.root, text="MQTT server")
        f.grid(row=row, column=0, columnspan=1, sticky=W + E, padx=5, pady=5, ipady=5)

        label_mqtt_address = Label(f, text="Host:port")
        label_mqtt_address.grid(row=irow, column=0, sticky=E)

        self.entry_mqtt_address = Entry(f)
        self.entry_mqtt_address.grid(row=irow, column=1, columnspan=2, sticky=W+E, padx=5)
        self.entry_mqtt_address.insert(0, "localhost:1883")

        self.button_mqtt_connect = Button(f, text="Connect", command=lambda: self.on_mqtt_connect_clicked())
        self.button_mqtt_connect.grid(row=irow, column=3, pady=10)

        row += 1
        irow = 0

        f = LabelFrame(self.root, text="Resolution (steps per mm)")
        f.grid(row=row, column=0, columnspan=1, sticky=W+E, padx=5, pady=5, ipady=5)

        label_x_res = Label(f, text="X")
        label_x_res.grid(row=irow, column=0, sticky=E)

        entry_x_res = Entry(f, textvariable='x_res')
        entry_x_res.grid(row=irow, column=1, padx=5)
        self.x_res = DoubleVar(f, 400.0, 'x_res')

        label_y_res = Label(f, text="Y")
        label_y_res.grid(row=irow, column=2, sticky=E)

        entry_y_res = Entry(f, textvariable='y_res')
        entry_y_res.grid(row=irow, column=3, padx=5)
        self.y_res = DoubleVar(f, 400.0, 'y_res')

        row += 1
        irow = 0

        f = LabelFrame(self.root, text="Initial coordinates")
        f.grid(row=row, column=0, columnspan=1, sticky=W+E, padx=5, pady=5, ipady=5)

        label_x_init = Label(f, text="X")
        label_x_init.grid(row=irow, column=0, sticky=E)

        entry_x_init = Entry(f, textvariable='x_init')
        entry_x_init.grid(row=irow, column=1, padx=5)
        self.x_init = DoubleVar(f, 0.0, 'x_init')

        label_y_init = Label(f, text="Y")
        label_y_init.grid(row=irow, column=2, sticky=E)

        entry_y_init = Entry(f, textvariable='y_init')
        entry_y_init.grid(row=irow, column=3, padx=5)
        self.y_init = DoubleVar(f, 0.0, 'y_init')

        button_set_xy = Button(
            f,
            text="Reset",
            command=lambda: self.reset_xy()
        )
        button_set_xy.grid(row=irow, column=4)

        row += 1
        irow = 0

        f = LabelFrame(self.root, text="Current coordinates")
        f.grid(row=row, column=0, columnspan=1, sticky=W+E, padx=5, pady=5, ipady=5)

        check_follow_xyz = Checkbutton(f, text="Follow XY?", onvalue=True, offvalue=False, variable="follow_xyz")
        check_follow_xyz.grid(row=irow, column=0, columnspan=3, sticky=S+W, padx=20)
        self.follow_xyz = Tkinter.IntVar(f, False, "follow_xyz")
        self.follow_xyz.trace("w", lambda *args: self.on_follow_xyz_changed())

        label_x_curr = Label(f, text="X")
        label_x_curr.grid(row=irow, column=3, sticky=E)

        label_x_curr_val = Label(f, text="0", textvariable='curr_x')
        label_x_curr_val.grid(row=irow, column=4, sticky=W)

        label_y_curr = Label(f, text="Y")
        label_y_curr.grid(row=irow, column=5, sticky=E)

        label_y_curr_val = Label(f, text="0", textvariable='curr_y')
        label_y_curr_val.grid(row=irow, column=6, sticky=W)

        label_z_curr = Label(f, text="Z")
        label_z_curr.grid(row=irow, column=7, sticky=E)

        label_z_curr_val = Label(f, text="0", textvariable='curr_z')
        label_z_curr_val.grid(row=irow, column=8, sticky=W)

        self.curr_x = DoubleVar(f, 0, 'curr_x')
        self.curr_y = DoubleVar(f, 0, 'curr_y')
        self.curr_z = DoubleVar(f, 0, 'curr_z')

        row += 1
        irow = 0

        f = LabelFrame(self.root, text="Image settings")
        f.grid(row=row, column=0, columnspan=1, sticky=W+E, padx=5, pady=5, ipady=5)

        label_file_name = Label(f, text="Image file")
        label_file_name.grid(row=irow, column=0, sticky=E)

        self.entry_img_file_name = Entry(f, state=DISABLED)
        self.entry_img_file_name.grid(row=irow, column=1, columnspan=2, sticky=W+E, padx=5)

        self.button_open_file = Button(f, text="Open", command=lambda: self.on_open_image_clicked())
        self.button_open_file.grid(row=irow, column=3, pady=10)

        irow +=1

        label_width = Label(f, text="Width (mm)")
        label_width.grid(row=irow, column=0, sticky=E)

        entry_width = Entry(f, textvariable='width')
        entry_width.grid(row=irow, column=1, padx=5)
        self.width = DoubleVar(f, 100.0, 'width')

        label_height = Label(f, text="Height (mm)")
        label_height.grid(row=irow, column=2, sticky=E)

        entry_height = Entry(f, textvariable='height')
        entry_height.grid(row=irow, column=3, padx=5)
        self.height = DoubleVar(f, 100.0, 'height')

        irow += 1

        label_gcode_step = Label(f, text="G-Code step")
        label_gcode_step.grid(row=irow, column=0, sticky=E)

        entry_gcode_step = Entry(f, textvariable='gcode_step')
        entry_gcode_step.grid(row=irow, column=1, padx=5)
        self.gcode_step = DoubleVar(f, 0.15, 'gcode_step')

        label_feedrate = Label(f, text="Feedrate")
        label_feedrate.grid(row=irow, column=2, sticky=E)

        entry_feedrate = Entry(f, textvariable='feedrate')
        entry_feedrate.grid(row=irow, column=3, padx=5)
        self.feedrate = DoubleVar(f, 1000.0, 'feedrate')

        irow += 1

        label_gcode_file = Label(f, text="G-Code file")
        label_gcode_file.grid(row=irow, column=0, sticky=E)

        self.entry_gcode_file = Entry(f, state=DISABLED)
        self.entry_gcode_file.grid(row=irow, column=1, columnspan=2, sticky=W+E, padx=5)

        self.button_save_gcode = Button(f, text="Save", command=lambda: self.on_save_gcode_clicked())
        self.button_save_gcode.grid(row=irow, column=3, pady=10)

    ####################################################
    #
    def on_connect_clicked(self):
        if self.eth_comm.connected:
            try:
                self.button_connect.config(text="Connect")
                self.eth_comm.laser_off()
                self.eth_comm.disconnect()
            except:
                pass
        else:
            self.button_connect.config(text="Disconnect")
            self.eth_comm.connect(self.entry_board_address.get(), lambda x, y, z: self.on_xyz(x, y, z))
            self.eth_comm.laser_off()
            self.eth_comm.reset_coordinates()
            # self.on_follow_xyz_changed()

    ####################################################
    #
    def on_mqtt_connect_clicked(self):
        if not self.mqtt_comm.is_connected():
            address = self.entry_mqtt_address.get()
            try:
                self.mqtt_comm.connect(address)
            except:
                tkMessageBox.showerror("Error", "Coudln't connect to %s" % address)
        else:
            self.mqtt_comm.disconnect()

    ####################################################
    #
    def on_laser_level_clicked(self, mode):
        self.change_laser_mode(mode)

    ####################################################
    #
    def change_laser_mode(self, mode):
        if mode == 0:
            self.eth_comm.laser_off()
        elif mode == 1:
            if self.use_init_pwm_value.get() > 0:
                self.eth_comm.laser_on(self.pwm_value.get(), self.init_pwm_value.get())
            else:
                self.eth_comm.laser_on(self.pwm_value.get())
        elif mode == 2:
            if self.use_init_pwm_value.get() > 0:
                self.eth_comm.laser_auto(self.pwm_value.get(), self.init_pwm_value.get())
            else:
                self.eth_comm.laser_auto(self.pwm_value.get())
        elif mode == 3:
            self.eth_comm.laser_on(10)

        self.laser_mode = mode

    ####################################################
    #
    def on_open_image_clicked(self):
        filename = tkFileDialog.askopenfilename(initialdir=self.last_used_dir, title="Choose B&W image file", defaultextension=".bmp")
        if filename:
            self.last_used_dir = path.dirname(filename)
            self.img_handler.open_image(filename)
            self.entry_img_file_name.config(state=NORMAL)
            self.entry_img_file_name.delete(0, END)
            self.entry_img_file_name.insert(0, filename)
            self.entry_img_file_name.config(state=DISABLED)

    ####################################################
    #
    def on_save_gcode_clicked(self):
        filename = tkFileDialog.asksaveasfilename(
            initialdir=path.dirname(self.entry_gcode_file.get()), title="Save G-Code file", defaultextension=".nc"
        )
        if filename:
            try:
                self.last_used_dir = path.dirname(filename)
                width = self.width.get()
                height = self.height.get()
                vstep = self.gcode_step.get()
                feedrate = self.feedrate.get()
                self.img_handler.save_gcode(filename, width, height, vstep, feedrate)
                self.entry_gcode_file.config(state=NORMAL)
                self.entry_gcode_file.delete(0, END)
                self.entry_gcode_file.insert(0, filename)
                self.entry_gcode_file.config(state=DISABLED)
                tkMessageBox.showinfo("Info", "%s was successfully written" % filename)
            except:
                print "Unexpected error:", sys.exc_info()[0]
                tkMessageBox.showerror("Error", "Coudln't save %s" % filename)

    ####################################################
    #
    def on_pwm_slider_changed(self):
        if self.laser_mode == 1:
            self.eth_comm.laser_on(self.pwm_value.get())
        elif self.laser_mode == 2:
            if self.use_init_pwm_value.get() > 0:
                self.eth_comm.laser_auto(self.pwm_value.get(), self.init_pwm_value.get())
            else:
                self.eth_comm.laser_auto(self.pwm_value.get())

    ####################################################
    #
    def reset_xy(self):
        self.x = 0
        self.y = 0
        self.eth_comm.reset_coordinates()

    ####################################################
    #
    def on_xyz(self, x, y, z):
        (impulses_per_mm_x, impulses_per_mm_y) = (self.x_res.get(), self.y_res.get())

        (x_init, y_init) = (self.x_init.get(), self.y_init.get())

        (width, height) = (self.width.get(), self.height.get())

        (offset_x_impulses, offset_y_impulses) = (
            impulses_per_mm_x * x_init,
            impulses_per_mm_y * y_init
        )

        # print ">>>> %f, %f" % (x, y)
        self.curr_x.set((x + offset_x_impulses) / impulses_per_mm_x)
        self.curr_y.set((y + offset_y_impulses) / impulses_per_mm_y)

        if not self.img_handler.has_image():
            return

        (img_width, img_height) = self.img_handler.image_size()

        (impulses_per_pixel_x,  impulses_per_pixel_y) = (
            impulses_per_mm_x * width / float(img_width),
            impulses_per_mm_y * height / float(img_height)
        )

        (current_pixel_x, current_pixel_y) = (
            int((x + offset_x_impulses) / impulses_per_pixel_x),
            int((y + offset_y_impulses) / impulses_per_pixel_y)
        )

        pixel = self.img_handler.pixel_at(current_pixel_x, current_pixel_y)
        # print "Pixel value %d" % pixel
        if pixel == 0 and self.laser_state != 1:
            laser_pwm = self.pwm_value.get()
            if self.use_init_pwm_value.get() > 0:
                self.eth_comm.laser_on(self.pwm_value.get(), self.init_pwm_value.get())
            else:
                self.eth_comm.laser_on(self.pwm_value.get())
            self.laser_state = 1
        elif pixel > 0 and self.laser_state != 0:
            self.eth_comm.laser_off()
            self.laser_state = 0

    ####################################################
    #
    def on_follow_xyz_changed(self):
        self.eth_comm.follow_xyz(self.follow_xyz.get())

    ####################################################
    #
    def on_mqtt_connected(self):
        self.button_mqtt_connect.config(text="Disconnect")

    ####################################################
    #
    def on_mqtt_disconnected(self):
        self.button_mqtt_connect.config(text="Connect")

    ####################################################
    #
    def set_pwm_value(self, value):
        self.pwm_value.set(value)

    ####################################################
    #
    def set_init_pwm_value(self, value):
        self.init_pwm_value.set(value)

    ####################################################
    #
    def set_dash_length(self, value):
        self.eth_comm.dash_length(value)


UI().root.mainloop()
