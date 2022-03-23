#include <iostream>

#include "chess_board.h"
using namespace std;

std::string solveNQueens(int n, std::pair<int, int> mandatoryQueenCoordinates)
{
    Chessboard chessboard(n);
    return  chessboard.Decision(mandatoryQueenCoordinates); //chessboard.PrintHardDecision(mandatoryQueenCoordinates);
}


int main()
{
    cout << solveNQueens(70, { 0,0 }) << endl;
    system("pause");
}