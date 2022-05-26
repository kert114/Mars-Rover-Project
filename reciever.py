import socket
import time
#receives message from control
#sending it to the javascript code.js	
#THIS IS THE WEBSERVER.x ON THE EC2 INSTANCE.   
def recv(socket):
	received = False
	try:
		recv_msg = socket.recv(1024).decode()
		received = True
	except: #This signifies disconnect
		#do nothing 
		pass
	if received: #If received, parse
		if recv_msg != "":
			print(f"received {recv_msg}")
	else:
		pass

server_port = 12000
server_ip = socket.gethostbyname(socket.gethostname())#'172.31.84.206' # ip address 
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # if already binded, don't bind again
server_socket.bind((server_ip,server_port))
server_socket.settimeout(0.01) #10ms timeout for receives, after which silent error is thrown
server_socket.listen() 

print('Server running on port ', server_port)

sockets = [None, None, None, None, None]

#Main server loop
while True:
	for i, socket in enumerate(sockets): #send and receive linearly
		if socket != None:
			recv(socket) #receive, and then perhaps send
