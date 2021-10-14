/**************************************************************************************
* MiniESR Ver. 2.1.5
* Created by Suhwan Hong with Evan Demos and Ryan Dunning
* for any questions, contact me at suhwanhong999@gmail.com
**************************************************************************************/
To power up device: 
Connect the power supply to the connector on the left of the screen. 
Power on by flipping the switch towards the white dot. This is the ON position
Connect the USB cable attached to the computer to the device. 
A orange light should be on, indicating the device is powered. 

//////////////////////////////////////////////////////////////////////////////////////////

To power down device: 
Disconnect the USB cable first. 
Then, flip the switch to the OFF position. 
Finally, disconnect the power supply cable. 
*In emergency, FLIP THE SWITCH FIRST

//////////////////////////////////////////////////////////////////////////////////////////

This is the instructions on how to run the miniESR without the computer interface 

First, install Arduino on your computer 
(https://www.arduino.cc/en/Main/Software)

Then, open up the ProfileLoader.ino file in the folder named "ProfileLoader" 
Scroll down to "PROTOCOL SECTION" 
Change any parameters as needed, following the directions written into the code.
Save the program by going to File -> Save.

Upload the file by clicking the '->' button loacted on the top left corner 
A green progress bar will appear on the bottom right. 
Once the profile loading is complete, the screen on the miniESR will display: 
"Profile Loaded" 
Close the ProfileLoader program. 

Load the MasterCode2.1.5.ino file to the machine using the same steps as before.
When PCR is ready to run, press the start button. (the middle of the 3) 

The button loacated closest to the screen switches the display function. 
One displays the current temperature and the other displays the number of cycle it's on.

//////////////////////////////////////////////////////////////////////////////////////////