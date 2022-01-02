from expression import *
if __name__ == "__main__":
    get = tuple(map(lambda x:const(int(x)),input().split()))
    assert 4 == len(get)
def remove(comb,i,j):
    return tuple([comb[k] for k in range(len(comb)) if k not in (i,j)])
def solve(comb):
    if len(comb) == 1 and comb[0].eval() and abs(comb[0].eval()-24)<0.001:
        return comb[0]
    size = len(comb)
    for i in range(size):
        for j in range(size):
            if i == j:continue
            base = remove(comb,i,j)
            for op in (plus,minus,times,divide):
                new_item = expression(op,(comb[i],comb[j]))
                result = solve(base + (new_item,))
                if result:return result
    return None
def solve_int(*ints):
    comb = tuple(map(const,ints))
    solution = solve(comb)
    if solution:
        solution = solution.dfs()
        return(str(solution))
    return "No Solution"
if __name__ == "__main__":
    solution = solve(get)
    if solution:
        solution = solution.dfs()
        print(solution)
    else:
        print("No Solution")
