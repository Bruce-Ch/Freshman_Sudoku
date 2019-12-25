#include <iostream>
#include <stack>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <cstdio>
using namespace std;

///initialize: provide sudoku[9][9]
///function: void output(): show the solution(s) as required
///          void sol1(): get the first solution
///          void sol2(): get the second solution
///members: int answer: the case index of the sudoku: 0: no solution 1: one solution 2: more than one solutions
///         int a[9][9]: after sol1() or sol2(), the solution of the sudoku
class DLX{
    const static int N = 9;
    const static int N2 = 81;
    const static int N3 = 729;
    const static int M = N3 * N2 * 5;

public:
    int a[N][N]{}, ans{};

private:
    int col_cnt[N3 * 2]{}, sol[2][N2 + 1]{};

    struct node{
        int u, d, l, r, row, col;
    }l[M]{};
    void ins(int x, int row, int col){
        l[x] =  (node){l[col].u, col, l[row].l, row, row, col};
        l[l[col].u].d = x;
        l[col].u = x;
        l[l[row].l].r = x;
        l[row].l = x;
        col_cnt[col]++;
    }
    void del(int x){
        l[l[x].u].d = l[x].d;
        l[l[x].d].u = l[x].u;
        l[l[x].l].r = l[x].r;
        l[l[x].r].l = l[x].l;
        col_cnt[l[x].col]--;
    }
    void back(int x){
        l[l[x].u].d = x;
        l[l[x].d].u = x;
        l[l[x].l].r = x;
        l[l[x].r].l = x;
        col_cnt[l[x].col]++;
    }
    void build(){
        int tot = N3 + N2 * 4;
        l[0] = (node){0, 0, tot, N3 + 1, 0, 0};//´´½¨head
        for(int i = 1; i <=	N3; i++)l[i] = (node){0, 0, i, i, i, 0};//´´½¨head_row
        for(int i = N3 + 1; i <= tot; i++)l[i] = (node){i, i, i - 1, i + 1, 0, i};//´´½¨head_col
        l[N3 + 1].l = l[tot].r = 0;
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                for(int k = 1; k <= N; k++){
                    ins(++tot, i * N2 + j * N + k, N3 + 1 + i * N + j);//(i,j)ÒÑ¾­ÌîÊý
                    ins(++tot, i * N2 + j * N + k, N3 + N2 + i * N + k);//iÐÐÒÑ¾­Ìîk
                    ins(++tot, i * N2 + j * N + k, N3 + N2 * 2 + j * N + k);//jÁÐÒÑ¾­Ìîk
                    ins(++tot, i * N2 + j * N + k, N3 + N2 * 3 + (i / 3 * 3 + j / 3) * N + k);//(i,j)¹¬ÒÑ¾­Ìîk
                }
            }
        }
    }

    bool init(){
        memset(col_cnt, 0, sizeof(col_cnt));
        memset(sol, 0, sizeof(sol));
        ans = 0;
        build();
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                if(a[i][j]){
                    int row = i * N2 + j * N + a[i][j];
                    if(l[row].r == row)return 0;
                    for(int c = l[row].r; c != row; c = l[c].r){//¸ÃÐÐµÚcÁÐ
                        for(int k = l[c].u; k != c; k = l[k].u){//cÁÐÇë¿Õ
                            if(l[k].col == k)del(k);
                            else {
                                while(l[l[k].row].r != l[k].row)del(l[l[k].row].r);
                            }
                        }
                    }
                    while(l[row].r != row)del(l[row].r);
                }
            }
        }
        return 1;
    }

    void solve(int dep = 1){
        if(l[0].r == 0){
            sol[ans][0] = dep - 1;
            ans++;
            return;
        }
        int mn = N3, c;
        for(int i = l[0].r; i; i = l[i].r){
            if(col_cnt[i] < mn){
                mn = col_cnt[i];
                if(mn <= 0)return;
                c = i;
            }
        }

        stack<int> s;
        for(int i = l[c].d; i != c; i = l[i].d){//Ñ¡ÔñÄÄÒ»ÐÐ
            int row = l[i].row;
            sol[ans][dep] = row;
            for(int j = l[row].r; j != row; j = l[j].r){//¸ÃÐÐµÚjÁÐ
                for(int k = l[j].u; k != j; k = l[k].u){//jÁÐÇë¿Õ
                    if(l[k].col == k){//ÁÐÊ×
                        s.push(k);
                        del(k);
                    }
                    else{
                        int del_row = l[k].row;
                        while(l[del_row].r != del_row){
                            s.push(l[del_row].r);
                            del(l[del_row].r);
                        }
                    }
                }
            }
            while(l[row].r != row){
                s.push(l[row].r);
                del(l[row].r);
            }

            solve(dep + 1);

            if(ans == 2)return;

            while(!s.empty()){
                back(s.top());
                s.pop();
            }
        }
    }

    void fill(int *s){
        for(int i = 1; i <= s[0]; i++){
            int x = (s[i] - 1) / N / N, y = (s[i] - 1) / N % N;
            a[x][y] = (s[i] - 1) % N + 1;
        }
    }

    void print(){
        for(int i = 0; i < N; i++){
            for(int j = 0; j < N; j++){
                printf("%d ", a[i][j]);
            }
            printf("\n");
        }
    }

