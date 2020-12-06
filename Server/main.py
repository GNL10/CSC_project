import sys
sys.path.append(".")
from server import Server

server = Server()

server.searchClients()

server.proccessCommand(client=0)

#server.respondToClient(0, "lsdjdlfkjsd")
