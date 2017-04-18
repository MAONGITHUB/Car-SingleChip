import socket

servername = '192.168.199.130'
serverport = 1111
buffsize = 2048

clientSocket =socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

while 1:
	sdata = input('Input your lowercase sentence:')
	clientSocket.sendto(sdata.encode(),(servername,serverport))

#rdata,serverAddress = clientSocket.recvfrom(buffsize)
#print (rdata.decode())

#clientSocket.close