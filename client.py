import socket
from select import select
from IPython import embed
HOST = "127.0.0.1"
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        while True:
            readable, writable, exceptional = select([conn], [], [], 0.2)
            if not readable:
                print("HERE!1")
                continue
            else:
                sock = readable[0]
                size = sock.recv(20)
                
                
                size = int(size.decode('UTF-8').strip('\x00'))
                data = sock.recv(size)
                print(data)