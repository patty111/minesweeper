#include <iostream>
using namespace std;

int EDGE;
int MINES;
char** ACTUAL_BOARD;
char** GUESS_BOARD;

void print_board(char** board){
    for (int i=0; i<EDGE; ++i){
        for (int j=0; j<EDGE; ++j){
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}



int main(){
    int level;
    cout << "Choose the Difficulty:\n";
    cout << "1 for EASY (9 * 9 map and 12 mines)\n";
    cout << "2 for MEDIUM (16 * 16 map and 40 mines)\n";
    cout << "3 for HARD (25 * 25 map and 100 mines)\n";

    cin >> level;

    switch (level)
    {
    case 2:
        MINES = 40;
        EDGE = 16;
        break;
    case3:
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
        for (int j=0; j<EDGE; ++j){
            ACTUAL_BOARD[i][j] = '-';
        }
    }

    print_board(ACTUAL_BOARD);


    return 0;
}