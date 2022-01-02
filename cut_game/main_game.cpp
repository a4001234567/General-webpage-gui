#include <vector>
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;
const int A_WIN = 1;
const int B_WIN = 2;
const int A_PLAY = -1;
const int B_PLAY = -2;
const int A_OWNED = 1;
const int B_OWNED = 2;
const int RED = 1;
const int BLUE = 2;
const int BLACK = 3;
const int infty = 114514;
//floor division that makes -1/2 = -1,used by "click" function
inline int floor_divide(int a,int b){
	if (a > 0) return a/b;
	return a/b-bool(a%b);
}
class cut_games{
	private:
		//the points are stored in a big matrix, each access in this vector either represents an edge(the intersection of two possible edges)
		//or an existing point. 
		//it marks whether the edge is taken by A, by B, and stores the information about the connectivity of existing points(see find_union set).
		//there's 2*n+1+2 * 2*m+1+ elements in total
		vector<vector<int> > nodes;//storage for the edge's condition
		int n,m;//A has n edges in width, B has m edges in his width

		//A_WIN,B_WIN,A_PLAY,or B_PLAY
		//A_WIN = A_OWNED,B_WIN = B_OWNED, which simplifies things in the later route part
		int status;
		//num is the 1 dimensional representation for all the elements
		int get_x(int num){
			return num%(2*m+3);
		}
		int get_y(int num){
			return num/(2*m+3);
		}
		int get_num(int x,int y){
			return y*(2*m+3)+x;
		}

		//return if the point(x,y) is a valid entry in the "nodes".
		bool is_legal(int x,int y){
			return (0 <= x && x <= ((m+1)<<1) && 0 <= y && y <= ((n+1)<<1));
		}

		int find_root(int num){
			int cur = num;//trace to the root
			while (nodes[get_x(cur)][get_y(cur)] > 0){
				cur = nodes[get_x(cur)][get_y(cur)];
			}
			int root = cur;
			cur = num;//update the root of all the nodes on the way up
			while (nodes[get_x(cur)][get_y(cur)] > 0){
				int temp = nodes[get_x(cur)][get_y(cur)];
				nodes[get_x(cur)][get_y(cur)] = root;
				cur = temp;
			}
			return root;
		}

		void merge(int num1,int num2){//for connecting edge
			int r1 = find_root(num1);
			int r2 = find_root(num2);
			if (r1 == r2) return;
			int x1 = get_x(r1), x2 = get_x(r2);
			int y1 = get_y(r1), y2 = get_y(r2);
			if (nodes[x1][y1] <= nodes[x2][y2]){
				nodes[x1][y1] += nodes[x2][y2];
				nodes[x2][y2] = r1;
			}
			else {
				nodes[x2][y2] += nodes[x1][y1];
				nodes[x1][y1] = r2;
			}
		}
		
		//mark the edge given by coordinates x,y checked
		//return 0 for success	1 for succes and also a winner	-1 for illegal request
		int check(int x,int y){
			// check if the request
			if ((x+y)&1)
				return -1;
			if (status > 0)//there's already a winner
				return -1;
			if (x <= 0 || y <= 0 || x >= 2 + (m<<1) || y >= 2 + (n<<1))
				return -1;//the point is out of range
			if (nodes[x][y])//the edge is already taken
				return -1;
			//apply the check
			if (status == A_PLAY){
				if (x&1)//connect the dots
					merge(get_num(x-1,y),get_num(x+1,y));
				else
					merge(get_num(x,y-1),get_num(x,y+1));
				nodes[x][y] = A_OWNED;//store the status for the point x,y
				if (find_root(get_num(0,1)) == find_root(get_num(2*m+2,1))){//check if someone is winning
					status = A_WIN;
					return 1;
				}
				status = B_PLAY;
				return 0;
			}
			if (status == B_PLAY){
				if (x&1)
					merge(get_num(x,y-1),get_num(x,y+1));
				else
					merge(get_num(x-1,y),get_num(x+1,y));
				nodes[x][y] = B_OWNED;
				if (find_root(get_num(1,0)) == find_root(get_num(1,2*n+2))){
					status = B_WIN;
					return 1;
				}
				status = A_PLAY;
				return 0;
			}
			return 0;
		}

