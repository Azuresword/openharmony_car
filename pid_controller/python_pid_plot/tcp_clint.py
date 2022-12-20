import socket
import time


def TestServer():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(("localhost", 8888))
    server.listen(5)
    connection, address = server.accept()
    print(connection, address)
    while True:
        recv_str = connection.recv(1024)
        recv_str = recv_str.decode("ascii")
        if not recv_str:
            break
        print("receive:{}".format(recv_str))

        send_str = "I'm fine, thx!"
        connection.send(bytes(send_str, encoding="ascii"))
        print("send:   {}".format(send_str))
        time.sleep(0.5)

    connection.close()
    server.close()
    print("client end, exit!")
    exit()


if __name__ == '__main__':
    TestServer()
