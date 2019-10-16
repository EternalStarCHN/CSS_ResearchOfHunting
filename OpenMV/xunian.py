THRESHOLD = (50, 100, -127, -2, -128, 53) # Grayscale threshold for dark things...
import sensor, image, time
import json
from pyb import UART
sensor.reset()
sensor.set_vflip(True)
sensor.set_hmirror(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQQVGA) # 80x60 (4,800 pixels) - O(N^2) max = 2,3040,000.
#sensor.set_windowing([0,20,80,40])
sensor.skip_frames(time = 2000)     # WARNING: If you use QQVGA it may take seconds
clock = time.clock()                # to process a frame sometimes.
uart = UART(3,9600)
while(True):
    clock.tick()
    img1 = sensor.snapshot().binary([THRESHOLD])
    img = img1.invert()             # use this if the line is in dark.
    line = img.get_regression([(100,100,0,0,0,0)], robust = True)
    if (line):
        rho_err = abs(line.rho())-img.width()/2
        if line.theta()>90:
            theta_err = line.theta()-180
            img.draw_line(line.line(), color = 127)
        else:
            theta_err = line.theta()
            img.draw_line(line.line(), color = 127)
        print(rho_err,line.magnitude(),theta_err)
        abs_rho = json.dumps(abs(rho_err))
        abs_theta = json.dumps(abs(theta_err))
        if rho_err < 0:
            r_ss = 0
            rho_ss = json.dumps(r_ss)
        else:
            r_ss = 1
            rho_ss = json.dumps(r_ss)
        if theta_err < 0:
            t_ss = 0
            theta_ss = json.dumps(t_ss)
        else:
            t_ss = 1
            theta_ss = json.dumps(t_ss)
        verify = 'A'
        uart.write(rho_ss)
        uart.write(abs_rho)
        uart.write(theta_ss)
        uart.write(abs_theta)
        uart.write(verify)
    pass
    #print(clock.fps())
