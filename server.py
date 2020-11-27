import psycopg2
DEBUG = False #test
admin_user = "db_admin"
admin_pwd = "db_admin"
db_name = "csc_db"

def create_user (username, password):
    command = "create user %s with encrypted password '%s';"
    run_db_command(admin_user, admin_pwd, command % (username, password))

def run_db_command (username, password, command):
    try:
        connection = psycopg2.connect(user = username, password = password, host = "127.0.0.1", port = "5432", database = db_name)
        cursor = connection.cursor()
        cursor.execute(command)
        connection.commit()        
    except (Exception, psycopg2.Error) as error :
        print ("Error:", error)
    finally:
        #closing database connection.
        if(connection):
            cursor.close()
            connection.close()
            if DEBUG:
                print("PostgreSQL connection is closed")

def query_db_command (username, password, command):
    try:
        connection = psycopg2.connect(user = username, password = password, host = "127.0.0.1", port = "5432", database = db_name)
        cursor = connection.cursor()
        cursor.execute(command)
        record = cursor.fetchall()
    except (Exception, psycopg2.Error) as error :
        print ("Error:", error)
    finally:
        #closing database connection.
        if(connection):
            cursor.close()
            connection.close()
            if DEBUG:
                print("PostgreSQL connection is closed")
            return record

username = 'client1'
password = 'client1'
table_name = 'cl1_table'

create_user (username, password)
run_db_command (username, password, "create table %s (col1 integer, col2 integer);" % table_name)
for i in range(100):
    run_db_command (username, password, "insert into %s values (%d, %d);" % (table_name, i, i))
print(query_db_command (username, password, "select * from cl1_table;"))


'''
    #cursor.execute("select * from test_table;")
    #command = "drop table test_table;"
    data = ("client1", "client1")
    command = "create user %s with encrypted password '%s';"
    command = "create table client1 (col1 integer, col2 integer);"
    command = "alter table client1 owner to client1;"

'''