import json
import os


def getTargetFimeName():
    current_dir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), "src", "config.h")
    return current_dir

def initConfigFile(fileStream):
    fileStream.write("#ifndef CONFIG_H\n#define CONFIG_H\n")
    fileStream.write("/*This file is generated using python script\n\r\
                     Any data edited would be lost next time the script run*/\n")

def endConfigFile(fileStream):
    fileStream.write("#endif //CONFIG_H\n")
    fileStream.close()

def open_target_file(targetFile):
    try:
        fileStream = open(targetFile, 'w+')
        initConfigFile(fileStream)
    except:
        fileStream = None
    return fileStream

def writeToFile(name, value, fileStream):
    msg = "#define %s %s\n"%(name, value)
    if fileStream == None or fileStream.closed:
        return
    fileStream.write(msg)

def readJsonFile(configFile, targetFile):
    fileStream = open_target_file(targetFile)
    with open(configFile) as json_file:
        data = json.load(json_file)
        maxMotorOutPut = data['maxMotorOutPut']
        inhaleExhaleRatio = data['inhaleExhaleRatio']
        breathPerMin = data['breathPerMin']
        for (state, ratio, substatesId) in [('inhale', inhaleExhaleRatio, ["EnterInhale", "RunInhale", "ExitInhale"]),
                                            ('exhale', 1-inhaleExhaleRatio, ["EnterExhale", "RunExhale", "ExitExhale"])]:
            for substateId in substatesId:
                substate = data['states'][state][substateId]
                name = substate['name']
                order = substate['order']
                duration = substate['durationPercentile']
                substateduration = ((60*1000.0)/breathPerMin)*ratio*duration/100
                print ("Duration for state %s is %d"%(name,substateduration ))
                for subsubstateID in ["start", "repeat", "leave"]:
                    subsubstate = substate["subStates"][subsubstateID]
                    name = subsubstate['name']
                    order = subsubstate['order']
                    durationInMsName = name+'_DURATION_IN_MS'
                    durationInMsValue = subsubstate['durationPercentile'] * substateduration/100
                    desiredOutputName = name+'DESIRED_OUTPUT'
                    desiredOutputValue = subsubstate['desiredOutputPercentile'] * maxMotorOutPut/100
                    maxMotorSpeedName = name+'MAXMOTORSPEED'
                    maxMotorSpeedValue = 1
                    print ("......Duration for substate %s is %d %d"%(name,durationInMsValue, desiredOutputValue))
                    fileStream.write("\n//%s\n"%name)
                    writeToFile(desiredOutputName, desiredOutputValue, fileStream)
                    writeToFile(maxMotorSpeedName, maxMotorSpeedValue, fileStream)
                    writeToFile(durationInMsName, durationInMsValue, fileStream)         
                    
                    fileStream.write("\n")
        endConfigFile(fileStream)
if __name__ == "__main__":
    readJsonFile("configuration.json", getTargetFimeName())

