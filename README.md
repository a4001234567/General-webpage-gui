# C++大作业介绍
## 主体结构
我们的任务是以C++程序作为主程序，用Python程序作为中转，将浏览器中的网页作为程序的输出界面。结构如下图：
![infrastructure](https://cdn.luogu.com.cn/upload/image_hosting/te8c81n1.png)
C++程序与python程序运行中通过pipe实现输入、输出互联，然后用户端在浏览器中输入网址进行连接，Python程序利用socket库进行响应，返回初始网页，其中包含了处理事件与画图、信息交流的全部javascript代码。
## 事件流程
浏览器打开网页后javascript代码开始执行，最先发送Update指令给服务器端，所有指令通过post请求发送给服务器，经过python程序从标准输入转发给C++程序，C++程序接受后从标准输出发送新的绘图指令给Python程序，之后通过对post请求的回应发送回浏览器，javascript程序通过对其进行处理开始进行画图。
### 对于C++程序
首先获得一条Update指令，输出程序的初始界面，之后基于输入的每一个事件（每次只有一个事件），发出新的绘图指令，以响应用户的操作。每次输出的绘图指令最后一条必须是DONE指令以标志这次的绘图结束了。
## 输入事件的格式与含义
|事件|含义|实例|
|:-------------------------------:|:-------:|:---:|
|Update W H|屏幕宽度变为Ｗ×H，其中W是宽，H是高|Update 1024 768|
|Click X Y|在（X，Y）的位置用户进行了单击|Click 500 500|
|Key key|用户点击了编号为key的按键|Key 56|
|BOOK|你在之前预定了此时进行交流|BOOK|
|Prompt msg|用户在输入框输入了信息msg|Prompt Hello{SPACE}World|
## 绘图指令的格式与含义
|指令|含义|实例|
|:-:|:-:|:-:|
|Rect X Y W H color|在（X，Y）位置绘制尺寸为W×H的矩形，颜色为color|Rect 0 0 1024 768 red|
|Circ X Y R color|在（X，Y）为圆外接正方形的左上角绘制半径为R的圆，颜色为color|Circ 5 5 5 blue|
|CLRS|将屏幕上所有图形清空|CLRS|
|Text X Y size color content|在（X，Y）为外接矩形左上角，字高度为size，颜色为color打出content|Text 0 0 50 #0000ff Hello{TAB}World|
|Tri X Y L R H deg color|以（X，Y）为外接矩形左上角绘制颜色为color的三角形|Tri 30 30 10 10 20 0 blue|
|Timer t|在t毫秒后发送BOOK事件，以期在t毫秒后更新屏幕内容|Timer 5|
|Alert msg|发送内容为msg的弹窗提醒|Alert Bow{SPACE}Tie{SPACE}Are{SPACE}COOL|
|Title title|将网页的名称设为title|Title Best{SPACE}Game{SPACE}Ever|
|Prompt msg|弹出输入框，输入的提示信息为msg|Prompt 输入你的择偶标准|
|DONE|告知本批绘图指令完毕|DONE|
## 部分内容的详细解释
### 坐标系统
坐标一般用（X，Ｙ）表示，X是点距左边框的距离，Y是点距右边框的距离。
### 按键编号
按键编号按照ASCII码，a是65，0是48，空格与回车等控制字符亦是根据ASCII码。
### 绘图部分的更多细节!
[detail about drawing](https://cdn.luogu.com.cn/upload/image_hosting/8jngtdf6.png)
### 文本信息格式
由于绘图指令以空格分割参数，以换行分割指令，因此绘图指令中的文本内容里要用 *{SPACE}* 代替空格，用 *{ENTER}* 代替换行，用 *{TAB}* 代替制表符。
## 更多内容
此项目的主要目的是为了实现一类**基于事件**的程序，所谓基于事件，是指程序图形界面的刷新只是在用户执行了操作时。像常规的五子棋、中国象棋等棋牌类游戏，以及一些算法的图形化演示等等，在不涉及动画效果时，可以用*基于事件*的程序来实现。因此我们的实现中在用户执行操作时，浏览器才发送POST请求，告知服务器端用户操作并接收图形界面刷新的指令。

然而为了实现更多功能，我们也引入了Timer指令与BOOK事件，由服务器端预定图形界面更新时间，从而也增加了对更加复杂的实时交互功能以及动画效果的支持。
