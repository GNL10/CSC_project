import os

def optionsMenu():

    optionFlag = True
    while (optionFlag):

        print("\nThis is the Options Menu:\n")
        print("- PRESS 0 to create a security key;")
        print("- PRESS 1 to send a command;")
        print("- PRESS 2 to read responses.")
        print("- PRESS 3 to quit.")
        
        client_option = input("    >> ")

        #Client wishes to create keys
        if client_option == "0":
            print("Your key was created")

        #Client wishes to send commands
        elif client_option == "1":
            commandFlag = True
            while (commandFlag):
                send_command = input("\nInsert command (if you have no more commands, type 'quit')\n    >> ")  

                if send_command == "quit":
                    commandFlag = False
                else: 
                    sendCommand(send_command)
                    print("Command sent!")

        #Client wishes to read command responses
        elif client_option == "2":
            print("\nThese are the responses to your commands:")
            readResponses()
                
        elif client_option == "3":
            optionFlag = False
        else:
            print("Invalid option.\n")

def sendCommand(send_command):
    with open('commands.txt', 'a') as file:
        file.write(send_command + os.linesep)

def readResponses():
    with open('responses.txt', 'r') as file:
        for line in file:
            print("    ",line)

optionsMenu()