		//this route finds a path from left to right if a won, or a path from top to bottom if b won
		//it is a variant of bellman ford algorithm and is supposed to find the shortest path in length
		//use the vector ane to remember the path(anc[x] means the shortest path to x would go through anc[x] first)
		//anc is allocated in the init function of the class, however there should be no need for a default value
		vector<int> anc;
		vector<int> route(){
			//store the points the path go through
			vector<int> result;
			//check if there's actually nobody winning
			if (status != A_WIN && status != B_WIN)
				return result;
			if (status == A_WIN){
				//since there's already a winner, the connectivity of the graph shall not be changed afterwards
				//the the place for the "find-union set" can be used as storage for the shortest distance.
				//and we shall update all the points with infty with only the left-most point with 0
				for (int x = 0;x < nodes.size();++x)
					for (int y = (x+1)&1;y < nodes[x].size();y+=2)
						nodes[x][y] = x?infty:0;
				bool is_update = true;
				while (is_update){
					is_update = false;
					for (int x = 0;x < nodes.size();++x)
						for (int y = (x+1)&1;y < nodes[x].size();y+=2){
							//update distances and ancestors, used a stupid method to enumerate all the direction
							if (is_legal(x+2,y) && nodes[x+1][y] == status)
								if (nodes[x+2][y] > nodes[x][y] + 1){
									nodes[x+2][y] = nodes[x][y]+1;
									anc[get_num(x+2,y)] = get_num(x,y);
									is_update = true;
								}
							if (is_legal(x,y+2) && nodes[x][y+1] == status)
								if (nodes[x][y+2] > nodes[x][y] + 1){
									nodes[x][y+2] = nodes[x][y] +1;
									anc[get_num(x,y+2)] = get_num(x,y);
									is_update = true;
								}
							if (is_legal(x,y-2) && nodes[x][y-1] == status)
								if (nodes[x][y-2] > nodes[x][y] +1){
									nodes[x][y-2] = nodes[x][y] +1;
									anc[get_num(x,y-2)] = get_num(x,y);
									is_update = true;
								}
							if (is_legal(x-2,y) && nodes[x-1][y] == status)
								if (nodes[x-2][y] > nodes[x][y] + 1){
									nodes[x-2][y] = nodes[x][y] + 1;
									anc[get_num(x-2,y)] = get_num(x,y);
									is_update = true;
								}
						}
				}
				//check if path is already calculated and find out the route
				int x = nodes.size()-1;
				//first find out the destination, because the update method would update all the rightmost points in one run
				int min_dis = infty;
				for (int y = 1;y < nodes[x].size();y += 2)
					min_dis = min_dis > nodes[x][y] ? nodes[x][y] : min_dis;
				for (int y = 1;y < nodes[x].size();y += 2)
					if (nodes[x][y] == min_dis){
						result.push_back(get_num(x,y));
						while (get_x(result.back()))
							result.push_back(anc[result.back()]);
						return result;
					}
			}
			if (status == B_WIN){
				for (int y = 0;y < nodes[0].size();++y)
					for (int x = (y+1)&1;x < nodes.size();x+=2)
						nodes[x][y] = y?infty:0;
				bool is_update = true;
				while (is_update){
					is_update = false;
					for (int y = 0;y < nodes[0].size();++y)
						for (int x = (y+1)&1;x < nodes.size();x+=2){
							if (is_legal(x,y+2) && nodes[x][y+1] == status)
								if (nodes[x][y+2] > nodes[x][y]+1){
									nodes[x][y+2] = nodes[x][y]+1;
									anc[get_num(x,y+2)] = get_num(x,y);
									is_update = true;
								}
							if (is_legal(x+2,y) && nodes[x+1][y] == status)
								if (nodes[x+2][y] > nodes[x][y]+1){
									nodes[x+2][y] = nodes[x][y]+1;
									anc[get_num(x+2,y)] = get_num(x,y);
									is_update = true;
								}
							if (is_legal(x-2,y) && nodes[x-1][y] == status)
								if (nodes[x-2][y] > nodes[x][y]+1){
									nodes[x-2][y] = nodes[x][y]+1;
									anc[get_num(x-2,y)] = get_num(x,y);
									is_update = true;
								}
							if (is_legal(x,y-2) && nodes[x][y-1] == status)
								if (nodes[x][y-2] > nodes[x][y] +1){
									nodes[x][y-2] = nodes[x][y]+1;
									anc[get_num(x,y-2)] = get_num(x,y);
									is_update = true;
								}
						}
				}
				int y = nodes[0].size()-1;
				int min_dis = infty;
				for (int x = 1;x < nodes.size();x += 2)
					min_dis = min_dis > nodes[x][y] ? nodes[x][y] : min_dis;
				for (int x = 1;x < nodes.size();x += 2)
					if (nodes[x][y] == min_dis){
						result.push_back(get_num(x,y));
						while (get_y(result.back()))
							result.push_back(anc[result.back()]);
						return result;
					}
			}
			return result;
		}
		//below are the basic drawing function
		//parameters for graphic user interface
		int basic_length = 20;//length of each grid
		//the length used when the screen is bigger than the whole grid
		//use it to keep the grid center
		int offset_x = 0,offset_y = 0;

		void rect(int x,int y, int w, int h,int color){
			int real_x = x + offset_x;
			int real_y = y + offset_y;
			printf("Rect %d %d %d %d ",real_x,real_y,w,h);
			if (RED == color)puts("red");
			if (BLUE == color)puts("blue");
			if (BLACK == color)puts("black");
		}

		void dot(int x,int y,int r,int color){
			int real_x = x * basic_length - r + offset_x;
			int real_y = y * basic_length - r + offset_y;
			printf("Circ %d %d %d ",real_x,real_y,r);
			if (RED == color)puts("red");
			if (BLUE == color)puts("blue");
			if (BLACK == color)puts("black");
		}

