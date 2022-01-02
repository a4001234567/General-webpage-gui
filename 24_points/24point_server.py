import socket
import json
import time
from point24 import *
host = ''
port = 2333
header = 'HTTP/1.1 200 OK\r\nContent-Type: text/html;charset=utf-8\r\n\r\n'.encode('utf-8')
w = 1024
h =768
nums = [0]
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
def recirc(x,y,w,h,color,border_radius = 10):
    rect(x,y+border_radius,w,h-2*border_radius,color)
    rect(x+border_radius,y,w-2*border_radius,h,color)
    circ(x,y,border_radius,color)
    circ(x+w-2*border_radius,y,border_radius,color)
    circ(x,y+h-2*border_radius,border_radius,color)
    circ(x+w-2*border_radius,y+h-2*border_radius,border_radius,color)

with socket.socket(socket.AF_INET6) as server:
    server.bind((host,port,0,0))
    server.listen()
    while True:
        try:
            cli_soc,cli_addr = server.accept()
            cli_soc.setblocking(0)
            time.sleep(.05)
            receive = cli_soc.recv(5000).decode('utf8')
            code,rheader,content = parse_response(receive)
        except Exception:continue
        if 'quit' in code:
            with cli_soc:
                cli_soc.sendall(header+b'<h1>QUIT</h1>')
            print("EXIT")
            break
        with cli_soc:
            response = header
            if 'GET' in code:
                with open('template.html','r')as file:
                    for line in file:response += line.encode('utf-8')
            else:
                if 'Click' in content:
                    _,x,y=content.split(' ')
                    rect(x,y,10,10,'red')
                if 'Key' in content:
                    _,keycode =  content.split(' ')
                    keycode = int(keycode)
                    if 48 <= keycode <= 57:
                        nums[-1] = 10*nums[-1]+keycode-48
                    elif keycode == 32:
                        nums.append(0)
                    else:
                        clrs()
                        res = solve_int(*nums)
                        alert(res)
                        rect(0,0,w,h,"#DA9F39")
                        nums = [0]
                if 'Update' in content:
                    _,w,h = content.split(' ')
                    w = int(w)
                    h = int(h)
                if len(nums)>1 or nums[0]:
                    clrs()
                    recirc(0,0,w,h,"#DA9F39");
                    for i,j in enumerate(nums):
                        recirc(w/len(nums)*(i+.5)-10,h/2,20+len(str(j))*25,60,'green')
                        text(w/len(nums)*(.5+i),h/2,50,'#4BC7D8',''if (j==0 and i+1==len(nums))  else str(j))
                else:recirc(0,0,w,h,"#DA9F39");
                response+=json.dumps(commands).encode('utf-8')
                print(commands)
                commands = []
            cli_soc.sendall(response)
