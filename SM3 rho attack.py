from time import time
from gmssl import sm3, func

#n为攻击长度，mess为初始信息
def rho(n,mess):
  lis = []
  lis.append(sm3.sm3_hash(func.bytes_to_list(mess))[:n])
  flag = 0
  start = time()
  while(1):
    data = bytes(lis[-1], encoding='utf-8')
    x = sm3.sm3_hash(func.bytes_to_list(data))
    data = bytes(x, encoding='utf-8')
    t_x = sm3.sm3_hash(func.bytes_to_list(data)[:n])
    for j in lis:
        if t_x==j:
            flag=1
            print(lis.index(j),"\n碰撞哈希:\n", j, "\n元素号:\n", len(lis), '\n',t_x)
            break
    if flag==1:
        break
    lis.append(t_x)
  end = time()
  print(end-start,"s")

n = 4
mess = b'helloworld'
rho(n,mess)
