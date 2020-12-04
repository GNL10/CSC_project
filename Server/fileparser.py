import os
class FileParser:

    def __init__(self):
        # nothing to do in initialization.... yet :)
        print("defining a file parse")

    @staticmethod
    def open(file_path):
        if not os.path.isfile(file_path): ValueError("submit a valid file path")
        content = {}
        with open(file_path) as fp:
            line = fp.readline()
            cnt = 1
            while line:
                content[cnt] = line.strip()
                line = fp.readline()
                cnt += 1
        return content

    @staticmethod
    def write(file_path, content):
        if not os.path.isfile(file_path): ValueError("submit a valid file path")
        with open(file_path, 'w+') as fp:
            fp.write(content)
        return 1

    @staticmethod
    def parse(cmd):
        comando = cmd.split()
        if not comando: return None, None
        print(comando)
        db_cmd = comando[0]
        db_key = (1,3)
        return db_cmd, db_key