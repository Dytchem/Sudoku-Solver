#include <iostream>
#include <cstdio> 
#include <cmath>
#include <algorithm>
#include <chrono>
using namespace std;

chrono::time_point<chrono::high_resolution_clock> start;

struct P{
	int x,y,w;
	bool operator<(const P& p)const{
		if (w!=p.w) return w>p.w;
		else if (x!=p.x) return x<p.x;
		else return y<p.y;
	}
};

int a[9][9];
P blk[81]; int tot=0;
int row[9],col[9],cel[3][3];

inline int get(const P& p){
	return row[p.x]|col[p.y]|cel[p.x/3][p.y/3];
}

inline void flip(const P& p,int n){
	a[p.x][p.y]^=n;
	row[p.x]^=1<<n;
	col[p.y]^=1<<n;
	cel[p.x/3][p.y/3]^=1<<n;
}
	
inline P init(){
	P re{-1};
	char c;
	for (int i=0;i<9;++i){
		for (int j=0;j<9;++j){
			P p{i,j};
			cin>>c; c-='0';
			if ((get(p)>>c)&1) re=P{i,j,c};
			if (c) flip(p,c);
			else blk[tot++]=p;
		}
	}
	
	for (int i=0;i<tot;++i){
		const int all=get(blk[i]);
		for (int j=1;j<=9;++j){
			blk[i].w+=(all>>j)&1;
		}
	}
	sort(blk,blk+tot);
	
	if (re.x!=-1) return re;
	cout<<"\n初始化完成！计时开始……\n";
	start=chrono::high_resolution_clock::now();
	
	return re;
}
	
inline void show(){
	for (int i=0;i<9;++i){
		for (int j=0;j<9;++j){
			cout<<a[i][j];
		}
		cout<<'\n';
	}
}

inline void finish(){
	chrono::time_point<chrono::high_resolution_clock> end=chrono::high_resolution_clock::now();
	chrono::duration<double> duration=end-start;
	cout<<"\n用时："<<ceil(duration.count()*1000)<<" ms"<<endl;
	
	cout<<"\n按回车键以结束程序……    ";
	getchar();
	getchar();
	exit(0);
}

int num=0,max_num=5;

void dfs(int o=0){
	if (num==max_num) return;
	if (o==tot){
		++num;
		cout<<"\n<答案"<<num<<">\n";
		show();
		//finish();
	}
	const P& p=blk[o];
	const int all=get(p);
	for (int i=1;i<=9;++i){
		if ((all>>i)&1) continue;
		flip(p,i);
		dfs(o+1);
		flip(p,i);
	}
}


int main(){
	P p=init();
	if (p.x!=-1){
		cout<<"\n输入数独不合规！\n错误在：行"<<p.x+1<<" 列"<<p.y+1<<" 数"<<p.w<<'\n';
		cout<<"\n按回车键以结束程序……    ";
		getchar();
		getchar();
		return 0;
	}

	max_num=1000; 
	//freopen("answer.txt","w",stdout);
	dfs();
	
	if (num==0) cout<<"\n无解！\n";
	else if (num==max_num) cout<<"\n部分求解完成！（答案数量上限为"<<max_num<<"）\n";
	else cout<<"\n求解完成！\n";
	finish();
	
	return 0;
}

/*

世界上最难的数独： 
800000000
003600000
070090200
050007000
000045700
000100030
001000068
008500010
090000400

全空数独（用于测试解的数量）： 
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000

一般数独（用于测试解的数量）： 
800000000
003600000
070090200
080007000
000045700
000100030
001000068
008500010
090000400

不合规的数独： 
869712543
243658179
175493286
954237651
316845792
752169834
521974368
438526917
697381425

合规数独： 
869712543
243658179
175493286
984237651
316845792
752169834
521974368
438526917
697381425

*/
