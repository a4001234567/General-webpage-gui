import socket
import json
import time
import random
host = ''
port = 1027
response_header = 'HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf-8\r\n\r\n'.encode('utf-8')
w = 1024
h =768
commands = []
def parse_response(response_text):
    code = ''
    header = dict()
    content = ''
    for idx,line in enumerate(response_text.split('\r\n')):
        if 0 == idx:
            code = line
            continue
        if line == '':
            content='\n'.join(response_text.split('\r\n')[idx+1:])
            break
        header[line[:line.find(':')]]=line[line.find(':')+1:]
    return code,header,content
def rect(x,y,w,h,color):
    commands.append(['Rect',[x,y,w,h,color]])
def circ(x,y,r,color):
    commands.append(['Circ',[x,y,r,color]])
def clrs():
    commands.append(['CLRS'])
def text(x,y,size,color,content):
    commands.append(["Text",[x,y,size,color,content]])
def alert(msg):
    commands.append(["Alert",[msg]])
def prompt(msg):
    commands.append(["Prompt",[msg]])
def title(msg):
    commands.append(["Title",[msg]])
def timer(ms):
    commands.append(["Timer",[ms]])
v = 1
size = 20
body = []
candy = []
status = 'play'
with socket.socket() as server:
    server.bind((host,port))
    server.settimeout(0.5)
    server.listen()
    while True:
        try:
            cli_soc,cli_addr = server.accept()
        except Exception:
            continue
        time.sleep(.05)
        receive = cli_soc.recv(5000).decode('utf8')
        code,rheader,content = parse_response(receive)
        if 'quit' in code:
            with cli_soc:
                cli_soc.sendall(response_header+b'<h1>QUIT</h1>')
            print("EXIT")
            break
        with cli_soc:
            response = response_header
            if 'GET' in code:
                w = 1024
                h = 768
                status = 'play'
                with open('template.html','r')as file:
                    for line in file:response += line.encode('utf-8')
                print(cli_addr)
            else:
                if content.split(' ')[0] == 'Update':
                    _,nw,nh = content.split(' ')
                    nw = int(nw)
                    nh = int(nh)
                    if 1024 == w and 768 == h:
                        timer(10)
                        w,h = nw,nh
                        body.append((w//(2*size),h//(2*size)))
                    else: w,h = nw,nh
                elif content == 'BOOK':
                    if not 'play' == status:
                        continue
                    timer(50)
                    if random.random()>1-0.5/len(body)+len(candy)/5:
                        pos_x = random.randint(0,(w//size)-2)
                        pos_y = random.randint(0,(h//size)-2)
                        if ((pos_x,pos_y) not in candy):
                            candy.append((pos_x,pos_y))
                    tail = body[-1]
                    body = [body[0]]+body[:-1]
                    if 1 == v:body[0] = (body[0][0]-1,body[0][1])
                    elif 2 == v:body[0] = (body[0][0],body[0][1]-1)
                    elif 3 == v:body[0] = (body[0][0]+1,body[0][1])
                    elif 4 == v:body[0] = (body[0][0],body[0][1]+1)
                    if body[0] == tail or body[0] in body[1:] or body[0][0]<0 or body[0][0]>(w//size-2) or body[0][1]<0 or body[0][1]>(h//size-2):
                        alert("You Lose")
                        status = 'lost'
                        body = []
                        candy = []
                    elif body[0] in candy:
                        clrs()
                        candy.remove(body[0])
                        body.append(tail)
                elif content.split(' ')[0] == 'Key':
                    _,key_code = content.split(' ')
                    if 36<int(key_code)<41:
                        if (int(key_code)-v)%2:v = int(key_code)-36
                    if key_code == '82':
                        status = 'play'
                        timer(10)
                        body.append((w//(2*size),h//(2*size)))
                    if key_code == '80':
                        status = 'paused'
                elif content.split(' ')[0] == 'Click':
                    if status != "play": continue
                    _,x,y = content.split(' ')
                    x = int(x)
                    y = int(y)
                    hx = body[0][0]*size+(w>>1)%size
                    hy = body[0][1]*size+(h>>1)%size
                    x -= hx
                    y -= hy
                    if y > abs(x):v = 4
                    if y < -abs(x): v = 2
                    if x < -abs(y): v = 1
                    if x > abs(y): v = 3
                if 'play' == status:
                    rect(0,0,w,h,'#795548')
                    rect((w>>1)%size,(h>>1)%size,size*(w//size-1),size*(h//size-1),'#689f38')
                    color = ("#767676","#888888")
                    for component in body:
                        rect(component[0]*size+(w>>1)%size,component[1]*size+(h>>1)%size,size,size,color[(component[0]+component[1])%2])
                    for pieces in candy:
                        circ(pieces[0]*size+(w>>1)%size,pieces[1]*size+(h>>1)%size,size>>1,'#e91e63')
                response+=json.dumps(commands).encode('utf-8')
                commands = []
            cli_soc.sendall(response)
