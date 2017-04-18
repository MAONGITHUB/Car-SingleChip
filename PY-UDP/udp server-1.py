import socket
serverport = 8080
buffsize = 1024


#Address =('192.168.199.130,'1111)

serverSocket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)		#SOCK_DGRAM表示基数据报格式即UDP连接
serverSocket.bind(('',serverport))		#（host,port）的形式表示地址、引号表示可以接受任何地址来的数据报
print("The server is ready to receive")

while 1:
	message,clientAddress = serverSocket.recvfrom(buffsize)
	#serverSocket.sendto(data,(clientAddress))
	print (message.hex())