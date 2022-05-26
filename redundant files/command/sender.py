import socket
import math, numpy as np, time, random

# THIS IS THE COMMAND on the local This is going to send the data to the server, then receive data back 
# #from the server and store that in a python.txt file.  
server_name = '172.31.17.101' 
server_port = 12000
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.settimeout(0.01) #10ms timeout for receives, after which silent error is thrown
connection = False
send_msg = "None"
send_msg_prev = "None"
msg  = "{o;x: 10,y: 20}" #this format needs to be done in the control - the formatting of the message.
print(msg)
# for now, it's a short cut. 
def network ():
	global recv_msg, send_msg, send_msg_prev, connection
	print("here")
	print(connection)
	if connection == False:
		try:
			try: server_socket.connect((server_name, server_port))
                               # print("trying to connect to server...")
			except:
				pass
			server_socket.send(msg.encode()) #Identifies which client is game
			print("Connected to receiver")
			connection = True
		except:
			pass
		
		if send_msg != send_msg_prev: #Check whether to send avoid duplicates
			try:
				server_socket.send(send_msg.encode())
				print(f"sent {send_msg}")
				send_msg = "aaaaaaaaaaaaaaaaaa"
			except:
				pass
		send_msg_prev = send_msg

running = True
if __name__ == "__main__":
	infrequent = 0
	while running: 
		if infrequent%5 == 0: #Make networking infrequent to reduce lag
			network()
		infrequent += 1
