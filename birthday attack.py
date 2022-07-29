import random
import itertools
import base64
import binascii
from gmssl import sm3, func
alphatable = "abcdefghijklmnopqrstuvwxyz"

def randomstr(randomlength=8):
  random_str =''
  base_str ='abcdefghigklmnopqrstuvwxyz'
  length = len(base_str) -1
  for i in range(randomlength):
    random_str += base_str[random.randint(0, length)]
  return random_str
#查找并加入,num为攻击次数，length为字符串长度

def attack(num,length = 6):
    hashmap={}
    alphatable = "abcdefghijklmnopqrstuvwxyz"
    i = 0
    for s in itertools.permutations(alphatable,length):
        i+=1
        strs=""
        for k in range(length):
            strs+=s[k]
        data = bytes(strs, encoding='utf-8')
        #切片大小界定碰撞长度
        sign = sm3.sm3_hash(func.bytes_to_list(data))[:7]
        if sign in hashmap:
          print("攻击成功")
          return (hashmap[sign],strs)
        else:
            hashmap[sign]=strs
        if i>=num:
            break
    print("攻击失败")
    return (0, 0)


print(attack(pow(2,16),8))
