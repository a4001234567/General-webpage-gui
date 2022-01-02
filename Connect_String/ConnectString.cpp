#include <iostream>
#include <string>
using namespace std;

//绝对值函数(求一个整数的绝对值)
inline int abs1(int x) {
	if (x < 0) { return -x; }
	else { return x; }
}
int W, H; 
string cs="green",cd="#f0f0f0";//系统颜色，默认绿色和灰色
//矩形边框函数(绘制以(x,y)为左上角，长w宽h，边框颜色为c1，厚度为cu,填充色为c2的矩形)
int cu = 10;
void Rect_e(int x, int y, int w, int h, string c1, string c2)
{
	cout << "Rect " << x << ' ' << y << ' ' << w << ' ' << h << ' ' << c1 << endl;
	cout << "Rect " << x + cu << ' ' << y + cu << ' ' << w - 2 * cu << ' ' << h - 2 * cu << ' ' << c2 << endl;
	return;
}
//文字函数(在矩形(x,y,w,h)中心显示文字words,大小为size,颜色为c)
void text(int x,int y,int w,int h,string words, int size, string c) {
	int len = words.length() * size / 2;
	cout << "Text " << x+(w - len) / 2 << ' ' << y+(h - size) / 2 << ' ' << size << ' ' << c <<' '<< words << endl;
}
//圆角矩形函数(以(x,y)为左上角，绘制长为w宽为h的的圆边矩形,外层颜色为c1,内层颜色为c2)
int r0 = 30;//曲率半径，默认30
void Recirc(int x, int y, int w, int h, string c1, string c2) {
	cout << "Circ " << x << ' ' << y << ' ' << r0 << ' ' << c1 << endl;
	cout << "Circ " << x + cu << ' ' << y + cu << ' ' << r0 - cu << ' ' << c2 << endl;

	cout << "Circ " << x + w - 2 * r0 << ' ' << y << ' ' << r0 << ' ' << c1 << endl;
	cout << "Circ " << x + w - 2 * r0 + cu << ' ' << y + cu << ' ' << r0 - cu << ' ' << c2 << endl;

	cout << "Circ " << x << ' ' << y + h - 2 * r0 << ' ' << r0 << ' ' << c1 << endl;
	cout << "Circ " << x + cu << ' ' << y + h - 2 * r0 + cu << ' ' << r0 - cu << ' ' << c2 << endl;

	cout << "Circ " << x + w - 2 * r0 << ' ' << y + h - 2 * r0 << ' ' << r0 << ' ' << c1 << endl;
	cout << "Circ " << x + w - 2 * r0 + cu << ' ' << y + h - 2 * r0 + cu << ' ' << r0 - cu << ' ' << c2 << endl;


	cout << "Rect " << x + r0 << ' ' << y << ' ' << w - 2 * r0 << ' ' << h << ' ' << c1 << endl;
	cout << "Rect " << x + r0 << ' ' << y + cu << ' ' << w - 2 * r0 << ' ' << h - 2 * cu << ' ' << c2 << endl;

	cout << "Rect " << x << ' ' << y + r0 << ' ' << w << ' ' << h - 2 * r0 << ' ' << c1 << endl;
	cout << "Rect " << x + cu << ' ' << y + r0 << ' ' << w - 2 * cu << ' ' << h - 2 * r0 << ' ' << c2 << endl;
	return;
}
//圆角矩形+内容
void Rext(int x, int y, int w, int h, string c1, string words,  int size) {
	Recirc(x, y, w, h, c1, cd);
	text(x, y, w, h, words, size, "black");
}

