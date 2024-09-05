<h3 align="center">cl-icp-p2p</h3>

  <p align="center">
    Python Server to C++ Dll Client
    <br />
  </p>
</div>

## What is cl-icp-p2p?
![image](https://github.com/user-attachments/assets/e36e7977-2987-4435-a29f-d81a48454ec1)
Yeah idk, i had to ask chatgpt what to call it lmao. But basically its just a python server that hosts from a ip / port, and when the dll client is injected into a process it will connect to the server, and you gain access to a "reverse terminal"* (cmd controlled by you)*

## How to setup cl-icp-p2p.
Well first things first, you need to setup the c++ dll visual studio project for the client. 
Also you need to setup the server.py file, Go ahead and run **get-requirements.bat**.
This will install all requirements you need for server.py. Next you can just run the server.py file however you'd like.

**NOTE:** MAKE SURE THAT YOUR SERVER IP & PORT IS SETUP IN BOTH SERVER AND CLIENT CODE.

""Server listening for connections..."" This is just waiting on the client to connect to the server.
Next you need to inject the client dll into a process. *(example: explorer.exe)*
[Process Hacker](https://processhacker.sourceforge.io/downloads.php) is a really good tool / injector for this project!

