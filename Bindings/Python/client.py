import socket
import requests

def main():
    add_request1 = requests.create_request_add_user(0x00,
        "basic",
        "first1",
        "last1",
        "This is a description1")
    add_request2 = requests.create_request_add_user(0x01,
        "hex",
        "first2",
        "last2",
        "This is a description2")
    add_request3 = requests.create_request_add_user(0x02,
        "fancy",
        "first3",
        "last3",
        "This is a description3")
    remove_request = requests.create_request_remove_user(0x01)
    list_request = requests.create_request_list_users()
    get_request = requests.create_request_get_user(0x00)
    display_request = requests.create_request_display_user(0x02)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
    s.connect(('127.0.0.1', 4444))

    #s.send(add_request3)
    s.send(display_request)
    buf = b""
    b = s.recv(0x100)
    while b:
        buf += b
        b = s.recv(0x100)

    print(f'Received: {buf}')

    s.close()

if __name__ == '__main__':
	main()