from datetime import datetime
#python -m pip install pyserial
from matplotlib import pyplot
from matplotlib.animation import FuncAnimation
from random import randrange
import time
import math
import serial
from serial import Serial
import re
import struct
import random
import math
#python -m pip install pyserial
ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.xonxoff = False
ser.rtscts = False
ser.dtr = False
ser.timeout = 1
ser.write_timeout = 1
ser.open()

x_data, y_data , y_data2 , y_data3 , y_data4= [], [], [], [], []
figure = pyplot.figure()
line, =  pyplot.plot(x_data, y_data, 'g-', label='target')
line2, = pyplot.plot(x_data, y_data2, 'b-', label='current')
line3, = pyplot.plot(x_data, y_data3, 'k-', label='state')
line4, = pyplot.plot(x_data, y_data4, 'r-', label='pressure')

startTime = time.time()
initValue = 1
strRead = ""
values = []
counter = 1
flowStartTime = None

def write(messageID, data):
    '''
    write byte array
    '''
    try:
        initValue = ((messageID << 24) + data) & 0xFFFFFFFF
        global values
        values = bytearray(struct.pack("<I", initValue))
        ser.write(values)
        ser.write(b';')
        ser.flushOutput()
    except Exception as e:
        print ("%d %s"%(data, str(e)))

def simulatePressure():
    '''
    simulate a sine wave
    using the
    '''
    global flowStartTime, counter
   
    if flowStartTime == None:
        return 0
    timeElapsed = (time.time() - flowStartTime) % 60
    pressure = (0.2 + math.sin(timeElapsed*12* 3.14/180)) * 100
    pressure = (int)(1000 *pressure)
    return pressure


def setStartTime(readData):
    '''
    sync 
    '''
    global flowStartTime
    match = re.search(r'26(\d+)', readData)
    if match:
        try:     
            data = (int)(match.group(1).encode('utf-8'))
            flowStartTime = data + time.time()
            print ("sync value is %d"%data)
            return data
        except Exception as e:
            print (str(e))    
    return None

def processData(readData):
    '''
    parse the data received
    '''
    global flowStartTime
    match = re.search(r'S:(\d+),SS:(\d+),T:(\d+),C:(\d+);', readData)
    if match:
        state = (int(match.group(1)) - 0xA0 ) * 10
        state += int(match.group(2)) - 0xA0
        if state > 45 and state < 65:
            flowStartTime = time.time()            
        target = int(match.group(3))
        current = int(match.group(4))
        current /= 1000.0
        return (state, target, current)
    return None

def read():
    '''
    read the data from the controller 
    '''
    global strRead, flowStartTime
    messageID = 0x18
    availData = ser.in_waiting
    if availData < 1:
        return None
    pressure = simulatePressure()    
    readData = ser.readline(availData).decode('utf-8')    
    status = processData(readData)
    if status != None:  
        time.sleep(0.1)
        write(messageID, pressure)        
        (state, target, current) = status
        return (state, target, current, pressure/1000.0)
    print(readData)
    return None

def update(frame):
    '''
    draws the data
    '''
    global x_data, y_data, y_data2, y_data3, y_data4
    elapsedTime = time.time()-startTime
    
    data = read()
    if data is None:
        return
    x_data.append(elapsedTime)
    try:
        amountToPlot = 100
        (state, target, current, pressure) = data
        y_data.append(target)        
        y_data2.append(current)
        y_data3.append(state)
        y_data4.append(pressure)
        [lenx1, leny11, leny12,  leny13, leny14] = [len(x_data), len(y_data), len(y_data2), len(y_data3), len(y_data4)]
        availData = len(x_data)
        if availData > amountToPlot:
            startIndex = availData-amountToPlot
            x_data = x_data[startIndex:]
            y_data = y_data[startIndex:]
            y_data2 = y_data2[startIndex:]
            y_data3 = y_data3[startIndex:]
            y_data4 = y_data4[startIndex:]
        [lenx, leny1, leny2, leny3, leny4] = [len(x_data), len(y_data), len(y_data2), len(y_data3), len(y_data4)]
        line.set_data(x_data, y_data)
        line2.set_data(x_data, y_data2)
        line3.set_data(x_data, y_data3)
        line4.set_data(x_data, y_data4)
        figure.gca().relim()
        figure.gca().autoscale_view()
    except Exception as e:
       print("Exception %s"%(str(e)))
    return line,line2, line3, line4

animation = FuncAnimation(figure, update, interval=250)
pyplot.legend()
pyplot.show()