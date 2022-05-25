import socket
import time
import LoadObjects, UploadObjects, deleteObjects, objectsdatabase
#receives message from control
#taking the information out of the database and sending it to the javascript code.js
def parse (msg, socket):
	#global control
	socket.settimeout(0.01)
	#print(f"{caddr[sockets.index(control)]} is the control")
	# upload to scores. 
	#parse the message and then send to relevant sections in the table. 
	x = msg.split(":")
	tmp2 = (x[1]).split("; ") #x[0] = o # print(int(tmp2[0])) = 1 # tmp2[1] = 2 for txt = "o:1; 2"
	coord = [tmp2[0], tmp2[1]]
	objectsdatabase.create_objects_table() # create the table
	UploadObjects.upload_object(int(x[0]), coord)
	#then load them to send to the code.js
	LoadObjects.get_objects()
	#parse and put into format
	#msg_to_send = send this to code.js ntd
	send(game, msg)

		

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
			parse(recv_msg, socket)
	else:
		pass


def send(socket, send_msg):
	try:
		socket.send(send_msg.encode())
		deleteObjects.delete_objects_table() # delete after have sent. 
		print(f"sent {send_msg}")
	except:
		pass

server_port = 3000
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
