import pythoncom
import pyHook
import win32api


def onKeyboardEvent(event):# 监听键盘事件
	print ("MessageName:", event.MessageName)
	print ("Message:", event.Message)
	print ("Time:", event.Time)
	print ("Window:", event.Window)
	print ("WindowName:", event.WindowName)
	print ("Ascii:", event.Ascii, chr(event.Ascii))
	print ("Key:", event.Key)
	print ("KeyID:", event.KeyID)
	print ("ScanCode:", event.ScanCode)
	print ("Extended:", event.Extended)
	print ("Injected:", event.Injected)
	print ("Alt", event.Alt)
	print ("Transition", event.Transition)
	print ("---")

	return True# 键盘事件监听函数的返回值


def main():
	keylisten = pyHook.HookManager()        # 创建一个“钩子”管理对象
	keylisten.KeyDown = onKeyboardEvent     # 监听所有键盘事件
	keylisten.HookKeyboard()    # 设置键盘“钩子”
	pythoncom.PumpMessages()       #进入循环，如不手动关闭，程序将一直处于监听状态
	if onKeyboardEvent.Key=='Escape':	#遇到Esc键退出
		#pyHook.HookManager.UnhookKeyboard()#语句有问题 
		win32api.PostQuitMessage()


if __name__ == "__main__":
	main()