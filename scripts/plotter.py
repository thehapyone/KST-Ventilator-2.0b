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

x_data, y_target , y_current , y_state , y_pressure= [], [], [], [], []
figure = pyplot.figure()
lineTarget, =  pyplot.plot(x_data, y_target, 'g-', label='target')
lineCurrent, = pyplot.plot(x_data, y_current, 'b-', label='current')
lineState, = pyplot.plot(x_data, y_state, 'k-', label='state')
linePressure, = pyplot.plot(x_data, y_pressure, 'r-', label='output')

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

def parseData(readData):
    '''
    parse the data received
    '''
    global flowStartTime
    match = re.search(r'S:(\d+),SS:(\d+),T:(\d+),C:(\d+),O:(\d+);', readData)
    if match:
        state = (int(match.group(1)) - 0xA0 ) * 10
        state += int(match.group(2)) - 0xA0
        if state > 45 and state < 65:
            flowStartTime = time.time()            
        target = int(match.group(3))
        current = int(match.group(4))
        current /= 1000.0
        output = int(match.group(5))
        output /= 1000.0
        return (state, target, current, output)
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
    status = parseData(readData)
    if status != None:  
        time.sleep(0.1)
        write(messageID, pressure)        
        (state, target, current, output) = status
        return [state, target, current,  output]
    print(readData)
    return None

def updateLines (lines, datas, values, amountNeeded):
    '''
    process the data
    '''
    xdata = None
    for (linex, data, value) in list(zip(lines, datas, values)):
        data.append(value)
        if len(data) > amountNeeded:
            startIndex = len(data)-amountNeeded
            data = data[startIndex:]
        if linex == None:
            xdata = data
        else:
            linex.set_data(xdata, data)
    return


def update(frame):
    '''
    draws the data
    '''
    global x_data, y_target, y_current, y_state, y_pressure
    elapsedTime = time.time()-startTime
    
    data = read()
    if data is None:
        return
    try:
        amountToPlot = 100
        [state, target, current, pressure] = data
        values = [elapsedTime, target, current, state, pressure]
        datas = [x_data, y_target, y_current, y_state, y_pressure]
        lines = [None, lineTarget, lineCurrent, lineState, linePressure]
        updateLines(lines, datas, values, amountToPlot)
        figure.gca().relim()
        figure.gca().autoscale_view()
    except Exception as e:
       print("Exception %s"%(str(e)))
    return y_target, y_current, y_state, y_pressure

animation = FuncAnimation(figure, update, interval=250)
pyplot.legend()
pyplot.show()