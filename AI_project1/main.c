#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// for the representation in the 3x3 board
#define EMPTY ' '
#define PLAYER1 'X'
#define PLAYER2 'O'


void print_board (const char board[3 * 3]){
    printf("=============\n");

    for (int i = 0; i < 3; i++) {
        printf("| %c | %c | %c |\n", board[i * 3], board[i * 3 + 1], board[i * 3 + 2]);

        if (i != 2){
            printf("----+---+----\n");
        } else  {
            printf("=============\n");
        }
    }
}


char check_winner (const char board[3 * 3]){
    int winning_combos[8][3] = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8},   //for the rows
            {0, 3, 6}, {1, 4, 7}, {2, 5, 8},   //for the columns
            {0, 4, 8}, {2, 4, 6}                               //and the 2 diagonals!!
    };

    for (int i = 0; i < 8; i++) {
        int x = winning_combos[i][0];
        int y = winning_combos[i][1];
        int z = winning_combos[i][2];

        if (board[x] != EMPTY && board[x] == board[y] && board[x] == board[z]){
            return board[x];
        }
    }

    for (int i = 0; i < 9; i++) {
        if (board[i] == EMPTY){
            return 0;
        }
    }

    return 'T';
}


int evaluate(const char board[3 * 3]){
    char winner = check_winner(board);

    if (winner == PLAYER1){
        return 10;
    } else if (winner == PLAYER2) {
        return -10;
    }

    return 0;
}


//minimax function with alpha-beta pruning
int minimax(char board[3 * 3], int depth, int alpha, int beta, int maximizing_player){
    char winner = check_winner(board);

    if (winner != 0 || depth == 0){
        return evaluate(board);
    }

    if (maximizing_player){
        int max_eval = INT_MIN; //since we will search for the biggest number available and compare it to something small => we get that number

        for (int i = 0; i < 9; i++) {
            if (board[i] == EMPTY){
                board[i] = PLAYER1; //X makes a move
                int eval_score = minimax(board, depth-1, alpha, beta, 0); //0 for maximizing player since that = False
                board[i] = EMPTY; //we undo the move
                max_eval = (max_eval > eval_score) ? max_eval : eval_score;
                alpha = (alpha > eval_score) ? alpha : eval_score;

                if (beta <= alpha) break; //alpha-beta pruning
            }
        }
        return max_eval;
    }
    else {
        int min_eval = INT_MAX; // we will search for the smallest number and compare it to something big

        for (int i = 0; i < 9; i++) {
            if (board[i] == EMPTY){
                board[i] = PLAYER2; //O makes a move
                int eval_score = minimax(board, depth-1, alpha, beta, 1);
                board[i] = EMPTY; //undo the move
                min_eval = (min_eval > eval_score) ? eval_score : min_eval;
                beta = (beta > eval_score) ? eval_score : beta;

                if (beta <= alpha) break;
            }
        }
        return min_eval;
    }
}


//function to find the best move using minimax with alpha-beta pruning
int find_best_move(char board[3 * 3]){
    int best_score = INT_MAX;
    int best_move = -1;

    for (int i = 0; i < 9; i++) {
        if (board[i] == EMPTY){
            board[i] = PLAYER2;
            int move_score = minimax(board, 9, INT_MIN, INT_MAX, 1);
            board[i] = EMPTY;

            if (move_score < best_score){
                best_score = move_score;
                best_move = i;
            }
        }
    }

    return best_move;
}


//counting how many cells are left to fill
int filled_cells(const char board[3 * 3]){
    int count = 0;

    for (int i = 0; i < 9; i++) {
        if (board[i] != EMPTY){
            count++;
        }
    }

    return count;
}


int main(){
    char board[3 * 3] = {EMPTY, EMPTY, EMPTY,
                         EMPTY, EMPTY, EMPTY,
                         EMPTY, EMPTY, EMPTY};

    while (1){
        system("cls");
        print_board(board);
        char winner = check_winner(board);

        if (winner != 0){
            if (winner == 'T'){
                printf("It's a tie!\n");
                system("pause");
            } else {
                printf("Player %c wins!\n", winner);
                system("pause");
            }
            break;
        }

        int move;
        if (filled_cells(board) % 2 == 0){ //PLAYER X'S TURN
            while (1){
                printf("Enter X's move (1-9): ");
                scanf("%d", &move);

                if (move > 0 && move < 10 && board[move -1] == EMPTY){
                    board[move - 1] = PLAYER1;
                    break;
                }
                else {
                    printf("Invalid move! Try again. \n");
                }
            }
        }
        else { //AI TURN (PLAYER O)
            move = find_best_move(board);
            board[move] = PLAYER2;

        }
    }
}