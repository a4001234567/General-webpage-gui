from random import choice
count = 0
for i in range(10000000):
    a = [0,1,2,3,4,5,6,7,8]
    result = []
    for i in range(9):
        result.append(choice(a))
        a.remove(result[i])
    for i in range(3):
        if (result[3*i]//3==result[3*i+1]//3==result[3*i+2]//3):
            count +=1
            break
print(count/10000000)
