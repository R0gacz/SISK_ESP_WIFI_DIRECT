import socket

UDP_IP = "0.0.0.0" 
UDP_PORT = 5678

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    data, addr = sock.recvfrom(1024)
    print(f"Message from WIFI 2P2 Server {addr}: {data.decode('utf-8')}")