MAX31865 Arduino Library
========================

The files in this folder provide an Arduino driver for the MAX31865 RTD chip and example code that illustrates how to use it.  

Original code was written by olewolf, modified by Charles-Henri Hallard

Code based on following datasheet http://datasheets.maximintegrated.com/en/ds/MAX31865.pdf

##Features
List of features I added from the original library:

- Added option to set Reference resistor at object instantiation
- Corrected threshold (set/get) not consistent problem
- added soft reconfiguration register with less option (to avoid SPI communication) and not setting again threshold
- added method to get reference resistor value `cfg_ref()`
- added method to get only fault status register `fault_status()`
- added method to get only configuration register `config_register()`

###Installation
Copy the content of this folder (zip file) library in your Arduino libraries folder in your sketchbook location, you should have now "your_sketchbook_folder/libraries/max31865" and this folder should contains the .cpp and .h files.
<br/>
To find your Arduino sketchbook location folder, in Arduino IDE, go to File>Preferences in the Arduino IDE.
<br/>
See [this tutorial][1] on Arduino libraries.
<br/>

###Sample usage
- [Read_Temperature][3] Basic reading temperature and serial display (original code author)
- [Full_Test_Oled][6] A powerfull testing sketch, the one I use to test my boards, It has full display on I2C OLED 128x64 and on Serial. Each step is detailled, it can see if breakout is connected, communication are good, and MAX31865 breakout board is well configured and see PT100/PT1000. It mainly use all library feature and use MAX31865 in 1-Shot mode.
- [Full_Test_Serial][7] same as above but without OLED display, serial only.

##Blog dedicated post
you can find it on my blog [here][5]

##Why
- When I created the MAX31865 breakout board, I needed a library to be able to read results. I then found this one which was working file. I just added on it the features I needed, thank to Original library author olewolf.

##License
Same as original, 

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

##Misc
See news and other projects on my [blog][4] 
 
[1]: http://learn.adafruit.com/arduino-tips-tricks-and-techniques/arduino-libraries
[2]: http://www.dsscircuits.com/index.php/articles/66-arduino-i2c-master-library
[3]: https://github.com/hallard/arduino-max31865/blob/master/Examples/Read_Temperature/Read_Temperature.ino
[4]: http://hallard.me
[5]: http://hallard.me/max31865/
[6]: https://github.com/hallard/arduino-max31865/blob/master/Examples/Full_Test_Oled/Full_Test_Oled.ino
[7]: https://github.com/hallard/arduino-max31865/blob/master/Examples/Full_Test_Serial/Full_Test_Serial.ino
