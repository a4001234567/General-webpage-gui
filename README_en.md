# INTRODUCTION TO THIS PROJECT
## Aims and structure
Our goal is to develop a gui display that can use webpages in browswer as the user-interface. And thus we divide a complete app into three components.
1. The html file(template.html) contains javascript code that enables interactions with user and communication with the server. It would draw at the command of the server.
2. The server(which was written in python) use sockets to process web request. It first transmitted the webpage in reponse to the GET request, then forward any event from the user to the main program and then send back the drawing command.
3. The main program contains the basic logic of the application, it receives user's action and react base on that.
## Process
First the server sends a GET request to the server, and the server responds with template.html, on receiving it, the browser run the javascript code and it would send an Update event via POST request, marking a new user's entrance.

Then everytime the user click(Click), press key(Key), or change the window's size(Update), or when you have scheduled a BOOK the browswer would send a POST request asking for what to do next.
## Format and meaning of the events
|event format|meaning|example|
|:-------------------------------:|:-------:|:---:|
|Update W H|Screen size become W×H|Update 1024 768|
|Click X Y|A mouse click at (X，Y)|Click 500 500|
|Key key|Pressed a key with its code beingkey|Key 56|
|BOOK|You scheduled the app for a BOOK request|BOOK|
|Prompt msg|User input msg during prompt|Prompt Hello{SPACE}World|
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
### 绘图部分的更多细节
![](assets/16346552240681.jpg)
### 文本信息格式
由于绘图指令以空格分割参数，以换行分割指令，因此绘图指令中的文本内容里要用 *{SPACE}* 代替空格，用 *{ENTER}* 代替换行，用 *{TAB}* 代替制表符。
## 更多内容
此项目的主要目的是为了实现一类**基于事件**的程序，所谓基于事件，是指程序图形界面的刷新只是在用户执行了操作时。像常规的五子棋、中国象棋等棋牌类游戏，以及一些算法的图形化演示等等，在不涉及动画效果时，可以用*基于事件*的程序来实现。因此我们的实现中在用户执行操作时，浏览器才发送POST请求，告知服务器端用户操作并接收图形界面刷新的指令。

然而为了实现更多功能，我们也引入了Timer指令与BOOK事件，由服务器端预定图形界面更新时间，从而也增加了对更加复杂的实时交互功能以及动画效果的支持。
