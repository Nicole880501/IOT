import socket
import RPi.GPIO as GPIO
import motor

bind_ip = "192.168.8.28"
#bind_ip = "192.168.0.228"

bind_port = 8888

limiDist = 30

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server.bind((bind_ip, bind_port))
server.listen(5)

try:
    client, addr = server.accept()
    print("Accepted connection from: %s:%d"%(addr[0], addr[1]))
    
    while True:
        distance = ultrasound.get_distance()
        print("cm=",distance)
        
        if distance > limiDist:
            data = client.recv(1024)
            if data == b'F': # move forward
               motor.Motor_Forward()
            elif data == b'B': # move backward
               motor.Motor_Backward()
            elif data == b'L': # move left
               motor.Motor_Left()
            elif data == b'R': # move right
               motor.Motor_Right()
            elif data == b'S': # stop
               motor.Motor_Stop()
        else: 
           motor.Motor_Stop()
        
except KeyboardInterrupt:
    client_socket.close()
    GPIO.cleanup()

finally:
    GPIO.cleanup()