//弹窗函数(发送一个居中的弹窗，内容为words)
void screen(string words,int size) {
	int x = W / 4, y = H / 3;
	Rect_e(x, y, 2*x, y, cs , "#f0f0f0");
	int len = words.length() * size/2;
	cout << "Text " << (W - len) / 2 << ' ' << (H - size) / 2 << ' ' << size << ' ' << "black " << words << endl;
}
//制表函数(居中做a行b列,每个格边长为w,颜色为c的方格表,并将每个方格的左上角坐标储存到数组p中,并记录边和点的使用情况)
int p[2][64][64]; int w0, a0, b0; string c0;//记录方格表的信息
string cb="white";//背景颜色，默认白色
bool u[64][64], v[64][64]; int h[64][64];//记录边和点使用状况
void mk_form(int a, int b, int w, string c)
{
	int x, y;
	x = (W - b * w) / 2;
	y = (H - a * w) / 2;
	w0 = w; a0 = a; b0 = b; c0 = c;
	h[0][0] = 1; h[a][b] = 1;
	int x1 = x, y1 = y;
	for (int i = 0; i <= a; i++)
	{
		x1 = x;
		for (int j = 0; j <= b; j++)
		{
			if (i <= a - 1 && j <= b - 1) { Rect_e(x1, y1, w, w, c, cb); }
			p[0][i][j] = x1;
			p[1][i][j] = y1;
			x1 += w - cu;
		}
		y1 += w - cu;
	}
	return;
}
//格点选择函数(确定点(x,y)在哪个格点附近,将结果储存到(a,b)中)
bool check(int x, int y, int& a, int& b)
{
	bool B = false;
	for (int i = 0; i <= a0; i++)
	{
		if (((y - p[1][i][0]) <= (10 + cu)) && ((y - p[1][i][0]) >= -10))
		{
			for (int j = 0; j <= b0; j++)
			{
				if (((x - p[0][0][j]) <= (10 + cu)) && ((x - p[0][0][j]) >= -10))
				{
					a = i; b = j;
					B = true; break;
				}
			}
		}
	}return B;
}
//格点近似函数(如果点(x,y)在某个格点附近，将(x,y)调整为这个格点的坐标)
bool apx(int& x, int& y)
{
	int a1, b1;
	bool B = check(x, y, a1, b1);
	if (B == true) { x = p[0][a1][b1]; y = p[1][a1][b1]; }
	return B;
}
//连线函数(判断点(x1,y1)点(x2,y2)的连线是否合法，若不合法，但是该两点已经连接，取消它们之间的连接;若合法，将两点用颜色为c的线连接)
void connect(int x1, int y1, int x2, int y2, string c)
{
	int x0 = x1, y0 = y1, a, b;
	if ((y1 == y2) && (x1 != x2)) {
		if (x1 > x2) { x0 = x2; }
		check(x0, y0, a, b);
		int temp = 0;
		bool Temp = true;
		for (int i = 1; i < abs1(x1 - x2) / (w0 - cu); i++) { temp = temp + h[a][b + i]; }//用temp记录中间点的使用情况
		for (int i = 0; i < abs1(x1 - x2) / (w0 - cu); i++) { Temp = (Temp && u[a][b + i]); }//用Temp记录中间线的使用情况
		if (Temp) {
			cout << "Rect " << x0 << ' ' << y0 << ' ' << abs1(x1 - x2) + cu << ' ' << cu << ' ' << c0 << endl;
			for (int i = 0; i < abs1(x1 - x2) / (w0 - cu); i++) { u[a][b + i] = false; }
			for (int i = 1; i < (abs1(x1 - x2)) / (w0 - cu); i++) { h[a][b + i] -= 2; }
			h[a][b]--; h[a][b + abs1(x1 - x2) / (w0 - cu)]--;
			if (h[a][b] >= 1) { cout << "Rect " << x0 << ' ' << y0 << ' ' << cu << ' ' << cu << ' ' << c << endl; }
			if (h[a][b + abs1(x1 - x2) / (w0 - cu)] >= 1) { cout << "Rect " << x0 + abs1(x1 - x2) << ' ' << y0 << ' ' << cu << ' ' << cu << ' ' << c << endl; }
		}//若已连接，取消连接
		else if ((h[a][b] <= 1) && (h[a][b + abs1(x1 - x2) / (w0 - cu)] <= 1) && (temp == 0)) {
			cout << "Rect " << x0 << ' ' << y0 << ' ' << abs1(x1 - x2) + cu << ' ' << cu << ' ' << c << endl;
			for (int i = 0; i < (abs1(x1 - x2)) / (w0 - cu); i++) { u[a][b + i] = true; }
			for (int i = 1; i < (abs1(x1 - x2)) / (w0 - cu); i++) { h[a][b + i] += 2; }
			h[a][b]++; h[a][b + abs1(x1 - x2) / (w0 - cu)]++;
		}//若合法，连接
		
	}
	else if ((x1 == x2) && (y1 != y2)) {
		if (y1 > y2) { y0 = y2; }
		check(x0, y0, a, b);
		int temp = 0;
		bool Temp = true;
		for (int i = 1; i < abs1(y1 - y2) / (w0 - cu); i++) { temp = temp + h[a + i][b]; }//用temp记录中间点的使用情况
		for (int i = 0; i < abs1(y1 - y2) / (w0 - cu); i++) { Temp = (Temp && v[a + i][b]); }//用Temp记录中间线的使用情况
		if (Temp) {
			cout << "Rect " << x0 << ' ' << y0 << ' ' << cu << ' ' << abs1(y1 - y2) + cu << ' ' << c0 << endl;
			for (int i = 0; i < abs1(y1 - y2) / (w0 - cu); i++) { v[a + i][b] = false; }
			for (int i = 1; i < (abs1(y1 - y2)) / (w0 - cu); i++) { h[a + i][b] -= 2; }
			h[a][b]--; h[a + abs1(y1 - y2) / (w0 - cu)][b]--; 
			if (h[a][b] >= 1) { cout << "Rect " << x0 << ' ' << y0 << ' ' << cu << ' ' << cu << ' ' << c << endl; }
			if (h[a + abs1(y1 - y2) / (w0 - cu)][b] >= 1) { cout << "Rect " << x0 << ' ' << y0 + abs1(y1 - y2) << ' ' << cu << ' ' << cu << ' ' << c << endl; }
		}//若已连接，取消连接
		else if ((h[a][b] <= 1) && (h[a + abs1(y1 - y2) / (w0 - cu)][b] <= 1) && (temp == 0)) {
			cout << "Rect " << x0 << ' ' << y0 << ' ' << cu << ' ' << abs1(y1 - y2) + cu << ' ' << c << endl;
			for (int i = 0; i < (abs1(y1 - y2)) / (w0 - cu); i++) { v[a + i][b] = true; }
			for (int i = 1; i < (abs1(y1 - y2)) / (w0 - cu); i++) { h[a + i][b] += 2; }
			h[a][b]++; h[a + abs1(y1 - y2) / (w0 - cu)][b]++;
		}//若合法，连接

	}
	return;
}
//识别函数(识别用户操作)
int cx = -1, cy = -1, ckey = 0; string cmsg; int cw, ch;
int reg(string& command)
{
	cout << "DONE" << endl;
	cin >> command;
	if (command == "Click") {
		cin >> cx >> cy;
		if ((cx >= 0) && (cy >= 0)) { return 1; }
		else { return 0; }
	}
	if (command == "Key") {
		cin >> ckey;
		return 2;
	}
	if (command == "Prompt") {
		cin >> cmsg;
		return 3;
	}
	if (command == "Update") {
		cin >> W >> H;
		return 4;
	}

	else { return 0; }
}
//填数函数(在第a行第b列的方格中心显示颜色为c的数字num，并将结果保存在数组n中)
int n[64][64]; bool I[64][64];
void put(int a, int b, int num, string c)
{
	n[a][b] = num; I[a][b] = true;
	cout << "Text " << p[0][a - 1][b - 1] + w0 / 3 << ' ' << p[1][a - 1][b - 1] + w0 / 4 << ' ' << w0 / 2 << ' ' << c << ' ' << num << endl;
	return;
}
void put(int a, int b, int num)//默认黑色
{
	n[a][b] = num; I[a][b] = true;
	cout << "Text " << p[0][a - 1][b - 1] + w0 / 3 << ' ' << p[1][a - 1][b - 1] + w0 / 4 << ' ' << w0 / 2 << ' ' << "black" << ' ' << num << endl;
	return;
}
//选中函数（选中点(x,y)后，在该格点显示一个颜色为cp的小正方形的提示）
string cc = "red", cp = "orange";//选中颜色，默认红色;笔的颜色，默认橙色;
bool slc0(int& x, int& y)
{
	int a, b;
	bool B = apx(x, y);
	if (B) {
		check(x, y, a, b);
		if ((h[a][b] == 0)||((a==0) && (b==0) && (h[0][0]==1))||((a==a0) && (b==b0) && (h[a0][b0]==1))) 
		{cout << "Rect " << x << ' ' << y << ' ' << cu << ' ' << cu << ' ' << c0 << endl;}
		else { cout << "Rect " << x << ' ' << y << ' ' << cu << ' ' << cu << ' ' << cp << endl; }
	}
	return B;
}
bool slc(int& x, int& y)
{
	bool B = apx(x, y);
	if (B) {
		cout << "Rect " << x << ' ' << y << ' ' << cu << ' ' << cu << ' ' << cc << endl;
	}
	return B;
}
//选择两点连线
void com_connect() {
	if (slc(cx, cy)) {
		int cx1 = cx, cy1 = cy; string command1;
		int com1 = reg(command1);
		if ((com1 == 1) && (slc(cx, cy)) && ((cx != cx1) || (cy != cy1))) {
			connect(cx1, cy1, cx, cy, cp);
			slc0(cx1, cy1); slc0(cx, cy);
		}
		else { slc0(cx1, cy1); }
	}return;
}
//沿线更新函数(将当前点的位置更新至沿线下一个点的位置)
bool e[64][64]; //检查数组;
bool up(int& a, int& b)
{
	e[0][0] = true;
	bool B = true;
	if (u[a][b] && (e[a][b + 1] == false)) { b = b + 1; }
	else if (v[a][b] && (e[a + 1][b] == false)) { a = a + 1; }
	else if ((b >= 1) && u[a][b - 1] && (e[a][b - 1] == false)) { b = b - 1; }
	else if ((a >= 1) && v[a - 1][b] && (e[a - 1][b] == false)) { a = a - 1; }
	else { B = false; }
	if (B) { e[a][b] = true; }
	return B;
}
//检查函数(检查当前方案是否合理)
bool exam() {
	for (int i = 0; i <= 63; i++) {
		for (int j = 0; j <= 63; j++) { e[i][j] = false; }
	}
	bool B = true;
	int a = 0, b = 0;
	while ((a != a0) || (b != b0))
	{
		B = up(a, b);
		if (B == false) { return false; }
	}
	//检查是否连通
	for (int i = 0; i <= a0; i++) {
		for (int j = 0; j <= b0; j++) {
			if ((e[i][j] == false) && (u[i][j] || v[i][j])) { B = false; return false; }
		}
	}
	//检查是否有多余边连接
	for (int i = 1; i <= a0; i++) {
		for (int j = 1; j <= b0; j++) {
			if (I[i][j] && ((u[i - 1][j - 1] + v[i - 1][j - 1] + u[i][j - 1] + v[i - 1][j]) != n[i][j]))
			{
				put(i, j, n[i][j], "red"); B = false;
			}
			else if(I[i][j]){ put(i, j, n[i][j]); }
		}
	}//检查每个数字是否完成
	return B;

}
//显示清空键
void scrclear() {
	int cu0 = cu; cu = 5;
	Rect_e(0, 0, 100, 100, c0, cd);
	text(0, 0, 100, 100, "clear", 30, cp);
	cu = cu0;
}

