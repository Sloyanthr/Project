长度扩展攻击:是指一种针对特定加密散列函数的攻击手段，攻击者可以利用H(M1)和M1的长度，不知道M1内容的情形下，将攻击者控制的M2计算出H（M1 ‖ M2）。

其核心是将H(M1)作为对M2做hash的IV值输入，然后根据已知的M1的长度和M2的长度确定新的填充长度，在进行hash，就能得到伪造的H(M1||M2)的值

![image](https://user-images.githubusercontent.com/104297950/181804366-e58c0001-ff50-4eb9-916a-b9ae8a9f8b92.png)

如图，EXMESS1_MESS2就是我们根据H(MESS1)和MESS1长度伪造hash的对应的信息，我们可以验证：

![image](https://user-images.githubusercontent.com/104297950/181804621-baa2ccb3-1bab-4ae0-8219-d720086b00a7.png)

经验证，根据H(MESS1)和MESS1长度伪造hash与对应信息的哈希值一致，说明我们的攻击成功
