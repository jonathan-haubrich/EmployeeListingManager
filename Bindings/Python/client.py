import socket
import requests

SERVER_IP = '127.0.0.1'
SERVER_PORT = 4444

def send_request(request):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
        s.connect((SERVER_IP, SERVER_PORT))

        s.send(request)
        buf = b""
        b = s.recv(0x100)
        while b:
            buf += b
            try:
                b = s.recv(0x100)
            except ConnectionResetError as e:
                break        

        print(f'Received: {buf}')
        s.close()

def main():
    add_request1 = requests.create_request_add_user(0x00,
        "basic",
        "A" * 16,
        "B" * 16,
        "This is a description1")
    add_request2 = requests.create_request_add_user(0x01,
        "hex",
        "C" * 16,
        "D" * 16,
        "This is a description2")
    add_request3 = requests.create_request_add_user(0x02,
        "fancy",
        "E" * 16,
        "F" * 16,
        "This is a description3")
    remove_request = requests.create_request_remove_user(0x00)
    list_request = requests.create_request_list_users()
    get_request = requests.create_request_get_user(0x00)
    display_request = requests.create_request_display_user(0x60)

    # for request in [add_request1, add_request2, add_request3]:
    #     send_request(request)

    send_request(display_request)

if __name__ == '__main__':
	main()