//显示换色键
void scrchangecolor() {
	int cu0 = cu; cu = 5;
	Rect_e(0, 100-cu, 100, 100, c0, cd);
	text(0, 100, 100-cu, 50, "pen", 30, cp);
	text(0, 150, 100-cu, 50, "color", 30, cp);
	cu = cu0;
}
string cT[4], ct[4];
//清空所有连线
void clear() {
	cout << "CLRS" << endl;
	mk_form(a0, b0, w0, c0); scrclear(); scrchangecolor();
	for (int i = 0; i <= a0; i++) {
		for (int j = 0; j <= b0; j++) {
			if (I[i][j]) { put(i, j, n[i][j]); }
			u[i][j] = false; v[i][j] = false; h[i][j] = 0;

		}
	}
	h[0][0] = 1; h[a0][b0] = 1;
}
//拉下颜色选择菜单	
int nc = 1;
void scrcp() {
	nc = 1;
	int cu0 = cu; cu = 5;
	string cpc[4] = { "#00aaff","red","#6943ff","#701e5e" };
	string ccc[4] = { "#00ffff","orange" ,"#00aaff","#8d6a9d" };

	for (int i = 0; i <= 3; i++) {
		if ((cpc[i] != c0) && (ccc[i] != c0)) {
			Rect_e(0, 100 - cu + nc * (100 - cu), 100, 100, c0, cpc[i]);
			 cT[nc-1] = cpc[i]; ct[nc-1] = ccc[i];nc++;
		}
	}nc--;
	cu = cu0;
}


