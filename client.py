import socket

def connect_to_server(server_ip, server_port):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        client_socket.connect((server_ip, server_port))

        client_socket.send(b"GET /index.html HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n")
        response = client_socket.recv(4096).decode('utf-8')

        print("Response from server:")
        print(response)

    finally:
        client_socket.close()

if __name__ == "__main__":
    server_ip = "127.0.0.1"  # Change this to your server's IP
    server_port = 4221       # Change this to your server's port

    connect_to_server(server_ip, server_port)

