这是基于本人编写的SM3进行的代码优化，主要运用了循环展开和减少函数的调用，在保证代码可读性的基础上进行了最大程度的优化

为了减少传参时间，函数参数选用了数组指针，去掉了一次循环中调用较少的P0和P1并将其直接加入了主体函数

![image](https://user-images.githubusercontent.com/104297950/181803329-6f83ee89-0ce9-4c3e-ae62-6a7639c45051.png)

循环展开主要应用在循环赋值上，一些循环次数较少的循环被直接去掉

![image](https://user-images.githubusercontent.com/104297950/181803077-f41c0852-4e87-4ce7-8e90-a97b4faebef7.png)

一些地方使用了移位等对计算机友好的操作代替乘除模

![image](https://user-images.githubusercontent.com/104297950/181803232-c0c14ffd-90df-49b6-babd-0ae22e545a33.png)

