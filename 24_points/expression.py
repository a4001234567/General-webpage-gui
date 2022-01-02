class operator:
    def __init__(self,symbol,inverse,priority,commutativity):
        self.symbol = symbol
        self.inverse = inverse
        self.priority = priority
        self.commutativity = commutativity
    def __repr__(self):
        return self.symbol
num = operator(None,None,0,None)
plus = operator('+',None,2,True)
minus = operator('-',plus,2,False)
plus.inverse = minus
times = operator('*',None,1,True)
divide = operator('/',times,1,False)
times.inverse = divide
operators = (plus,minus,times,divide)
char2op =  {t.__repr__():t for t in operators}
class expression:
    exp_type = num#(num,plus,minus,times,divide)
    data = [None,]
    def __init__(self,input_type,input_data):
        assert input_type in (num,plus,minus,times,divide)
        if type(input_data) == tuple:input_data = list(input_data)
        self.exp_type = input_type
        if input_type == num:
            assert len(input_data) == 1
            assert type(input_data[0]) in (float,int)
        else:
            assert len(input_data) == 2
            assert type(input_data[0]) == expression and type(input_data[1]) == expression
        self.data = input_data
    def eval(self):
        if self.exp_type == num:return self.data[0]
        l_result = self.data[0].eval()
        r_result = self.data[1].eval()
        if l_result == None or r_result == None: return None
        if self.exp_type == plus: return l_result+r_result
        if self.exp_type == minus: return l_result-r_result
        if self.exp_type == times: return l_result*r_result
        if self.exp_type == divide:
            if 0 == r_result: return None
            return l_result/r_result
    def left_rotate(self):
        if num == self.exp_type or self.exp_type.priority != self.data[1].exp_type.priority:
            return self
        temp = self.data[1]
        self.data[1] = temp.data[0]
        temp.data[0] = self
        if not self.exp_type.commutativity:temp.exp_type = temp.exp_type.inverse
        return temp
    def dfs(self):
        if num == self.exp_type: return self
        prev = self
        now = self.left_rotate()
        while prev != now:
            prev = now
            now = prev.left_rotate()
        now.data[0] = now.data[0].dfs()
        now.data[1] = now.data[1].dfs()
        return now
    def __repr__(self):
        if self.exp_type == num:return self.data[0].__repr__()
        else:
            ltype = self.data[0].exp_type
            ctype = self.exp_type
            rtype = self.data[1].exp_type
            lbracket = rbracket = False
            lbracket = ltype.priority > ctype.priority
            rbracket = rtype.priority > ctype.priority or (rtype.priority == ctype.priority and ctype.commutativity == False)
            result = ''
            if lbracket:result += '(' + self.data[0].__repr__() + ')'
            else : result += self.data[0].__repr__()
            result += ctype.__repr__()
            if rbracket:result += '(' + self.data[1].__repr__() + ')'
            else:result += self.data[1].__repr__()
            return result
def const(x):
    return expression(num,(x,))
def create_expression(string):
    if string[0] == '(' and string[-1] == ')':string = string[1:-1]
    cur_pri = 0
    depth = 0
    op_loc = []
    for idx,c in enumerate(string):
        if c == '(' : depth -=1
        elif c == ')' : depth += 1
        elif depth == 0:
            if c in char2op.keys():op_loc.append(idx)
            else: continue
            if c in '+-' : cur_pri = 2
            elif c in '*/' : cur_pri = max(cur_pri,1)
    if cur_pri == 0:
        if '.' in string:return const(float(string))
        return const(int(string))
    op_loc = [idx for idx in op_loc if char2op[string[idx]].priority == cur_pri]
    temp = create_expression(string[:op_loc[0]])
    for idxx in range(len(op_loc)):
        next_exp = create_expression(string[op_loc[idxx]+1:op_loc[idxx+1]if idxx != len(op_loc)-1 else None])
        temp = expression(char2op[string[op_loc[idxx]]],(temp,next_exp))
    return temp
if __name__ == "__main__":
    a = expression(minus,(const(1),const(2)))#a:    1   -   2   =   -1
    b = expression(minus,(const(3),const(4)))#b:    3   -   4   =   -1
    c = expression(minus,(a,b))#c:  -1  -   -1  =   0
    d = expression(minus,(const(5),const(6)))#d:    5   -   6   =   -1
    e = expression(minus,(const(7),const(8)))#e:    7   - *  8   =   -1
    f = expression(minus,(d,e))#f:  -1  -   -1  =   0
    g = expression(times,(c,f))
    #h = create_expression('1+3*5+6*(1+2)')
    #print(h,end = ' = ')
    #print(h.eval())
    while True:
        i = input()
        if i == 'exit':break
        print(create_expression(i).eval())

