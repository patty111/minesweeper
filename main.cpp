#include <iostream>
#include <random>
using namespace std;

int EDGE, MINES;
bool KABOOM = false;

vector<pair<int, int> > find_all;
vector<pair<int, int> > mine_loc;
int dx[] = {1, 0, -1, -1, -1, 0, 1, 1};
int dy[] = {1, 1, 1, 0, -1, -1, -1, 0};

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
    case 100:
        MINES = 3;
        EDGE = 4;
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
            ACTUAL_BOARD[i][j] = '#';
    }


    GUESS_BOARD = new char*[EDGE];
    for (int i=0; i<EDGE; ++i){
        GUESS_BOARD[i] = new char[EDGE];
        for (int j=0; j<EDGE; ++j){
            GUESS_BOARD[i][j] = '#';
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

        if (ACTUAL_BOARD[x][y] == '#'){
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
    // printing row index
    cout << "  ";
    for (int i=0; i<EDGE; ++i)
        printf("%2d ", i);
    cout << endl;

    for (int i=0; i<EDGE; ++i){
        printf("%2d", i);
        for (int j=0; j<EDGE; ++j){
            printf("%2c ", board[i][j]);
        }
        cout << endl;
    }
    cout << endl;
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
    for (auto itr: mine_loc){
        GUESS_BOARD[itr.first][itr.second] = '*';
    }
    print_board(GUESS_BOARD);
}

// recursively check adjacent cells whether should reveal
void reveal_cell(int r, int c){
    if (GUESS_BOARD[r][c] != '#' || KABOOM)
        return;
    if (ACTUAL_BOARD[r][c] == '*'){
        reveal_mines();
        KABOOM = true;
        return;
    }

    int count = count_adjacent_mines(r, c);
    GUESS_BOARD[r][c] = '0' + count;

    if (count == 0){
        for (int i=0; i<8; ++i){
            int new_r = r + dx[i];
            int new_c = c + dy[i];
            if (is_valid(new_r, new_c) && !is_mine(new_r, new_c)){
                reveal_cell(new_r, new_c);
            }
        }
    }
    return;
} 


void flag(int r, int c){
    pair<int, int> tmp = make_pair(r, c);
    if (GUESS_BOARD[r][c] == '#'){
        GUESS_BOARD[r][c] = 'F';
        find_all.push_back(tmp);
        return;
    }
    if (GUESS_BOARD[r][c] == 'F')
        GUESS_BOARD[r][c] = '#';
        find_all.erase(remove(find_all.begin(), find_all.end(), make_pair(r, c)), find_all.end());
}


bool check_find_all(){
    if (find_all.size() != MINES)
        return false;

    for (auto itr: find_all){
        if (is_mine(itr.first, itr.second))
            continue;
        return false;
    }
    return true;
}



void run_mine_sweeper(){
    while (true){
        int r, c;
        print_board(GUESS_BOARD);
        
        if (check_find_all() == true){
            cout << "You win !\n";
            break;
        }  

        cout << "Input a gird (enter -1, -1 for flag): ";
        cin >> r >> c;
        if (r != -1 && c != -1){
            reveal_cell(r, c);
            if (KABOOM){
                cout << "You lost!\n";
                break;
            }
        }
        else{
            cout << "Input flag location: ";
            cin >> r >> c;
            flag(r, c);
        }     
    }
}



int main(){
    init();
    mine_generate();
    run_mine_sweeper();
    return 0;
}