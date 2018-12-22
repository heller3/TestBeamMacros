import socket
import sys
import time

ip_address = "192.168.133.46"
use_socket = 17000
runFileName ="/data-08/TestBeam/Users/RunNumber/OtherRuns0NextRunNumber.txt"

def init_ots():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    MESSAGE = "OtherRuns0,Initialize"
    sock.sendto(MESSAGE, ("192.168.133.46", use_socket))
#    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
 #   print "Initialize: received message:", data
    time.sleep(5)

def config_ots():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#    MESSAGE = "PhysicsRuns0,Configure,FQNETConfig"
    MESSAGE = "OtherRuns0,Configure,T992Config"
    sock.sendto(MESSAGE, ("192.168.133.46", use_socket))
  #  data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
  #  print "Configure: received message:", data
    time.sleep(5)

def start_ots(Delay=False):
    runFile = open(runFileName)
    nextRun = int(runFile.read().strip())
    runFile.close()
    
    incrementRunFile = open(runFileName,"w")
#    print str(nextRun+1)+"\n"
    incrementRunFile.write(str(nextRun+1)+"\n")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    MESSAGE = "OtherRuns0,Start, %d" % (nextRun) 
    print MESSAGE
    sock.sendto(MESSAGE, ("192.168.133.46", use_socket))
    #data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    #print "Start: received message:", data
    if Delay: time.sleep(5)

def stop_ots(Delay=True):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    MESSAGE = "OtherRuns0,Stop"
    sock.sendto(MESSAGE, ("192.168.133.46", use_socket))
 #   data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
#    print "Stop: received message:", data
    if Delay: time.sleep(5)
