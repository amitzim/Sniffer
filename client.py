import socket
from select import select
from IPython import embed
HOST = "127.0.0.1"
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    conn, addr = s.accept()
    data = b""
    with conn:
        print('Connected by', addr)
        while True:
            readable, writable, exceptional = select([conn], [], [], 0.2)
            if not readable:
                continue
            else:
                sock = readable[0]
                size = sock.recv(20)
                print("size: " + size.decode('utf-8'))
                try:
                    size = int(size.decode('UTF-8').strip('\x00'))
                    print("size of recieved meesage: " + str(size))
                except UnicodeDecodeError as e:
                    print(e)
                    continue
                
                sock.send(b"OK")
                counter = 0
                while len(data) < size:
                    counter+=1
                    print(counter)
                    chunck = sock.recv(128)
                    #print(chunck.decode('utf-8'))
                    #data += chunck.decode('utf-8')
                    data += chunck
                    #print("Data now: " + data)
                print(data)
            break