public:
    void output(){
        if(ans == 0)cout << "No_solution" << endl;
        else if(ans == 1){
            cout << "OK" <<endl;
            fill(sol[0]);
            print();
        }
        else if(ans == 2){
            cout << "Multiple_solutions" <<endl;
            fill(sol[0]);
            print();
            cout << endl;
            for(int i = 1; sol[1][i] == 0; i++)sol[1][i] = sol[0][i];
            fill(sol[1]);
            print();
        }
    }
    void sol1(){
        if(ans == 0)cerr << "No solution1!" << endl;
        else fill(sol[0]);
    }
    void sol2(){
        if(ans <= 1)cerr << "No solution2!" << endl;
        else fill(sol[1]);
    }
    DLX(int b[N][N]){
        memcpy(a, b, sizeof(a));
        if(init())solve();
    }
};

///function: void build(void): Build a sudoku with random numbers and show it on the screen, making
///                            sure the sudoku it builds has only one solution.
class BWR{
private:
    int sudoku[9][9] = {{0}};///to store the sudoku it built

    int static pos2block(int x, int y){///transfer position to block index
        return x / 3 * 3 + y / 3;
    }

    void build_raw_sudoku(){///build a sudoku with 25 numbers
        int g = 0;
        bool x[9][10] = {{false}}, y[9][10] = {{false}}, block[9][10] = {{false}};///to store whether the number has been in the part
        while(g < 25){///fill in 25 numbers
            int tmp = rand() % 9 + 1;///get random number
            int posx = rand() % 9, posy = rand() % 9;///get random position
            if(sudoku[posx][posy] == 0 && !x[posx][tmp] && !y[posy][tmp] && !block[pos2block(posx, posy)][tmp]){///if not in the part
                sudoku[posx][posy] = tmp;///fill the number in the blank in sudoku
                x[posx][tmp] = true;///mark that this number has been in the part
                y[posy][tmp] = true;
                block[pos2block(posx, posy)][tmp] = true;
                g ++;
            }
        }
    }

    void next_sudoku(DLX * x){///fill 3 more numbers in the sudoku
        int g = 0;
        x->sol1();
        while(g < 3){
            int posx = rand() % 9, posy = rand() % 9;
            if(sudoku[posx][posy] == 0){
                sudoku[posx][posy] = x->a[posx][posy];
                g ++;
            }
        }
    }

    void show(){///show the sudoku on the screen as required
        for(int i = 0; i < 9; i ++){
            for(int j = 0; j < 9; j ++){
                if(sudoku[i][j] == 0){
                    cout << '-' << ' ';
                } else {
                    cout << sudoku[i][j] << ' ';
                }
            }
            cout << endl;
        }
    }

public:
    void build(){///build a sudoku
        do{
            memset(sudoku, 0, sizeof(sudoku));///clear sudoku
            build_raw_sudoku();///get a sudoku with 25 numbers
            DLX * x = new DLX(sudoku);///solve the sudoku
            if(x->ans == 1){///if there is only one solution
                show();
                delete x;
                return;
            }
            int g = 4;
            while(x->ans == 2 && g --){///repeat until there is only one solution
                next_sudoku(x);///fill in 3 more numbers
                * x = DLX(sudoku);///solve the sudoku again
                if(x->ans == 1){
                    show();
                    delete x;
                    return;
                }
            }
        }while(true);///loop until return
    }

};

///Get a command from keyboard, and return it as an integer.
int get_command(){
    int command;
    cin >> command;
    return command;
}

///Get a sudoku, and transfer it into number-only sudoku.
void input(int sudoku[9][9]){
    char tmp;
    for(int i = 0; i < 9; i ++){
        for(int j = 0; j < 9; j ++){
            cin >> tmp;
            if(tmp == '-'){
                sudoku[i][j] = 0;
            } else {
                sudoku[i][j] = tmp - '0';
            }
        }
    }
}

///Use ways provided in class DLX to solve the sudoku and show the solution(s) as required.
void solve(){
    int sudoku[9][9];
    input(sudoku);
    DLX * x = new DLX(sudoku);
    x->output();
    delete x;
}

///Use ways provided in class BWR to build a sudoku as required.
void build(){
    BWR * x = new BWR;
    x->build();
    delete x;
}

int main(){
    srand(time(nullptr));
    int command = get_command();
    if(command == 1){
        build();
    }
    else if(command == 2){
        solve();
    }
    else{
        cout << "Invalid Input!" << endl;
    }

    return 0;
}
