import serial
import time


port = 'COM1'
baud = 9600
RAW_MAG = b"\x01"
TRUE_HDG = b"\x02"
MAG_HDG = b"\x09"
CAL = b"\x56"
OFF = b"\x00"
AUTO = b"\x01"
MANUAL = b"\x02"
RESET = b"\xFF"
CAL_QUAL = b"\x08"
PANDR = b"\x06"
FILTER = b"\x90"
FILTER_M = b"\x00"
FILTER_N = b"\x20"
SP_TEMP = b"\x11"
MTG = b"\x4A"
HORIZ = b"\x00"
VERT = b"\x01"

SPARTON_START_CHAR = b"\xA4"
SPARTON_END_CHAR = b"\xA0"

class FLEX:
    def __init__(self):
        self.ser = serial.Serial()
        self.ser.port = port
        self.ser.baud = baud
        self.ser.timeout = 1
        self.ser.writeTimeout = 1

        self.heading = []
        self.pitch = []
        self.roll = []
        self.power = False
        self.FilterSet = False
        self.MtgSet = False
        
    def SP300xD_2_PowerOn(self):
        print("Power On")
        self.ser.open()
        self.ser.flush()
        self.power = True
        return

    def SP300xD_2_PowerOff(self):
        print("Power Off")
        self.ser.close()
        self.power = False
        return
    
    def SP300xD_2_Autocal(self):
        self.ser.write(SPARTON_START_CHAR)
        self.ser.write(CAL)
        self.ser.write(AUTO)
        self.ser.write(SPARTON_END_CHAR)
        self.ser.flushOutput()
        return

    def SP300xD_2_Filter_Set_Command(self):
        self.ser.write(SPARTON_START_CHAR)
        self.ser.write(FILTER)
        self.ser.write(FILTER_M)
        self.ser.write(FILTER_N)
        self.ser.write(SPARTON_END_CHAR)
        self.ser.flushOutput()
        return

    def SP300xD_2_Mtg_Config_Command(self):
        self.ser.write(SPARTON_START_CHAR)
        self.ser.write(MTG)
        self.ser.write(HORIZ)
        self.ser.write(SPARTON_END_CHAR)
        self.ser.flushOutput()
        return

    def SP300xD_2_Direction_Command(self):
        self.ser.write(SPARTON_START_CHAR)
        self.ser.write(MAG_HDG)
        self.ser.write(SPARTON_END_CHAR)
        time.sleep(0.1)
        self.ser.flushOutput()
        return

    def SP300xD_2_Tilts_Command(self):
        self.ser.write(SPARTON_START_CHAR)
        self.ser.write(PANDR)
        self.ser.write(SPARTON_END_CHAR)
        self.ser.flushOutput()
        return

    def SP300xD_2_DPR_Command(self):
        #temp = []
        #temp = "%s%s%s%s%s%s" % (SPARTON_START_CHAR,MAG_HDG,SPARTON_END_CHAR,SPARTON_START_CHAR,PANDR,SPARTON_END_CHAR)
        #print(temp)
        temp = b"\xA4\x09\xA0\xA4\x06\xA0"
        self.ser.write(temp)
