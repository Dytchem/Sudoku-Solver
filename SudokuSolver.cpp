#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <iostream>
using namespace std;

struct P {
    int x, y, w;
    bool operator<(const P& p) const {
        if (w != p.w)
            return w > p.w;
        else if (x != p.x)
            return x < p.x;
        else
            return y < p.y;
    }
};

int a[9][9];
P blk[81];
int tot = 0;
int row[9], col[9], cel[3][3];  // 状态压缩
int Log2[1 << 11];

inline int get(const P& p) {  // 位运算
    return row[p.x] | col[p.y] | cel[p.x / 3][p.y / 3];
}

inline void flip(const P& p, int n) {  // 位运算
    a[p.x][p.y] ^= n;
    row[p.x] ^= 1 << n;
    col[p.y] ^= 1 << n;
    cel[p.x / 3][p.y / 3] ^= 1 << n;
}

inline void init() {
    for (int i = 1; i < 11; ++i)
        Log2[1 << i] = i;

    P f{-1};
    char c;
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            P p{i, j};
            cin >> c;
            c -= '0';
            if ((get(p) >> c) & 1)
                f = P{i, j, c};
            if (c)
                flip(p, c);
            else
                blk[tot++] = p;
        }
    }

    if (f.x != -1) {
        cout << "\n输入数独不合规！\n错误在：行" << f.x + 1 << " 列" << f.y + 1 << " 数" << f.w << '\n';
        cout << "\n按回车键以结束程序……    ";
        getchar();
        getchar();
        exit(0);
    }
}

inline void show() {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            cout << a[i][j];
        }
        cout << '\n';
    }
}

inline void Sort(int o) {  // 启发式排序
    for (int i = o; i < tot; ++i) {
        int all = get(blk[i]);
        blk[i].w = 0;
        for (int j = Log2[all & -all]; j; all ^= 1 << j, j = Log2[all & -all])
            blk[i].w++;
    }
    sort(blk + o, blk + tot);
}

bool isSort[81];
int num = 0, max_num;

void dfs(int o = 0) {  // DFS
    if (num == max_num)
        return;
    if (o == tot) {  // 答案数量截断
        ++num;
        cout << "\n<答案" << num << ">\n";
        show();
        return;
    }
    if (!isSort[o]) {  // 启发式排序
        isSort[o] = true;
        Sort(o);
    }
    const P& p = blk[o];
    int all = ~(get(p) | 1);
    for (int i = Log2[all & -all]; i <= 9; all ^= 1 << i, i = Log2[all & -all]) {  // 位运算 剪枝
        flip(p, i);
        dfs(o + 1);
        flip(p, i);
    }
}

int main() {
    init();
    cout << "\n初始化完成！计时开始……\n";
    auto start = chrono::high_resolution_clock::now();

    max_num = 1000;
    // freopen("answer.txt","w",stdout);
    dfs();

    if (num == 0)
        cout << "\n无解！\n";
    else if (num == max_num)
        cout << "\n部分求解完成！（答案数量上限为" << max_num << "）\n";
    else
        cout << "\n求解完成！\n";

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "\n用时：" << ceil(duration.count() * 1000) << " ms" << endl;

    cout << "\n按回车键以结束程序……    ";
    getchar();
    getchar();

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

无解数独：
009001620
570028030
300700004
890070400
060403090
001090076
600007008
040130065
027600900

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
