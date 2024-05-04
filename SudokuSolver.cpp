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
int row[9], col[9], cel[3][3];  // ״̬ѹ��
int Log2[1 << 11];

inline int get(const P& p) {  // λ����
    return row[p.x] | col[p.y] | cel[p.x / 3][p.y / 3];
}

inline void flip(const P& p, int n) {  // λ����
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
        cout << "\n�����������Ϲ棡\n�����ڣ���" << f.x + 1 << " ��" << f.y + 1 << " ��" << f.w << '\n';
        cout << "\n���س����Խ������򡭡�    ";
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

inline void Sort(int o) {  // ����ʽ����
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
    if (o == tot) {  // �������ض�
        ++num;
        cout << "\n<��" << num << ">\n";
        show();
        return;
    }
    if (!isSort[o]) {  // ����ʽ����
        isSort[o] = true;
        Sort(o);
    }
    const P& p = blk[o];
    int all = ~(get(p) | 1);
    for (int i = Log2[all & -all]; i <= 9; all ^= 1 << i, i = Log2[all & -all]) {  // λ���� ��֦
        flip(p, i);
        dfs(o + 1);
        flip(p, i);
    }
}

int main() {
    init();
    cout << "\n��ʼ����ɣ���ʱ��ʼ����\n";
    auto start = chrono::high_resolution_clock::now();

    max_num = 1000;
    // freopen("answer.txt","w",stdout);
    dfs();

    if (num == 0)
        cout << "\n�޽⣡\n";
    else if (num == max_num)
        cout << "\n���������ɣ�������������Ϊ" << max_num << "��\n";
    else
        cout << "\n�����ɣ�\n";

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "\n��ʱ��" << ceil(duration.count() * 1000) << " ms" << endl;

    cout << "\n���س����Խ������򡭡�    ";
    getchar();
    getchar();

    return 0;
}

/*

���������ѵ�������
800000000
003600000
070090200
050007000
000045700
000100030
001000068
008500010
090000400

ȫ�����������ڲ��Խ����������
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000
000000000

һ�����������ڲ��Խ����������
800000000
003600000
070090200
080007000
000045700
000100030
001000068
008500010
090000400

�޽�������
009001620
570028030
300700004
890070400
060403090
001090076
600007008
040130065
027600900

���Ϲ��������
869712543
243658179
175493286
954237651
316845792
752169834
521974368
438526917
697381425

�Ϲ�������
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