//游戏操作部分；
bool T[4][4];//记录每关通过情况；
void op(int S, int G)//第S层第G关
{
	bool F=false;//记录颜色菜单是否被拉下
	int cu0=5;//菜单粗细
	scrclear(); scrchangecolor();
	string command;
	while (1) {
		int com = reg(command);
		if ((com == 1)) { 
			if(apx(cx,cy)){ com_connect(); }
			else if ((cx <= 100) && (cy <= 100)) { clear(); F = false; }
			else if ((cx <= 100) && (cy <= 200 - cu0)) { 
				if (F) { cout << "Rect " << 0 << ' ' << 200 - cu0 << ' ' << 100 << ' ' << nc * (100 - cu0) << ' ' << cb << endl; F = false; }
				else { scrcp(); F = true; }
			}
			else if (F &&(cx <= 100) && (cy > 200 - cu0) && (cy < 200-cu0 + nc * (100 - cu0))) {
				int N = (cy - 200 + cu0) / (100 - cu0) ;
				cp = cT[N];
				cc = ct[N];
			}
		}
		else if ((com == 2) && (ckey == 27)) { break; }
		else if ((com == 2) && (ckey == 13)) {
			if (exam()) {
				screen("Congratulations!",H/9);
				string tempcom; int com0;
				while (1) {
					com0 = reg(tempcom);
					if ((com0 == 2) && (ckey == 13)) { break; }
				}
				T[S - 1][G - 1] = true;
				break;
			}
			else { continue; }
		}
	}cout << "CLRS" << endl;
	return;
}
//重置函数(恢复所有需要重置的全局变量的初始值)
void re0() {
	cout << "CLRS" << endl;
	for (int i = 0; i <= 63; i++) {

		for (int j = 0; j <= 63; j++) {
			u[i][j] = false; v[i][j] = false; h[i][j] = 0;
			n[i][j] = 0; I[i][j] = false;
		}
	}
	for (int i = 0; i <= 3; i++) {
		cT[i] = "black"; ct[i] = "black";
	}
	nc =1;
	return;
}


