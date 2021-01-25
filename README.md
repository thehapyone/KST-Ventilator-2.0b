# KST Ventilator 2.0 Project Documentation 

## Flow Chart
The Flow Chart or psudeo code used for this project so far is available in this draw.io link below. 
https://drive.google.com/file/d/1QWlhkn7i-TUC2WAttREYT4iai9oc0Win/view?usp=sharing

## Display Design 
The graphics used for the display was designed with the Canva (A free online design platform). The live deisgn document is available here
https://www.canva.com/design/DAEP6nqmQ9w/share/preview?token=X7omEkEh8P3v0QsDclBK6Q&role=EDITOR&utm_content=DAEP6nqmQ9w&utm_campaign=designshare&utm_medium=link&utm_source=sharebutton


## Development Environment
The software development environment is based on two platforms - 
 - Visual Studio Code using the Platform.io extension
 - Arduino IDE

 Athough, the codes are developed in Visual studio, a full compatible arduino code is also generated. Moroever, the cpp files from Visual studio can directly by used in Arduino by just changing the main.cpp to main.ino and including all the dependencies in the root folder.


## Project Setup
Here, you find some information about the project setup
 ### Codes
  - The src codes are located in the /src directory. The main.cpp is the source code when using the Visual studio environment. 
  - The Arduino codes are located in /src/Arduino Equivalent Code directory. The ventilator.ino houses the latest Arduino code.
  - The libaries used for this project are located in the /lib directory. Here two main libaries are used: EasyNextion and Honeywell. The EasyNextion libary (https://github.com/Seithan/EasyNextionLibrary) is an opensource libaray used in the communication with the Nextion screen. The Honeywell library is used for communication with the pressure sensor and was developed by me (Ayo - https://github.com/thehapyone)

### Display
The display used in this project is the Nextion NX4832t035 3.5inch touchsreen display. 
#### Communication
Communication to the display is with UART. SoftwareSerial on the Arduino is used to initiate communicate to the Screen. SoftwareSerial is used on the Arduino Uno because the Arduino Uno only has one HardwareSerial capability and to free it for debugging purpose. SoftwareSerial was defined in Pin 5 and 6 representing RX and TX respectively. 
 - The EasyNextion Library is used for coding interation with the display.

#### Source Code and Assets
 - The display interface development is done with the Nextion application. The source files for the display is located in /src/display.

### Sensors
The below are the sensors available for this project. 
 - Pressure Sensor: The Honeywell ABPDRRV060MGSA3 Gauge Pressure Sensor. It's an SPI 3.3V capable of measuring between 0 - 60mbar. It was connected to Arduino through the SPI ports.
 - MPL115A2 I2C Barometric Pressure and Temperature Sensor: The sensor has not be incorporated.

 ### Motors
 Two motors are available -
  - Standard DC motor
  - Brushless DC Motor with a Speed Controller

 ### Connection
 The below image shows the device connection and interfaces.


 ### Changelogs
 Here is a summary of the development change history -
 '''c
 /**
 * v0.1 #######################################
 * - Simple breathing patten using constant motor speed value and cpu blocking (delay)
 * - No support for exhale pressure assist (Motor is disabled during expiratory)
 * v0.2 #######################################
 * - CPU blocking removed.
 * - Support for setting the breaths per minute
 * - Support for setting the inhale and exhale duration
 * - Support for setting the inhale and exhale speed
 * - Support for measuring motor current usage
 * v0.21 ########################################
 * - Addition of pressure sensor support.
 * v0.22 ########################################
 * - Migrate code to support new motor : Brushless DC motor with an ESC
 * v0.23 ########################################
 * - Added display support. The device can be monitored and controlled from a touchsreen display
 **/
 '''
