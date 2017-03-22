# How To Calibrate Gill Wind Sensors OS-5000S Compass for PMEL Instruments
The following documents the procedure for calibrating the Ocean Server OS-5000S
compass installed in a PMEL modified Gill Wind sensor housing.

-----

## Requirements

| NAME        | Description           | Qty  |
|:------------- |:-------------| :-----:|
| Wind Sensor     | *PMEL Modified Wind Sensor with OS-5000S Compass* | 1 |
| Cable | *Compass Test Cable* | 1 |
| Power Supply | *Power Supply or Battery (3.3 - 24VDC)* | 1 |
| Laptop | *Laptop with Ocean Server Compass Utility Installed | 1 |
| RS232 Adapter | USB-to-RS232 Device/Cable | 1 |
| Test Jig | Wind Sensor Test Jig with Pitch & Roll Gimbal | 1 |

<div style="page-break-after: always;"></div>

## Procedure
### Test Setup
- [ ]  Install the Wind Sensor
  - [ ] Attach the Cable to the Wind Sensor
  - [ ] Mount the Wind Sensor on the Test Jig
  - [ ] Align the gimbal so pitch & roll are at 0&deg;
  - [ ] Install the jig on the rotary monument with north indicators aligned.
  - [ ] Rotate turn-table to 0&deg;
  - [ ] Connect the RS232 Adapter to the Laptop
  - [ ] Connect the Power Supply to the Test Cable


### Software Setup
- [ ] Load the Ocean-Server Compass Utility Program

![Fig1](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_1.png)


&nbsp;
- [ ] Select "Scan for Compass" from the top menu

![Step_2](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Figures_Small\Cal_Step_2alt.png)



&nbsp;
  * You should see the following result.  If not, verify that the serial and power
  are connected and restart the test.

![Step_3](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_3alt.png)

&nbsp;
- [ ] With the compass "Alive", select the "Program" button from the top menu.

![Step_4](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_4.png)


<div style="page-break-after: always;"></div>
### X,Y Calibration

- [ ] Select ***"Calibration X,Y Compass Level: Start"*** from the menu (press button).
  * Press **Enter** to exit the pop-up window
  * When started, rotate the turntable clockwise so that it performs one revolution in 20+ seconds.
  * Press the **Spacebar** when complete

![Step_5](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_5.png)


-----

### Z Calibration

- [ ] Adjust the pitch axis of the gibal so that compass is 90&deg; to normal.  

- [ ] Align sensor North with 0&deg; on the monument

- [ ] Press ***"Calibrate Z; Compass @90 Degrees: Start"***
  * Press **Enter** to exit the pop-up window
  * Rotate the turntable through 360&deg; over 20+ seconds
  * Press the **Spacebar**

![Step_6](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_6.png)



<div style="page-break-after: always;"></div>
### Soft Iron Calibration
- [ ] Adjust the pitch axis of the gibal so that compass is 0&deg; to normal.  

- [ ] Align sensor North with 0&deg; on the monument

- [ ] Select ***"Soft Iron: On"*** Radio Button

- [ ] Press ***"Soft Iron: Calibrate"***

  * Press **Enter** to exit the pop-up window
  * Press the **Spacebar**
&nbsp;
- [ ] Rotate the turntable to 90&deg;
  * Press **Enter** to exit the pop-up window
  * Press the **Spacebar**
&nbsp;
- [ ] Rotate the turntable to180&deg;
  * Press **Enter** to exit the pop-up window
  * Press the **Spacebar**
&nbsp;
- [ ] Rotate the turntable to 270&deg;
  * Press **Enter** to exit the pop-up window
  * Press the **Spacebar**

![Step_7](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_7.png)


-----

### Trim Roll & Pitch
- [ ] Ensure the Pitch & Roll axis of the gibal so that compass is 0&deg; to normal.  

- [ ] Select ***"Trim Roll and Pitch (Tilt on <3&deg;): Set"***

![Step_9](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_9.png)



### Set Baud Rate and Data Format
- [ ] Select ***"Display"*** from the menu

- [ ] Select ***"Azimuth", "Pitch", "Roll"*** from check boxes.  Deselect all others

- [ ] Select ***"Set"*** To the right of check boxes
![Step_10b](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_10b.png)


&nbsp;
- [ ] Select ***"Baud Rate: 9600"*** from pull-down menu.

- [ ] Select ***"Set"***
![Step_10b4](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_10b4.png)


&nbsp;
- [ ] Select ***"Output Rate: 5 samples/sec"*** from menu.

- [ ] Select ***"Set"***

![Step_10b5](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_10b5.png)



- [ ] Select ***"Maths/Offsets"*** from the menu

- [ ] Ensure the following settings are correct.  If they are not, apply change
and press corresponding ***"Set"***

| Variable | Value |
| :------ | :------: |
| Set-Reset Rate | 100 sentences |
| Skip n first readings | 0 |
| Euler Math | 4=v2.4 |
| Average Samples | 4 |
| AD Decimation Filter | 3 |
| Deviation | **Enter Deviation if Required** |
| Declination | **Enter Declination if Required** |
| Temperature Offset | **DO NOT CHANGE** |
| Acc Range | 0 |
| Max Pressure [PSI] | N/A |

&nbsp;
- [ ] Select ***"Set"***





![Step_1](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_11.png)


&nbsp;

----

### Grab Compass Calibration Data

- [ ] Select ***"Compass Data"*** from top menu

- [ ] Copy, paste and save data in text document

![Step_12](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_12.png)


## Validate Compass Calibration

- [ ] Rotate the Turntable to 0&deg;

- [ ] Select ***"Output"*** from top menu

![Step_13](https://github.com/NOAA-PMEL/micro/blob/master/SpartonReplacement/Documents/CompassCalibration/Figures_Small/Cal_Step_13.png)


- [ ] Rotate the turntable to the following points and record the output.

| Turntable Heading | Recorded Value | Error (expected - actual) |
| :-------- |------:| ----: |
| 0&deg; |  &deg;| &deg;|
| 15&deg; | &deg;|&deg;|
| 30&deg; | &deg;|&deg;|
| 45&deg; | &deg;|&deg;|
| 60&deg; | &deg;|&deg;|
| 75&deg; | &deg;|&deg;|
| 90&deg; | &deg;|&deg;|
| 15&deg; | &deg;|&deg;|
| 120&deg; |  &deg;|&deg;|
| 135&deg; | &deg;|&deg;|
| 150&deg; | &deg;|&deg;|
| 165&deg; | &deg;|&deg;|
| 1800&deg; |  &deg;|&deg;|
| 195&deg; | &deg;|&deg;|
| 210&deg; | &deg;|&deg;|
| 225&deg; | &deg;|&deg;|
| 240&deg; | &deg;|&deg;|
| 255&deg; | &deg;|&deg;|
| 270&deg; | &deg;|&deg;|
| 285&deg; | &deg;|&deg;|
| 300&deg; | &deg;|&deg;|
| 315&deg; | &deg;|&deg;|
| 330&deg; | &deg;|&deg;|
| 345&deg; |  &deg;|&deg;|


## Calibration Complete
