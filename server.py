#
#   cl-icp-p2p python server
#   made by @lemontoe_ on discord
#   https://github.com/lemontoe
#

import os,sys,time,ctypes,socket,threading
import colorama as clr
clr.init()

# Server Setup
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('0.0.0.0', 9999)) # Server ip and port
server_socket.listen(5)

os.system("cls")
print(f"\n\n   {clr.Style.DIM}Server listening for connections...{clr.Style.NORMAL}")
ctypes.windll.kernel32.SetConsoleTitleA("cl-icp server")

clients = []

def get_current_time(): # Grabbing current time
    current_time = time.strftime("%H:%M:%S", time.localtime())
    return f" {clr.Fore.LIGHTWHITE_EX}{clr.Style.DIM}[ {clr.Style.NORMAL}{clr.Fore.YELLOW}{current_time} {clr.Fore.LIGHTWHITE_EX}{clr.Style.DIM}] {clr.Style.NORMAL}"

def handle_client(client_socket):
    while True:
        try:
            command = input(f"{clr.Fore.WHITE}\n ┌──({clr.Fore.GREEN}server@p2p{clr.Fore.WHITE})-[{clr.Fore.YELLOW}~{clr.Fore.WHITE}] \n └─{clr.Style.DIM}#{clr.Style.NORMAL} ")
            if command.lower() == 'exit':
                client_socket.send('exit'.encode())
                client_socket.close()
                sys.exit(0)
            client_socket.send(command.encode())

            # Loop to receive full response
            response = ""
            while True:
                part = client_socket.recv(1024).decode()
                response += part
                if len(part) < 1024:
                    break

            print(f"\n{get_current_time()}Response from client :\n   {response}")

        except Exception as err:
            print(f"\n\n{get_current_time()}Unexpected Error : {clr.Fore.RED}{clr.Style.DIM}{err}{clr.Style.NORMAL}")
            break

    client_socket.close()

def accept_connections():
    while True:
        client_socket, addr = server_socket.accept()
        print(f"\n{get_current_time()}Client Connected! : {clr.Fore.GREEN}{clr.Style.DIM}{addr}{clr.Style.NORMAL}")
        clients.append(client_socket)
        client_thread = threading.Thread(target=handle_client, args=(client_socket,))
        client_thread.start()

if __name__ == "__main__":
    accept_connections()