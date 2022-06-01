import socket
print("We're in tcp server...");
server_port = 12000
server_ip = socket.gethostbyname(socket.gethostname())#'172.31.84.206'
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

print(server_socket)
server_socket.bind((server_ip,server_port))
server_socket.settimeout(0.01) #10ms timeout for receives, after which silent error is thrown
server_socket.listen(1) 
print('Server running on port ', server_port)

def parse(recv_msg, socket):
    recipient = msg.split(,)
#incase
def send(socket, send_msg):
    try:
        socket.send(send_msg.encode())
        print(f"sent {send_msg}")
    except:
        pass

def recv(socket):
    received = False
    try:
        recv_msg = socket.recv(1024).decode()
        received = True
    except:
        if recv_msg != "":
            print(f"recevied {recv_msg}")
            parse(recv_msg, socket)
#Now the loop that actually listens from clients
while True:
    connection_socket, caddr = server_socket.accept()
    #notice recv and send instead of recvto and sendto
    cmsg = connection_socket.recv(1024)  	
    cmsg = cmsg.decode()
    print(cmsg, socket)
    
    connection_socket.send(cmsg.encode())

