import socket
import json
import time
def log(text):
    with open('forward.log','a')as file:
        file.write(time.strftime("%H:%M:%S\n"))
        file.write(text)
        file.write('\n')
host = ''
port = 1027
header = 'HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf-8\r\n\r\n'.encode('utf-8')
def parse_response(response_text):
    response_text = response_text.split('\r\n')
    header = dict()
    content = ''
    for idx,line in enumerate(response_text):
        if 0 == idx:
            method,path,protocol = line.split(' ')
        elif '' == line:
            content='\n'.join(response_text[idx+1:])
            break
        else: 
            header[line[:line.find(':')]]=line[line.find(':')+1:]
    return (method,path,protocol),header,content
with socket.socket() as server:
    server.bind((host,port))
    server.listen()
    while True:
        log("ready to receive")
        cli_soc,cli_addr = server.accept()
        log(str(cli_addr))
        response = header
        time.sleep(.1)
        log("receiving")
        try:
            cli_soc.setblocking(0)
            receive = cli_soc.recv(3000).decode('utf8')
        except Exception as e:
            log(str(e))
            continue
        log(receive+'\n')
        head,rheader,event = parse_response(receive)
        log("received")
        if head[0] == 'GET':
            if '/quit' == head[1]:
                with cli_soc:
                    cli_soc.sendall(header+b'<h1>QUIT</h1>')
                print('EXIT')
                log('EXIT')
                break
            if '/' == head[1]:
                with open('template.html','r') as file:
                    for line in file:
                        response += line.encode('utf-8')
                with cli_soc:
                    cli_soc.sendall(response)
        if head[0] == 'POST':
            log(event)
            print(event)
            with cli_soc:
                commands = []
                while True:
                    command = input().split()
                    command[-1] = command[-1].replace('{SPACE}',' ').replace('{ENTER}','\n')
                    if 'DONE' == command[0]:
                        break
                    commands.append([command[0],command[1:]])
                response+=json.dumps(commands).encode('utf-8')
                cli_soc.sendall(response)
        log("sent")
