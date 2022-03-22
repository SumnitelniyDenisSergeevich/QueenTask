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
    cout << solveNQueens(30, { 3,3 }) << endl;
    system("pause");
}