
import os

def readData():
    pressureData = "Pressure (breathing modeS) - Current des.txt"
    i = 1
    with open(pressureData) as fileStream:
        for line in fileStream:
            print ("%i, %s"%(i, fileStream.readline()))
            i += 1


if __name__ == "__main__":
    readData()