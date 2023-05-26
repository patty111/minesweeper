#include <iostream>
#include <random>
using namespace std;

int EDGE;
int MINES;
int dx[] = {1, 0, -1, -1, -1, 0, 1, 1};
int dy[] = {1, 1, 1, 0, -1, -1, -1, 0};
vector<pair<int, int> > mine_loc;

char** ACTUAL_BOARD;
char** GUESS_BOARD;


void init(){
    int level;
    cout << "Choose the Difficulty:\n";
    cout << "1 for EASY (9 * 9 map and 12 mines)\n";
    cout << "2 for MEDIUM (16 * 16 map and 40 mines)\n";
    cout << "3 for HARD (25 * 25 map and 100 mines)\n: ";

    cin >> level;

    switch (level){
    case 2:
        MINES = 40;
        EDGE = 16;
        break;
    case 3:
        MINES = 100;
        EDGE = 25;
        break;
    default:
        MINES = 12;
        EDGE = 9;
        break;
    }


    ACTUAL_BOARD = new char*[EDGE];
    for (int i=0; i<EDGE; ++i){
        ACTUAL_BOARD[i] = new char[EDGE];
        for (int j=0; j<EDGE; ++j)
            ACTUAL_BOARD[i][j] = '-';
    }


    GUESS_BOARD = new char*[EDGE];
    for (int i=0; i<EDGE; ++i){
        GUESS_BOARD[i] = new char[EDGE];
        for (int j=0; j<EDGE; ++j){
            GUESS_BOARD[i][j] = '-';
        }
    }
}


void mine_generate(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, EDGE - 1);

    int placed = 0;
    while (placed < MINES){
        int x = dis(gen);
        int y = dis(gen);
        mine_loc.push_back(make_pair(x, y));

        if (ACTUAL_BOARD[x][y] == '-'){
            ACTUAL_BOARD[x][y] = '*';
            ++placed;
        }
    }
}


bool is_mine(int r, int c){
    if (ACTUAL_BOARD[r][c] == '*')
        return true;
    return false;
}

bool is_valid(int r, int c){
    if (r >= 0 && r < EDGE && c >= 0 && c < EDGE)
        return true;
    return false;
}


void print_board(char** board){
    for (int i=0; i<EDGE; ++i){
        for (int j=0; j<EDGE; ++j){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}


int count_adjacent_mines(int r, int c){
    

    int adj_mine_count = 0;
    for (int i=0; i<8; ++i){
        if (is_valid(r+dx[i], c+dy[i]) && is_mine(r+dx[i], c+dy[i]))
            adj_mine_count++;
    }
    return adj_mine_count;
}


void reveal_mines(){
    
}

// recursively check adjacent cells whether should reveal
void reveal_cell(int r, int c){
    if (GUESS_BOARD[r][c] != '-')
        return;
    if (ACTUAL_BOARD[r][c] == '*'){
        reveal_mines();
        cout << "You lost!\n";
        return;
    }

    int count = count_adjacent_mines(r, c);
    GUESS_BOARD[r][c] = '0' + count;

    if (count != 0){
        for (int i=0; i<8; ++i){
            int new_r = r + dx[i];
            int new_c = c + dy[i];
            if (is_valid(new_r, new_c) && !is_mine(new_r, new_c))
                reveal_cell(new_r, new_c);
        }
    }
    return;
} 



int main(){

    init();
    mine_generate();
    print_board(ACTUAL_BOARD);
    reveal_cell(5, 2);
    print_board(GUESS_BOARD);

    return 0;
}