##        self.ser.write(SPARTON_START_CHAR)
##        self.ser.write(MAG_HDG)
##        self.ser.write(SPARTON_END_CHAR)
##        self.ser.write(SPARTON_START_CHAR)
##        self.ser.write(PANDR)
##        self.ser.write(SPARTON_END_CHAR);
        #temp = ''.join(temp)
        #print(temp)
        #self.ser.write(temp)
        self.ser.flushOutput()
        return
        
    def Get_SP300xD_2_Bytes(self,numbytes):
        data = self.ser.read(numbytes)
        #self.ser.flushInput()
        return data

    def Get_SP300XD_2_Data(self):
        #Get the Heading value
        self.SP300xD_2_Direction_Command()
        time.sleep(0.25)
        data = self.Get_SP300xD_2_Bytes(5)
        #print(data[1])
        if(len(data) > 4 and data[1] == int.from_bytes(MAG_HDG,byteorder='big',signed=False)):
            #print("HEADgood= ")
            #print(data)
            dint = int.from_bytes(data[2:4],byteorder='big',signed=False)
            self.heading = (dint * 360.0) / 4096
        else:
            #print("HEADbad = ")
            #print(data)
            self.heading = 1E35
        #print(self.heading)

        #Get the Pitch/Roll values
        self.SP300xD_2_Tilts_Command()
        time.sleep(0.1)
        data = self.Get_SP300xD_2_Bytes(7)
        #print(data[1])
        if(len(data) > 6 and data[1] == int.from_bytes(PANDR,byteorder='big',signed=False)):
            #print("PANDRgood = ")
            #print(data)
            dint = int.from_bytes(data[2:4],byteorder='big',signed=True)
            self.pitch = (dint * 90)/4096
            dint = int.from_bytes(data[4:6],byteorder='big',signed=True)
            self.roll = (dint * 180)/4096
        else:
            #print("PANDRbad = ")
            #print(data)
            self.pitch = 1E35
            self.roll = 1E35

        #print(self.pitch)
        #print(self.roll)
        return

    def Print_SP300xD_2_Q(self):
        return

    def Init_Sp300xD_2(self):
        #turn on
        self.SP300xD_2_PowerOn()
        time.sleep(1)

        #set the filter
        print("Set Filter")
        self.SP300xD_2_Filter_Set_Command()
        flg = False
        idx = 0
        while(flg == False and idx < 3):
            data = self.Get_SP300xD_2_Bytes(5)
            if(len(data) > 3 and data[3] != FILTER_N):
                time.sleep(1)
                idx += 1
                print(idx)
            else:
                flg = True
            
        if(flg == False and idx >= 3):
            print("Filter Not Set")
            self.FilterSet = False
        else:
            print("Filter Set")
            self.FilterSet = True
            
        #Set the mount
        print("Set Mount")
        self.SP300xD_2_Mtg_Config_Command()
        flg = False
        idx = 0
        while(flg == False and idx < 3):
            data = self.Get_SP300xD_2_Bytes(4)
            
            if(len(data) > 2 and (data[2] != 0 and data[2] != 1)):
                time.sleep(1)
                idx += 1
            else:
                flg = True

        if(flg == False and idx >= 3):
            print("Mount not set")
        else:
            if(data[2] == 0):
                print("Horizontal mtg set")
                self.MtgSet = True
            elif(data[2] == 1):
                print("Vertical mtg set")
                self.MtgSet = True
        return

    def ClearInvalid(self):
        #for i in range(0,256):
        #    self.ser.write("\n")
        self.SP300xD_2_Direction_Command()
        time.sleep(0.5)
        self.ser.flushInput()
        self.ser.flushOutput()
    

    def Test1_SP300xD_2_InvalidValues(self):
        print("Test Invalid Values")
        for i in range(0,10):
            self.ser.write(i)

        self.ClearInvalid();
        return

    def Get_SP300XD_2_Data2(self):
        #Get the Heading value
        self.SP300xD_2_DPR_Command()
        #time.sleep(0.25)
        data = self.Get_SP300xD_2_Bytes(5)
        #print(data[1])
        if(len(data) > 4 and data[1] == int.from_bytes(MAG_HDG,byteorder='big',signed=False)):
            #print("HEADgood= ")
            #print(data)
            dint = int.from_bytes(data[2:4],byteorder='big',signed=False)
            self.heading = (dint * 360.0) / 4096
        else:
            #print("HEADbad = ")
            #print(data)
            self.heading = 1E35
        #print(self.heading)

        #Get the Pitch/Roll values
        
        time.sleep(0.1)
        data = self.Get_SP300xD_2_Bytes(7)
        #print(data[1])
        if(len(data) > 6 and data[1] == int.from_bytes(PANDR,byteorder='big',signed=False)):
            #print("PANDRgood = ")
            #print(data)
            dint = int.from_bytes(data[2:4],byteorder='big',signed=True)
            self.pitch = (dint * 90)/4096
            dint = int.from_bytes(data[4:6],byteorder='big',signed=True)
            self.roll = (dint * 180)/4096
        else:
            #print("PANDRbad = ")
            #print(data)
            self.pitch = 1E35
            self.roll = 1E35

        #print(self.pitch)
        #print(self.roll)
        return

f = FLEX()

f.Init_Sp300xD_2()
#f.SP300xD_PowerOn()
#f.SP300xD_2_Direction_Command()
#f.Get_SP300XD_2_Data()
f.Test1_SP300xD_2_InvalidValues()
f.Get_SP300XD_2_Data()



for i in range(0,100):
    time.sleep(1)
    f.Get_SP300XD_2_Data2()
    print(str(f.heading) + ',' + str(f.pitch) + ',' + str(f.roll))


f.SP300xD_2_PowerOff()
