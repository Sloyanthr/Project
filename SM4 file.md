这是改进版的SM4代码，并且可以用于文件加密

运行方式：

将代码粘贴到一个可运行的项目中，直接运行即可

![image](https://user-images.githubusercontent.com/104297950/181809645-f743294d-9004-4498-9922-0f3ed7bae2b6.png)

filename为加解密源文件，即加密时的明文文件，解密时的密文文件

wfilename是加解密写入的文件名，是加密时的密文文件，解密时的明文文件

MODE用于切换加密模式和解密模式，ENCRYPT为加密，DECRYPT为解密

演示：

我们尝试加密一个mp4文件

![image](https://user-images.githubusercontent.com/104297950/181810438-843d6b50-34cf-4bd8-b3e3-438cfcb9e540.png)

![image](https://user-images.githubusercontent.com/104297950/181810537-cab1af1b-0e0a-4198-81eb-28923b446a5d.png)

mp4文件可以正常打开

运行完后文件夹中多了一个密文文件

![image](https://user-images.githubusercontent.com/104297950/181810700-53e1be19-140d-4338-9c71-4f193642e13a.png)

我们再尝试将其解密

![image](https://user-images.githubusercontent.com/104297950/181810818-d2a6026c-d9e3-45e6-b10f-3ca10ba22072.png)

文件夹中出现了解密后的文件

![image](https://user-images.githubusercontent.com/104297950/181810857-79c55bb0-1d1a-40f0-accb-7fc60440f82a.png)

可以正常打开

![image](https://user-images.githubusercontent.com/104297950/181810939-2bd40077-1dc4-4674-8185-3455f13c5cdb.png)
