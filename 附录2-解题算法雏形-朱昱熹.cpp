//Bruce 2019011332

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

struct sudoku{///存储数独
    int num[9][9];///存储标准：如果填写过，则用1～9表示，如果未填写过，则用0表示
};

int answer[2][9][9] = {{{0}}};

///将位置转换成block的索引
int pos2block(int x, int y){
    return x / 3 * 3 + y / 3;
}

int block2x(int block, int idx){
    return block / 3 * 3 + idx / 3;
}

int block2y(int block, int idx){
    return block % 3 * 3 + idx % 3;
}

///生成一个填入了25个数的数独
///返回：生成的数独
sudoku make_sudoku(){
    sudoku made = {};
    int g = 0;
    bool x[9][10] = {{false}}, y[9][10] = {{false}}, block[9][10] = {{false}};
    while(g < 25){
        int tmp = rand() % 9 + 1;
        int posx = rand() % 9, posy = rand() % 9;
        if(made.num[posx][posy] == 0 && !x[posx][tmp] && !y[posy][tmp] && !block[pos2block(posx, posy)][tmp]){
            made.num[posx][posy] = tmp;
            x[posx][tmp] = true;
            y[posy][tmp] = true;
            block[pos2block(posx, posy)][tmp] = true;
            g ++;
        }
    }
    return made;
}

int dfs(sudoku in, bool x[9][10], bool y[9][10], bool block[9][10], int a, int idx, int noa){
    if(a == 10){
        for(int i = 0; i < 9; i ++){
            for(int j = 0; j < 9; j ++){
                answer[noa][i][j] = in.num[i][j];
            }
        }
        return noa + 1;
    }
    if(idx == 9){
        return dfs(in, x, y, block, a + 1, 0, noa);
    }
    if(block[idx][a]){
        return dfs(in, x, y, block, a, idx + 1, noa);
    }

    for(int i = 0; i < 9; i += 3){
        int posx = block2x(idx, i);
        if(x[posx][a]){
            continue;
        }
        for(int j = 0; j < 3; j ++){
            int posy = block2y(idx, i + j);
            if(y[posy][a] || in.num[posx][posy]){
                continue;
            }
            x[posx][a] = true;
            y[posy][a] = true;
            in.num[posx][posy] = a;
            noa = dfs(in, x, y, block, a, idx + 1, noa);
            if(noa == 2){
                return 2;
            }
            x[posx][a] = false;
            y[posy][a] = false;
            in.num[posx][posy] = 0;
        }
    }
    return noa;
}

///获取数独的解
///参数：要解的数独
///返回：解的个数
int get_answer(sudoku in){
    bool x[9][10] = {{0}};///表征行：第一维为行号，第二维为记录的字符
    bool y[9][10] = {{0}};///表征列
    bool block[9][10] = {{0}};///表征块
    ///记录危险状态
    for(int i = 0; i < 9; i ++){
        for(int j = 0; j < 9; j ++){
            if(in.num[i][j]){
                if(!x[i][in.num[i][j]] && !y[j][in.num[i][j]] && !block[pos2block(i, j)][in.num[i][j]]) {
                    x[i][in.num[i][j]] = true;
                    y[j][in.num[i][j]] = true;
                    block[pos2block(i, j)][in.num[i][j]] = true;
                } else {
                    return -1;
                }
            }
        }
    }

    ///逐行深搜
    return dfs(in, x, y, block, 1, 0, 0);
}

///从键盘输入一个数独，并将其转换成标准形式
///返回：根据输入得到的标准数独（用0至9表示各个数或空格）
sudoku input(){
    sudoku in = {};
    char tmp;
    for(int i = 0; i < 9; i ++){
        for(int j = 0; j < 9; j ++){
            cin >> tmp;
            if(tmp == '-'){
                in.num[i][j] = 0;
            } else {
                in.num[i][j] = tmp - '1' + 1;
            }
        }
    }
    return in;
}

///将全局变量answer中的解展示出来
///参数：解的index
void display(int idx){
    for(int i = 0; i < 9; i ++){
        for(int j = 0; j < 9; j ++){
            cout << answer[idx][i][j] << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

///将得到了的解数独结果展示出来
///参数：get_answer函数的返回值，即数独解的个数
void output(int noa){
    if(noa == -1){
        cout << "No_solution" << endl;
        cerr << "No_solution" << endl;
    }
    else if(noa == 0){
        cout << "No_solution" << endl;
        cerr << "No_solution" << endl;
    }
    else if(noa == 1){
        cout << "OK" << endl;
        cerr << "OK" << endl;
        display(0);
    }
    else if(noa == 2){
        cout << "Multiple_solutions" << endl;
        cerr << "Multiple_solutions" << endl;
        display(0);
        display(1);
    }
}

///将传入的sudoku显示出来
void show(sudoku in){
    //sudoku out{};
    for(int i = 0; i < 9; i ++){
        for(int j = 0; j < 9; j ++){
            cout << in.num[i][j] << ' ';
        }
        cout << endl;
    }
}

///根据一个填入了25个数的多解数独和其一个解，生成一个填入了约38个数的数独
///参数：多解数独
///引用：全局变量：answer[0]
///返回：一个填入约38个数的数独
sudoku build_sudoku(sudoku made){
    int g = 0;
    /*
    for(int i = 0; i < 9; i ++){
        for (int j = 0; j < 9; j ++){
            if(made.num[i][j] == 0){
                made.num[i][j] = answer[0][i][j];
                g ++;
                if(g > 13){
                    return made;
                }
            }
        }
    }
     */
    while (g < 13){
        int posx = rand() % 9, posy = rand() % 9;
        if(made.num[posx][posy] == 0){
            made.num[posx][posy] = answer[0][posx][posy];
            g ++;
        }
    }
    return made;
}

int main(){
    srand(time(nullptr));
    int com;
    cin >> com;
    sudoku in = {};
    if(com == 1) {
        sudoku made = {};
        do {
            int g = 0;
            do {
                made = make_sudoku();
                g = get_answer(made);
            } while (g <= 0);
            if (g == 1) {
                show(made);
                break;
            }
            sudoku out = build_sudoku(made);
            if(get_answer(out) == 1){
                show(out);
                break;
            }
        }while(true);
    }
    if(com == 2) {
        in = input();
        output(get_answer(in));
    }
    return 0;
}

/*
 2
 - - 5 3 - - - - -
 8 - - - - - - 2 -
 - 7 - - 1 - 5 - -
 4 - - - - 5 3 - -
 - 1 - - 7 - - - 6
 - - 3 2 - - - 8 -
 - 6 - 5 - - - - 9
 - - 4 - - - - 3 -
 - - - - - 9 7 - -
 */