//游戏关卡(开始第S层第G关)
void game(int S, int G) {
	if (S == 1) {
		//1-1
		cc = "#00ffff"; cp = "#00aaff";
		if (G == 1) {
			re0();
			mk_form(2, 2, 200, "#ffcc00");
			put(1, 1, 3);
			put(2, 2, 1);
			op(1, 1);
			return;
		}
		if (G == 2) {
			re0();
			mk_form(3, 3, 150, "#ffcc00");
			put(1, 1, 3); put(1, 3, 3);
			put(2, 1, 2); put(2, 3, 2);
			put(3, 3, 1);
			op(1, 2);
			return;
		}
		if (G == 3) {
			re0();
			mk_form(3, 3, 150, "#ffcc00");
			put(1, 2, 3); put(1, 3, 2);
			put(3, 1, 1); put(3, 2, 0); put(3, 3, 1);
			op(1, 3);
			return;
		}
		if (G == 4) {
			re0();
			mk_form(3, 3, 150,"#ffcc00");
			put(1, 2, 2);
			put(2, 2, 3);
			put(3, 2, 1); put(3, 3, 1);
			op(1, 4);
		}
	}
	if (S == 2) {
		//2-1
		cc = "orange"; cp = "red";
		if (G == 1) {
			re0();
			mk_form(3, 3, 150, "#00aaff");
			put(1, 1, 2); put(1, 3, 2);
			put(2, 2, 2);
			put(3, 1, 2); put(3, 3, 2);
			op(2, 1);
			return;
		}
		//2-2
		else if (G == 2) {
			re0();
			mk_form(4, 4, 100, "#00aaff");
			put(1, 2, 2); put(1, 3, 2);
			put(2, 1, 2); put(2, 4, 2);
			put(3, 1, 2); put(3, 4, 2);
			put(4, 2, 2); put(4, 3, 2);
			op(2, 2);
			return;
		}
		//2-3 五阶全二法阵；
		else if (G == 3) {
			re0();
			mk_form(5, 5, 100, "#0055ff");
			put(1, 1, 2); put(1, 2, 2); put(1, 4, 2); put(1, 5, 2);
			put(2, 1, 2); put(2, 5, 2);
			put(3, 3, 2);
			put(4, 1, 2); put(4, 5, 2);
			put(5, 1, 2); put(5, 2, 2); put(5, 4, 2); put(5, 5, 2);
			op(2, 3);
			return;
		}
		//2-4
		else if (G == 4) {
			re0();
			mk_form(6, 6, 100, "#0055ff");
			put(1, 2, 2); put(1, 5, 2);
			put(2, 1, 2); put(2, 3, 2); put(2, 4, 2); put(2, 6, 2);
			put(3, 2, 2); put(3, 5, 2);
			put(4, 2, 2); put(4, 5, 2);
			put(5, 1, 2); put(5, 3, 2); put(5, 4, 2); put(5, 6, 2);
			put(6, 2, 2); put(6, 5, 2);
			op(2, 4);
			return;
		}
	}
	if (S == 3) {
		//3-1
		cc = "#00aaff"; cp = "#6943ff";
		if (G == 1) {
			re0();
			mk_form(4, 4, 100, "#ff90c4");
			put(1, 1, 2); put(1, 3, 3);
			put(2, 2, 2);
			put(3, 1, 3); put(3, 3, 2);
			put(4, 4, 2);
			op(3,1);
			return;
		}
		//3-2
		else if (G == 2) {
			re0();
			mk_form(4, 4, 100, "#ff90c4");
			put(1, 1, 1); put(1, 2, 0); put(1, 3, 1);
			put(2, 2, 1);
			put(3, 1, 2); put(3, 3, 1); put(3, 4, 2);
			put(4, 4, 1);
			op(3, 2);
			return;
		}
		//3-3
		else if (G == 3) {
			re0();
			mk_form(4, 4, 100, "#ff43aa");
			put(1, 2, 3); put(1, 4, 2);
			put(2, 2, 2); put(2, 4, 2);
			put(3, 2, 2); put(3, 4, 2);
			put(4, 2, 3); put(4, 4, 2);
			op(3, 3);
			return;
		}
		//3-4
		else if (G == 4) {
			re0();
			mk_form(4, 4, 100, "#ff43aa");
			put(1, 1, 3); put(1, 2, 3);
			put(2, 1, 2); put(2, 2, 2); put(2, 3, 2); put(2, 4, 3);
			put(3, 1, 3); put(3, 2, 2); put(3, 3, 2); put(3, 4, 2);
			put(4, 3, 1); put(4, 4, 1);
			op(3, 4);
			return;
		}
	}
	if (S == 4) {
		//4-1
		cc = "#8d6a9d"; cp = "#701e5e";
		if (G == 1) {
			re0();
			mk_form(4, 4, 100, "#ffc800");
			put(1, 1, 2); put(1, 4, 2);
			put(2, 1, 2); put(2, 2, 1);
			put(3, 2, 0); put(3, 4, 2);
			put(4, 1, 2); put(4, 4, 2);
			op(4, 1);
			return;
		}
		//4-2
		else if (G == 2) {
			re0();
			mk_form(5, 5, 100, "#ffc800");
			put(1, 1, 2); put(1, 3, 1); put(1, 4, 0); put(1, 5, 1);
			put(2, 2, 2);
			put(3, 1, 1); put(3, 3, 2); put(3, 4, 2);
			put(4, 1, 0); put(4, 3, 2); put(4, 4, 2);
			put(5, 1, 1); put(5, 5, 2);
			op(4, 2);
			return;
		}
		//4-3
		else if (G == 3) {
			re0();
			mk_form(6, 6, 100, "orange");
			put(1, 1, 2); put(1, 5, 3);
			put(2, 2, 2); put(2, 6, 1);
			put(3, 3, 2);
			put(4, 1, 0); put(4, 4, 2); put(4, 6, 1);
			put(5, 3, 2); put(5, 5, 2);
			put(6, 3, 2); put(6, 6, 2);
			op(4, 3);
		}
		//4-4
		else if (G == 4) {
			re0();
			mk_form(7, 7, 100, "orange");
			put(1, 2, 1); put(1, 5, 2); put(1, 7, 2);
			put(2, 1, 2);
			put(3, 2, 2); put(3, 4, 3); put(3, 7, 2);
			put(4, 1, 2); put(4, 3, 2); put(4, 6, 2);
			put(5, 2, 2); put(5, 5, 3); put(5, 7, 1);
			put(6, 1, 2); put(6, 6, 0);
			put(7, 3, 1);
			op(4, 4);
			return;
		}
	}
}
//显示选关界面
void choose1() {
	bool B=true;
	for (int i = 0; i <= 3; i++) {
		B = true;
		for (int j = 0; j <= 3; j++) { B = (B && T[i][j]); }
		if (B) {
			string is = to_string(i+1);
			Rext(W / 6 - W / 10 + 7 * i * W / 30, (H - W / 6) / 2, W / 6, W / 6, "orange", is, W / 18);
		}
		else {
			string is = to_string(i+1);
			Rext(W / 6 - W / 10 + 7 * i * W / 30, (H - W / 6) / 2, W / 6, W / 6, cs, is, W / 18);
		}
	}
}
void choose2(int i) {
	for (int j = 0; j <= 3; j++) {
		string is = to_string(i);
		if (T[i-1][j]) {
			string js = to_string(j + 1);
			Rext(W / 6 - W / 10 + 7 * j * W / 30, (H - W / 6) / 2, W / 6, W / 6, "orange", is+'-'+js, W / 18);
		}
		else {
			string js = to_string(j + 1);
			Rext(W / 6 - W / 10 + 7 * j * W / 30, (H - W / 6) / 2, W / 6, W / 6, cs, is + '-' + js, W / 18);
		}
	}
}
//判断点击位置(读入鼠标点击位置，判断该位置在第几个方格)
int chg() {
	string command1;
	while (1) {
		int com1 = reg(command1);
		if (com1 == 1) {
			if ((cy >= (H - W / 6) / 2) && (cy <= (H + W / 6) / 2)) {
				for (int i = 0; i <= 3; i++) {
					if ((cx >= (W / 6 - W / 10 + 7 * i * W / 30)) && (cx <= (W / 3 - W / 10 + 7 * i * W / 30))) { return(i + 1); }
				}
			}
		}
		else if ((com1 == 2) && (ckey == 27)) { cout << "CLRS" << endl; break; }
	}return 0;
}
//游戏logo
void con(int a1, int b1, int a2, int b2) {
	connect(p[0][a1][b1], p[1][a1][b1], p[0][a2][b2], p[1][a2][b2],"#701e5e");
}
void csgo() {
	re0();
	cb = "#dddddd"; int cu0 = cu; cu = 15;
	mk_form(7, 7, 100, "white");
	con(1, 1, 1, 3); con(1, 1, 3, 1);con(3, 1, 3, 3);
	con(1, 4, 1, 6); con(1, 4, 2, 4); con(2, 4, 2, 6); con(2, 6, 3, 6); con(3, 4, 3, 6);
	con(4, 1, 4, 3); con(4, 1, 6, 1); con(6, 1, 6, 3); con(6, 3, 5, 3); con(5, 2, 5, 3);
	con(4, 4, 4, 6); con(4, 6, 6, 6); con(4, 4, 6, 4); con(6, 4, 6, 6);
	cb = "white";cu = cu0;
}

int main()
{
	string command;
	cin >> command >> W >> H;	
	csgo();
	while (1) {
		int com = reg(command);
		if ((com == 2) && (ckey==13)) {
			while (1) {
				cout << "CLRS" << endl;		
				choose1();
				int s = 0, g = 0;
				s = chg();
				if (s>=1) {
					while (1) {
						cout << "CLRS" << endl;
						choose2(s);
						g = chg();
						if (g >= 1) { game(s, g); }
						else { break; }
					}
					continue;
				}
				else { break; }
			}
		}//选关部分
		else if ((com == 2) && (ckey == 46)) { for (int i = 0; i <= 3; i++) { for (int j = 0; j <= 3; j++) { T[i][j] = false; } } }//重置游戏数据
		else if ((com == 2) && (ckey == 27)) { cout << "CLRS" << endl; break; }
		csgo();
	}
	return 0;
}
