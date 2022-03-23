#include <iostream>

#include "chess_board.h"
using namespace std;

std::string solveNQueens(int n, std::pair<int, int> mandatoryQueenCoordinates)
{
    Chessboard chessboard(n);
    if (n >= 50) {
        return  chessboard.Decision(mandatoryQueenCoordinates); //chessboard.PrintHardDecision(mandatoryQueenCoordinates);
    }
    else {
        return  chessboard.Block5Decision(mandatoryQueenCoordinates);
    }
}


int main()
{
    cout << solveNQueens(128, { 38,4 }) << endl;
    system("pause");
}