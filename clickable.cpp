#include <iostream>
#include <algorithm>
#include <random>
#include <stdio.h>
#include <termios.h>
using namespace std;

// ansi escape
#define BLUE1 "\033[38;5;12m"
#define GREEN2 "\033[38;5;82m"
#define RED3 "\033[38;5;196m"
#define DARK_BLUE4 "\033[38;5;21m"
#define BROWN5 "\033[38;5;88m"
#define CYAN6 "\033[38;5;14m"
#define BLACK7 "\033[38;5;16m"
#define GREY8 "\033[38;5;247m"

#define FLAG "\033[38;5;226m"
#define MINE "\033[38;5;207m"
#define COLOR_RESET "\033[0m"
#define BACKGROUND "\033[48;5;236m"


int EDGE, MINES;
bool KABOOM = false;

vector<pair<int, int> > find_all;
vector<pair<int, int> > mine_loc;
int dx[] = {1, 0, -1, -1, -1, 0, 1, 1};
int dy[] = {1, 1, 1, 0, -1, -1, -1, 0};

char** ACTUAL_BOARD;
char** GUESS_BOARD;

void enableTerminalEcho(bool enable) {
    struct termios term;
    tcgetattr(fileno(stdin), &term);

    if (enable) {
        term.c_lflag |= ECHO;
    } else {
        term.c_lflag &= ~ECHO;
    }

    tcsetattr(fileno(stdin), TCSANOW, &term);
}


void init(){
    int level;
    cout << "Choose the Difficulty:\n";
    cout << "1 for EASY (9 * 9 map and 12 mines)\n";
    cout << "2 for MEDIUM (16 * 16 map and 40 mines)\n";
    cout << "3 for HARD (25 * 25 map and 100 mines)\n: ";

    cin >> level;

    switch (level){
    case 2:
        MINES = 38;
        EDGE = 16;
        break;
    case 3:
        MINES = 95;
        EDGE = 25;
        break;
    case 100:
        MINES = 1;
        EDGE = 4;
        break;
    default:
        MINES = 11;
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
    for (int i=0; i<EDGE; ++i){
        // printf("%2d", i);
        for (int j=0; j<EDGE; ++j){
            switch (board[i][j]){
            case 'F':
                cout << FLAG;
                break;
            case '*':
                cout << MINE;
                break;
            case '1':
                cout << BLUE1;
                break;
            case '2':
                cout << GREEN2;
                break;
            case '3':
                cout << RED3;
                break;
            case '4':
                cout << DARK_BLUE4;
                break;
            case '5':
                cout << BROWN5;
                break;
            case '6':
                cout << CYAN6;
                break;
            case '7':
                cout << BLACK7;
                break;
            case '8':
                cout << GREY8;
                break;
            default:
                break;
            }

            cout << BACKGROUND;
            printf("%c ", board[i][j]);
            cout << COLOR_RESET;
        }
        cout << endl;
    }
    cout << "    " << MINES - find_all.size() << " left";
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
    GUESS_BOARD[r][c] = count != 0 ? '0' + count : ' ';
    

    if (count == 0){
        for (int i=0; i<8; ++i){
            int new_r = r + dx[i];
            int new_c = c + dy[i];
            if (is_valid(new_r, new_c) && !is_mine(new_r, new_c))
                reveal_cell(new_r, new_c);
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
    if (GUESS_BOARD[r][c] == 'F') {
        GUESS_BOARD[r][c] = '#';
        auto it = std::find(find_all.begin(), find_all.end(), tmp);
        if (it != find_all.end()) {
            find_all.erase(it);
        }
    }
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
    printf("\033[?1000h"); // disable mouse support
    print_board(GUESS_BOARD);

    while (true){
        enableTerminalEcho(false);

        char input[32];
        fgets(input, sizeof(input), stdin); // read input from stdin
        
        if (check_find_all() == true){
            cout << "\033[48;5;42m" << "You win !" << COLOR_RESET << endl;
            break;
        }  

        
        int button, row, col;
        if (input[0] == '\033' && input[1] == '[' && input[2] == 'M') {
            sscanf(input, "\033[<%d;%d;%d", &button, &row, &col); // extract button number, column, and row

            button = input[3] - ' ';
            col = (int)((input[4] - ' ' - 1) / 2);
            row = input[5] - ' ' - 1 - EDGE - 3; // minus the edge and the \n for foramtting
            // cout << "row: " << row << " col:" << col << endl;
        }
        if (button == 2){    // if press right mouse, set flag
            flag(row, col);
        }
        else if (row >= 0 && col >= 0 && row <= EDGE-1 && col <= EDGE-1){
            reveal_cell(row, col);
            if (KABOOM){
                cout << "\033[48;5;200m" << "You lost!" << COLOR_RESET << endl;
                break;
            }
        }
        else
            continue;

        enableTerminalEcho(true);
        print_board(GUESS_BOARD);
    }
}



int main(){
    init();
    mine_generate();
    run_mine_sweeper();
    cout << "\033[?1000l"; // disable mouse support

    enableTerminalEcho(true);    
    return 0;
}