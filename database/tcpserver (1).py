import socket
print("We're in tcp server...");

#select a server port
server_port = 12000
#create a UDP socket
welcome_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print(welcome_socket)
#bind the server to the localhost at port server_port
welcome_socket.bind(('0.0.0.0',server_port))

#extra for tcp socket:
welcome_socket.listen(1)

#ready message
print('Server running on port ', server_port)

#Now the loop that actually listens from clients
while True:
    connection_socket, caddr = welcome_socket.accept()
    #notice recv and send instead of recvto and sendto
    cmsg = connection_socket.recv(1024)  	
    cmsg = cmsg.decode()
    print(cmsg)
    connection_socket.send(cmsg.encode())

