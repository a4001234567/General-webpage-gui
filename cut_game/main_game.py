A_WIN = 1
B_WIN = 2
A_PLAY = -1
B_PLAY = -2
A_OWNED = 1
B_OWNED = 2
RED = 1
BLUE = 2
BLACK = 3
infty = 114514

class cut_games:
    def _get_x(self,num):
        return num%(2*self._m+3)

    def _get_y(self,num):
        return num//(2*self._m+3)

    def _get_num(self,x,y):
        return y*(2*self._m+3)+x

    def _is_legal(self,x,y):
        return 0 <= x <= (self._m+1)<<1 and 0 <= y <= (self._n+1)<<1

    def _find_root(self,num):
        cur = num;
        while self._nodes[self._get_x(cur)][self._get_y(cur)] > 0:
            cur = self._nodes[self._get_x(cur)][self._get_y(cur)]
        root = cur
        cur = num
        while self._nodes[self._get_x(cur)][self._get_y(cur)] > 0:
            temp = self._nodes[self._get_x(cur)][self._get_y(cur)]
            self._nodes[self._get_x(cur)][self._get_y(cur)] = root
            cur = temp
        return root

    def _merge(self,num1,num2):
        r1 = self._find_root(num1)
        r2 = self._find_root(num2)
        if r1 == r2 :return
        x1 = self._get_x(r1)
        x2 = self._get_x(r2)
        y1 = self._get_y(r1)
        y2 = self._get_y(r2)
        if self._nodes[x1][y1] <= self._nodes[x2][y2]:
            self._nodes[x1][y1] += self._nodes[x2][y2]
            self._nodes[x2][y2] = r1
        else:
            self._nodes[x2][y2] += self._nodes[x1][y1]
            self._nodes[x1][y1] = r2

    def _check(self,x,y):
        if (x+y)%2 or self._status > 0 or x <= 0 or y <= 0 or x >= (self._m+1)<<1 or y >= (self._n+1)<<1:
            return -1#when it is out of range or not an edge or there's no need for moves(someone has won)
        if self._nodes[x][y]:
            return -1#the edge is already taken
        if (self._status == A_PLAY and x%2 == 1) or (self._status == B_PLAY and x%2 == 0):
            self._merge(self._get_num(x-1,y),self._get_num(x+1,y))
        else: self._merge(self._get_num(x,y-1),self._get_num(x,y+1))
        self._nodes[x][y] = (A_OWNED if self._status == A_PLAY else B_OWNED)
        if self._status == A_PLAY:
            self._status = B_PLAY
            if self._find_root(self._get_num(0,1)) == self._find_root(self._get_num((self._m+1)<<1,1)):#A WON!
                self._status = A_WIN
                return 1
        else:
            self._status = A_PLAY
            if self._find_root(self._get_num(1,0)) == self._find_root(self._get_num(1,(self._n+1)<<1)):
                self._status = B_WIN
                return 1
        return 0

    def _route(self):
        result = []
        if self._status != A_WIN and self._status != B_WIN:
            return result
        if self._status == A_WIN:
            for x in range(len(self._nodes)):
                for y in range((x+1)%2,len(self._nodes[x]),2):
                    self._nodes[x][y] = infty if x else 0
            is_update = True
            while is_update:
                is_update = False
                for x in range(len(self._nodes)):
                    for y in range((x+1)%2,len(self._nodes[x]),2):
                        for nx,ny in ((x+2,y),(x,y+2),(x,y-2),(x-2,y)):
                            if self._is_legal(nx,ny) and self._nodes[(nx+x)>>1][(ny+y)>>1] == self._status:
                                if self._nodes[nx][ny] > self._nodes[x][y] + 1:
                                    self._nodes[nx][ny] = self._nodes[x][y] + 1
                                    self._anc[self._get_num(nx,ny)] = self._get_num(x,y)
                                    is_update = True
            x = len(self._nodes)-1
            min_dis = infty
            for y in range(1,len(self._nodes[x]),2):
                min_dis = min(min_dis,self._nodes[x][y])
            for y in range(1,len(self._nodes[x]),2):
                if self._nodes[x][y] == min_dis:
                    result.append(self._get_num(x,y))
                    while self._get_x(result[-1]):
                        result.append(self._anc[result[-1]])
                    return result
        else:
            for y in range(len(self._nodes[0])):
                for x in range((y+1)%2,len(self._nodes),2):
                    self._nodes[x][y] = infty if y else 0
            is_update = True
            while is_update:
                is_update = False
                for y in range(len(self._nodes[0])):
                    for x in range((y+1)%2,len(self._nodes),2):
                        for nx,ny in ((x+2,y),(x,y+2),(x,y-2),(x-2,y)):
                            if self._is_legal(nx,ny) and self._nodes[(nx+x)>>1][(ny+y)>>1] == self._status:
                                if self._nodes[nx][ny] > self._nodes[x][y] + 1:
                                    self._nodes[nx][ny] = self._nodes[x][y] + 1
                                    self._anc[self._get_num(nx,ny)] = self._get_num(x,y)
                                    is_update = True
            y = len(self._nodes[0])-1
            min_dis = infty
            for x in range(1,len(self._nodes),2):
                min_dis = min(min_dis,self._nodes[x][y])
            for x in range(1,len(self._nodes),2):
                if self._nodes[x][y] == min_dis:
                    result.append(self._get_num(x,y))
                    while self._get_y(result[-1]):
                        result.append(self._anc[result[-1]])
                    return result

    def _rect(self,x,y,w,h,color):
        real_x = x + self._offset_x
        real_y = y + self._offset_y
        print("Rect %d %d %d %d "%(real_x,real_y,w,h),end = '')
        if RED == color:print("red")
        elif BLUE == color:print("blue")
        elif BLACK == color:print("black")

    def _dot(self,x,y,r,color):
        real_x = x * self._basic_length - r + self._offset_x
        real_y = y * self._basic_length - r + self._offset_y
        print("Circ %d %d %d "%(real_x,real_y,r),end = '')
        if RED == color:print("red")
        elif BLUE == color:print("blue")
        elif BLACK == color:print("black")

    def _draw_board(self):
        for x in range(len(self._nodes)):
            for y in range((x+1)%2,len(self._nodes[x]),2):
                if x%2:
                    self._dot(x,y,self._basic_length//10,BLUE)
                else:
                    self._dot(x,y,self._basic_length//10,RED)
		
    def _draw_connection(self,x,y,stroke = -1):
        if -1 == stroke:stroke = self._basic_length//20
        if (x+y)%2:return
        node_status = self._nodes[x][y]
        if (node_status == A_OWNED and x%2 == 1) or (node_status == B_OWNED and x%2 == 0):
            self._rect((x-1)*self._basic_length,y*self._basic_length-stroke,self._basic_length<<1,stroke<<1,RED if node_status == A_OWNED else BLUE)
        elif (node_status == B_OWNED and x%2 == 1) or (node_status == A_OWNED and x%2 == 0):
            self._rect(x*self._basic_length-stroke,(y-1)*self._basic_length,stroke<<1,self._basic_length<<1,BLUE if node_status == B_OWNED else RED)

    def _draw_line(self):
        for x in range(len(self._nodes)):
            for y in range(x%2,len(self._nodes[x]),2):
                self._draw_connection(x,y)
		
    def _draw_route(self):
        if self._status in (A_WIN,B_WIN):
            path = self._route()
            for i in range(0,len(path)-1):
                self._draw_connection((self._get_x(path[i])+self._get_x(path[i+1]))>>1,(self._get_y(path[i])+self._get_y(path[i+1]))>>1,self._basic_length//10)
            return path

    def _draw_game(self):
        self._draw_line()
        if self._status in (A_WIN,B_WIN):
            self._draw_route()
        self._draw_board()

    def __init__(self,n,m):
        self._nodes = [[0]*(2*n+3) for i in range(2*m+3)]
        self._anc = [-1]*(2*m+3)*(2*n+3)
        self._n = n
        self._m = m
        self._status = A_PLAY
        self._offset_x = 0
        self._offset_y = 0
        self._basic_length = 20
        for x in range(len(self._nodes)):
            for y in range(len(self._nodes[x])):
                if (x+y)%2:self._nodes[x][y] = -1
        for y in range(3,2*n+3,2):
            self._merge(self._get_num(0,y-2),self._get_num(0,y))
            self._nodes[0][y-1] = A_OWNED
            self._merge(self._get_num(2*m+2,y-2),self._get_num(2*m+2,y));
            self._nodes[2*m+2][y-1] = A_OWNED
        for x in range(3,2*m+3,2):
            self._merge(self._get_num(x-2,0),self._get_num(x,0))
            self._nodes[x-1][0] = B_OWNED
            self._merge(self._get_num(x-2,2*n+2),self._get_num(x,2*n+2));
            self._nodes[x-1][2*n+2] = B_OWNED

    def update_screen_size(self,new_width,new_height):
        canvas_width = self._basic_length*(2*self._m+2)
        canvas_height = self._basic_length*(2*self._n+2)
        self._offset_x = (new_width-canvas_width)>>1
        self._offset_y = (new_height-canvas_width)>>1
        self._draw_game();	

    def click(self,x,y):
        x -= self._offset_x
        y -= self._offset_y
        a = (x+y+self._basic_length)//(self._basic_length<<1)
        b = (x-y+self._basic_length)//(self._basic_length<<1)
        r = self._check(a+b,a-b)
        if -1 == r:return -1
        if self._status == A_WIN:print("Alert A{SPACE}WIN!")
        if self._status == B_WIN:print("Alert B{SPACE}WIN!")
        self._draw_connection(a+b,a-b)
        if 1 == r: self._draw_route()
        return r
	
    def draw(self):
        self._draw_game()
"""while (true){
		cin>>command;
		if ("EXIT" == command) return 0;
		if ("Update" == command){
			CLRS();
			scanf("%d %d",&update_w,&update_h);
			a.update_screen_size(update_w,update_h);
		}
		if ("Click" == command){
			int pos_x,pos_y;
			scanf("%d %d",&pos_x,&pos_y);
			a.click(pos_x,pos_y);
		}
		if ("BOOK" == command){}
		if ("Prompt" == command){
			string trash;cin>>trash;
		}
		if ("Key" == command){
			int key_code;scanf("%d",&key_code);
			if (key_code == 80){
				a = cut_games(cur_n,cur_n+1);
				CLRS();
				a.update_screen_size(update_w,update_h);
				puts("Alert New{SPACE}Game");
			}
		}
		puts("DONE");
	}
	return 0;
}"""
