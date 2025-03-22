import socket

def start_client(host='127.0.0.1', port=8080):
    # Create a socket object
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        # Connect to the server
        client_socket.connect((host, port))
        print(f"Connected to server at {host}:{port}")

        # Send a bytes array to the server
        message = [69, 0, 0, 0]
        client_socket.sendall(bytes(message))
        print(f"Sent: {message}")

        # Receive a response from the server
        data = client_socket.recv(1024)  # Buffer size is 1024 bytes
        print(f"Received: {data}")

if __name__ == "__main__":
    start_client()