		void draw_board(){//draw all the dots
			for (int x = 0;x < nodes.size();++x)
				for (int y = 0;y < nodes[x].size();++y)
					if ((x+y)&1){
						if (x&1){//dots of B
							dot(x,y,basic_length/10,BLUE);
						}else{
							dot(x,y,basic_length/10,RED);
						}
					}
		}
		
		void draw_connection(int x,int y,int stroke = -1){
			if (-1 == stroke) stroke = basic_length/20;
			if ((x+y)&1) return;
			if (nodes[x][y] == A_OWNED){
				if (x&1)
					rect((x-1) * basic_length,y * basic_length - stroke,basic_length << 1,stroke << 1,RED);
				else
					rect(x * basic_length - stroke,(y-1) * basic_length,stroke << 1,basic_length << 1,RED);
				return;
			}
			if (nodes[x][y] == B_OWNED){
				if (x&1)
					rect(x * basic_length - stroke,(y-1) * basic_length,stroke << 1,basic_length << 1,BLUE);
				else
					rect((x-1) * basic_length,y * basic_length - stroke,basic_length << 1,stroke << 1,BLUE);
			}
		}

		void draw_line(){
			for (int x = 0;x < nodes.size();++x)
				for (int y = 0;y < nodes.size();++y)
					draw_connection(x,y);
		}
		
		void draw_route(){
			if (this->status == A_WIN || this->status == B_WIN){
				vector<int> path = this->route();
				for (int i = 0;i < path.size()-1;++i)
					draw_connection((get_x(path[i])+get_x(path[i+1]))>>1,(get_y(path[i])+get_y(path[i+1])>>1),basic_length/10);
			}
		}

		void draw_game(){
			draw_line();
			if (this->status == A_WIN || this->status == B_WIN)
				draw_route();
			draw_board();
		}
	public:
		cut_games(int n,int m){
			anc.assign((2*m+3)*(2*n+3),-1);
			nodes.assign(2*m+3,vector<int>());
			for (int i = 0;i<nodes.size();++i)
				nodes[i].assign(2*n+3,0);
			this->n = n;
			this->m = m;
			this->status = A_PLAY;
			for (int x = 0;x < nodes.size();++x)
				for (int y = 0;y < nodes[x].size();++y)
					if ((x+y)&1) nodes[x][y] = -1;
			for (int y = 3;y < 2*n+3; y += 2){
				merge(get_num(0,y-2),get_num(0,y));
				nodes[0][y-1] = A_OWNED;
				merge(get_num(2*m+2,y-2),get_num(2*m+2,y));
				nodes[2*m+2][y-1] = A_OWNED;
			}
			for (int x = 3;x < 2*m+3; x += 2){
				merge(get_num(x-2,0),get_num(x,0));
				nodes[x-1][0] = B_OWNED;
				merge(get_num(x-2,2*n+2),get_num(x,2*n+2));
				nodes[x-1][2*n+2] = B_OWNED;
			}
		}

		void update_screen_size(int new_width,int new_height){
			int canvas_width = this->basic_length*(2*m+2);
			int canvas_height = this->basic_length*(2*n+2);
			this->offset_x = (new_width-canvas_width)>>1;
			this->offset_y = (new_height-canvas_width)>>1;
			draw_game();
		}	

		int click(int x,int y){
			x -= offset_x;
			y -= offset_y;
			//int a = int(floor(double(x+y+basic_length)/2/basic_length));
			int a = floor_divide(x+y+basic_length,basic_length<<1);
			//int b = int(floor(double(x-y+basic_length)/2/basic_length));
			int b = floor_divide(x-y+basic_length,basic_length<<1);
			int r = this->check(a+b,a-b);
			if (-1 == r) return -1;
			if (status == A_WIN)puts("Alert A{SPACE}WIN!");
			if (status == B_WIN)puts("Alert B{SPACE}WIN!");
			draw_connection(a+b,a-b);
			if (1 == r) draw_route();
			return r;
		}
		void draw(){
			this->draw_game();
		}
		void test(){
			check(1,1);
			check(3,1);
			check(2,2);
			check(3,3);
			check(1,1);
			vector<int> temp;
			temp = this->route();
			for(int i = 0;i<temp.size();++i)
				printf("%d ",temp[i]);
		}
};
void CLRS(){
	puts("CLRS");
}
int main(){
	cut_games a(10,11);
	int cur_n = 10;
	string command;
	int update_w = 1024,update_h = 768;
	while (true){
		cin>>command;
		if ("EXIT" == command) return 0;
		if ("Update" == command){
			CLRS();
			scanf("%d %d",&update_w,&update_h);
			a.update_screen_size(update_w,update_h);
		}
		if ("Click" == command){
			int pos_x,pos_y;
			scanf("%d %d",&pos_x,&pos_y);
			a.click(pos_x,pos_y);
		}
		if ("BOOK" == command){}
		if ("Prompt" == command){
			string trash;cin>>trash;
		}
		if ("Key" == command){
			int key_code;scanf("%d",&key_code);
			if (key_code == 80){
				a = cut_games(cur_n,cur_n+1);
				CLRS();
				a.update_screen_size(update_w,update_h);
				puts("Alert New{SPACE}Game");
			}
		}
		puts("DONE");
	}
	return 0;
}
