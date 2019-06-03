import serial
import io


ser = serial.Serial('/dev/ttyS5')
# print(ser.name)
ser.baudrate = 115200
while(1>0):
    print(ser.read(1))
    os.system("aplay TomStyrma2.wav")



    # print('here')
