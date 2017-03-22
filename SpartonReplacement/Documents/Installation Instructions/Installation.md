# MSP430 Programming Instructions

PMEL's Engineering Development Division uses MSP430 microcontrollers in many of
the custom circuits.  The following is the procedure for generating and
programming the microcontrollers, also known as "Flashing".

## Requirements
The following are the basic hardware and software requirements for generating and
loading binary files onto the microcontroller.
### Hardware
* Texas Instruments MSP-FET Flash Emulation Tool

### Software
* IAR Embedded Workbench for MSP430
* Elprotronic, Inc. FET-Pro-430-LITE
  * Free download from www.elprotronic.com  

## Generating "Binary" files
*This is adapted from the T.I. wiki page*
http://processors.wiki.ti.com/index.php/Generating_and_Loading_MSP430_Binary_Files

The IAR integrated development environment (IDE) and compiler do not generate a typical
binary file during compilation.  Therefore downloads to the microcontroller normally
require interfacing the programmer (MSP-FET) to the IDE.

Elprotronic, Inc. makes a program that will download the images to the microcontroller
by using an output file that can be generated at compilation time.  In order
to generate this file, complete the following:
1. In **Projects->Options** select the **Linker** tab (Figure 1).  In the **Output**
tab, select "Allow C-SPY-specific extra output file"

  ![Linker_1](C:\Users\Casari\version-control\PMEL\micro\SpartonReplacement\Documents\Installation Instructions\Linker_1.png)

2. In the **Extra Output** tab of the same **Linker** Menu, select "Generate extra output file".  
Under "Format", select Output Format: "msp430-txt"

  ![Linker_2](C:\Users\Casari\version-control\PMEL\micro\SpartonReplacement\Documents\Installation Instructions\Linker_2.png)

  * The output file will be found in the project directory, in Debug->Exe->Projectname.txt

## Loading "Binary" files to target
1. Open the FET-Pro-430 program
2. Press the button "Open Code File ->", navigate to the location of the "binary"
file (ProjectName.txt)
  * In the source code project folder, this is found in Debug->Exe->Projectname.txt

  ![Step_2](C:\Users\Casari\version-control\PMEL\micro\SpartonReplacement\Documents\Installation Instructions\FET_Pro430_Step2.png)

3. In the Microcontroller Type block
  * In "Group", select the general type of MSP430 used.
  * On the next line, select the specific type of MSP430 used.

  ![Step_3](C:\Users\Casari\version-control\PMEL\micro\SpartonReplacement\Documents\Installation Instructions\FET_Pro430_Step3.png)

4. In the "Device Action" block, press "AUTO PROG."
  * If successfull, the "Status" block should show **"Pass"**
  * If unsuccessfull, press "ERASE FLASH" and try step 4 again.

  ![Step_4](C:\Users\Casari\version-control\PMEL\micro\SpartonReplacement\Documents\Installation Instructions\FET_Pro430_Step4.png)
5. The device is now programmed
