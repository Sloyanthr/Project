这是基于本人编写的SM3进行的代码优化，主要运用了循环展开和减少函数的调用，在保证代码可读性的基础上进行了最大程度的优化

为了减少传参时间，函数参数选用了数组指针，去掉了一次循环中调用较少的P0和P1并将其直接加入了主体函数

![image](https://user-images.githubusercontent.com/104297950/181803329-6f83ee89-0ce9-4c3e-ae62-6a7639c45051.png)

循环展开主要应用在循环赋值上，一些循环次数较少的循环被直接去掉

![image](https://user-images.githubusercontent.com/104297950/181803077-f41c0852-4e87-4ce7-8e90-a97b4faebef7.png)

一些地方使用了移位等对计算机友好的操作代替乘除模

![image](https://user-images.githubusercontent.com/104297950/181803232-c0c14ffd-90df-49b6-babd-0ae22e545a33.png)

运行方式:全局变量MESS为需要hash的信息，一个元素一个字节，int变量

下图中上面的运行时间为优化前100000次hash的运行时间，下面的运行时间对应优化后100000次hash的运行时间:

![image](https://user-images.githubusercontent.com/104297950/181807495-0805b948-76c9-4a2e-9a9d-a3a1cfd611c1.png)

可以看出优化效果明显
