import socket
s=socket.socket()
s.bind(('127.0.0.1',12000))
s.listen(0)
print("Listening")
while True:
    print("opening connection")
    client,addr = s.accept()
    print("connection accepted")
    while True:
        content=client.recv(32)
        if len(content)==0:
            print("no data recieved")
        else:
            print("recieved:")
            print(content)

    print("closing connection")
    client.close
