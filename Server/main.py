import sys
sys.path.append(".")
from server import Server

server = Server()

server.searchClients()

server.respondToClient(0, "lsdjdlfkjsd")
