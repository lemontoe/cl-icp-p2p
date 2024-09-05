<h3 align="center">cl-icp-p2p</h3>

  <p align="center">
    Python Server to C++ Dll Client
    <br />
  </p>
</div>

## What is cl-icp-p2p?
![image](https://github.com/user-attachments/assets/e36e7977-2987-4435-a29f-d81a48454ec1)
cl-icp-p2p stands for c++ and python *(cross-lang)* inter-process communication *(IPC)* Peer-to-Peer. Or in other words, it's a python server that listens on an ip/port, and a c++ dll client that connects back to the server when injected into a process (e.g., explorer.exe). The server gains access to a reverse terminal, allowing you to execute commands on the client-side.

*Note: didn't know what to call it, so i asked chatgpt lol*

## How to setup cl-icp-p2p.
Step 1 :
  Well first things first, you need to setup the c++ dll visual studio project for the client. 
  Optional : you can set up the injector too for easy injection.
  Run the `get-requirements.bat` file to install all the necessary dependencies for the Python server.
  Next you can just run the server.py file however you'd like.

  **NOTE:** MAKE SURE THAT YOUR SERVER IP & PORT IS SETUP IN BOTH SERVER AND CLIENT CODE. ALSO FIREWALL MAY BLOCK PORTS!!

Step 2 :
  `Server listening for connections...` This is just waiting on the client to connect to the server.
  Next you need to inject the client dll into a process. *(example: explorer.exe)*
  [Process Hacker](https://processhacker.sourceforge.io/downloads.php) is a really good tool / injector for this project!

  Once you injected your client into a process, you should have a linux terminal looking cmd and it should say somewhere "Client Connected!" that means your client is connected to the server host. 
