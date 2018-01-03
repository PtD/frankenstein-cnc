from PIL import Image
import tkMessageBox

##################################################
#
class ImageHandler:
    def __init__(self):
        self.img = None
        self.width = None
        self.height = None

    def open_image(self, filename):
        try:
            print "Opening %s" % filename
            self.img = Image.open(filename)
            self.img.load()
            (self.width, self.height) = self.img.size
            print "Image size is %d x %d" % (self.width, self.height)
            pass
        except:
            tkMessageBox.showerror("Error", "Couldn't open image file %s" % filename)

    def pixel_at(self, x, y):
        if x < 0 or x >= self.width or y < 0 or y >= self.height:
            return 255
        else:
            return self.img.getpixel((x, self.height - (y + 1)))

    def image_size(self):
        return (self.width, self.height)

    def save_gcode(self, gcode_filename, width, height, vstep, feedrate):
        if self.img:
            self.save_optimized_gcode(gcode_filename, width, height, vstep, feedrate)
        else:
            self.save_unoptimized_gcode(gcode_filename, width, height, vstep, feedrate)

    def save_unoptimized_gcode(self, gcode_filename, width, height, vstep, feedrate):
        print "Writing to %s, feedrate is %d, vertical step is %f" % (gcode_filename, feedrate, vstep)
        out = open(gcode_filename, 'w')
        out.write('(Width {0} mm, Height {1} mm)\n'.format(width, height))
        out.write('G21\n')  # work in millimeters
        out.write('F{0}\n'.format(feedrate))
        out.write('G0 X-1Y-1\n')
        for y in range(0, int(height / vstep) + 1):
            if y % 2 == 0:
                out.write('G1 X{0}\n'.format(width + 1, feedrate))
            else:
                out.write('G1 X-1\n'.format(feedrate))
            out.write('G0 Y{0}\n'.format((y + 1) * vstep))
        out.close()

    ####################################################
    # width, height - in mm
    # vstep - in mm
    # feedrate - in mm/min
    def save_optimized_gcode(self, gcode_filename, width, height, vstep, feedrate):
        if self.img is None:
            return

        px_to_mm = lambda x, y: (x * float(width) / float(self.width), y * float(height) / float(self.height))
        mm_to_px = lambda x, y: (int(x * float(self.width) / float(width)), int(y * float(self.height) / float(height)))

        print "Writing to %s, feedrate is %d, vertical step is %f" % (gcode_filename, feedrate, vstep)
        out = open(gcode_filename, 'w')
        out.write('(Width {0} mm, Height {1} mm)\n'.format(width, height))
        out.write('G21\n')  # work in millimeters
        out.write('F{0}\n'.format(feedrate))
        out.write('G0 X-1Y0\n')
        out.write('M01\n')

        scanline = vstep
        (prev_x_left, prev_x_right) = (-1, self.width)
        moving_right = True
        while scanline < height:
            print "Scanline %f" % scanline
            (col, row) = mm_to_px(0, scanline)
            (x_left, x_right) = (self.find_leftmost_0_pixel(row), self.find_rightmost_0_pixel(row))
            # move to next row if there are no black pixels in the current one
            if x_left >= 0:
                if moving_right:
                    start = min(prev_x_left, x_left) - 1
                else:
                    start = max(prev_x_right, x_right) + 1

                out.write('G1 X{0}\n'.format(px_to_mm(start, 0)[0]))
                out.write('G0 Y{0}\n'.format(scanline))

                (prev_x_left, prev_x_right) = (x_left, x_right)
                moving_right = not moving_right

            scanline += vstep

        out.write("M02\n")
        out.close()


    def find_leftmost_0_pixel(self, y):
        for x in range(0, self.width):
            if self.img.getpixel((x, self.height - (y + 1))) == 0:
                return x

        return -1

    def find_rightmost_0_pixel(self, y):
        for x in reversed(range(0, self.width)):
            if self.img.getpixel((x, self.height - (y + 1))) == 0:
                return x

        return self.img.size[0]

    def pixel_to_mm(self, px, py, width, height):
        pass

    def mm_to_pixel(self, mmx, mmy, width, height):
        pass

    def has_image(self):
        return self.img is not None


#h = ImageHandler()
#h.open_image("c:/users/pedro/000-13776642-skull-and-crossbones-figure-illustration-on-white-background.bmp")
#h.save_optimized_gcode("c:/users/pedro/test.gcode", 860.0, 750.0, 0.15, 700.0)