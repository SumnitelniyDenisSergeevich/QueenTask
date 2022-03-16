#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <optional>
#include <set>
using namespace std;

namespace nQueens
{
    struct chessboard_map {
        std::vector<std::vector<bool>> map;
        size_t queen_count = 0;
    };
    //using chessboard_map = std::vector<std::vector<uint16_t>>;//(y,x)

    class Chessboard {
    public:
        Chessboard(std::size_t chessboard_size);

        string PrintHardDecision(std::pair<int, int> mandatoryQueenCoordinates);
    private:
        chessboard_map chessboard_map_;      //в конструкторе заполняется еденицами(true), по дефолту нулями;
        std::optional<chessboard_map> result_; // здесь храним удачные результаты

        void AddFirstQueen(size_t col, size_t row);

        void AddQueen(size_t row, size_t col, std::set<size_t>& rows, std::set<size_t>& cols);

        bool CheckDiagonalFields(size_t x, size_t y) const;
        bool CheckTopLeftDiagonalFields(int x, int y) const;
        bool CheckTopRightDiagonalFields(int x, int y) const;
        bool CheckDownLeftDiagonalFields(int x, int y) const;
        bool CheckDownRightDiagonalFields(size_t x, size_t y) const;
    };

    Chessboard::Chessboard(size_t size) {
        chessboard_map_.map.resize(size); // создали chessboard_size строк(y)
        for (vector<bool>& x : chessboard_map_.map) { // создали chessboard_size столбцов(x)
            x.resize(size, false);    //заполнили false
        }
    }

    //void PrintChess(const chessboard_map& chessboard) {
    //    for (const auto& y : chessboard.map) {
    //        for (const auto& x : y) {
    //            cout << x << " ";
    //        }
    //        cout << endl;
    //    }
    //    cout << endl;
    //}

    string Chessboard::PrintHardDecision(std::pair<int, int> mandatoryQueenCoordinates) {
        try {
            AddFirstQueen(mandatoryQueenCoordinates.first, mandatoryQueenCoordinates.second);
        }
        catch (runtime_error) {

        }
        string result = "";
        if (result_) {
            cout << "hoho" << endl;
            for (const auto& y : result_->map) {
                for (const auto& x : y) {
                    if (x) {
                        result += "Q";
                    }
                    else {
                        result += ".";
                    }
                }
                result += '\n';
            }
        }
        return result;
    }

    void Chessboard::AddFirstQueen(size_t col, size_t row) {
        std::set<size_t> rows, cols;
        for (size_t i = 0; i < chessboard_map_.map.size(); ++i) {
            rows.insert(i);
            cols.insert(i);
        }

        AddQueen(row, col, rows, cols);
    }

    void Chessboard::AddQueen(size_t row, size_t col, std::set<size_t>& rows_, std::set<size_t>& cols_) {
        if (!CheckDiagonalFields(col, row)) {
            return;
        }
        chessboard_map_.map[row][col] = true;// Cтавим Ферзя
        ++chessboard_map_.queen_count;
        if (chessboard_map_.queen_count == chessboard_map_.map.size()) {
            result_ = chessboard_map_;
            throw runtime_error("lol");
        }

        std::set<size_t> rows = rows_, cols = cols_;
        rows.erase(row);
        cols.erase(col);
        ////print--------------------------------------------
        //cout << endl;
        //cout << "rows: ";
        //for (const size_t row : rows) {
        //    cout << row << ", ";
        //}
        //cout << endl;
        //cout << "cols: ";
        //for (const size_t col : cols) {
        //    cout << col << ", ";
        //}
        //cout << endl;
        //for (const auto& y : chessboard_map_.map) {
        //    for (const auto& x : y) {
        //        cout << x << " ";
        //    }
        //    cout << endl;
        //}
        //cout << endl;
        ////endprint--------------------------------------------
        //system("pause");
        for (const size_t row : rows) {
            for (const size_t col : cols) {
                AddQueen(row, col, rows, cols);
            }
        }
        --chessboard_map_.queen_count;
        chessboard_map_.map[row][col] = false;
    }


    bool Chessboard::CheckDiagonalFields(size_t x, size_t y) const {
        bool ch1 = CheckTopLeftDiagonalFields(x, y);
        bool ch2 = CheckTopRightDiagonalFields( x, y);
        bool ch3 = CheckDownLeftDiagonalFields(x, y);
        bool ch4 = CheckDownRightDiagonalFields(x, y);
        return (ch1 && ch2 && ch3 && ch4);
    }

    bool Chessboard::CheckTopLeftDiagonalFields(int x, int y) const {
        while ((x -= 1) >= 0 && (y -= 1) >= 0) {
            if (chessboard_map_.map.at(y).at(x)) {
                return false;
            }
        }
        return true;
    }

    bool Chessboard::CheckTopRightDiagonalFields(int x, int y) const {
        while ((x += 1) < chessboard_map_.map.size() && (y -= 1) >= 0) {
            if (chessboard_map_.map.at(y).at(x)) {
                return false;
            }
        }

        return true;
    }

    bool Chessboard::CheckDownLeftDiagonalFields(int x, int y) const {
        while ((x -= 1) >= 0 && (y += 1) < chessboard_map_.map.size()) {
            if (chessboard_map_.map.at(y).at(x)) {
                return false;
            }
        }
        return true;
    }

    bool Chessboard::CheckDownRightDiagonalFields(size_t x, size_t y) const {
        while (((x += 1) < chessboard_map_.map.size()) && ((y += 1) < chessboard_map_.map.size())) {
            if (chessboard_map_.map.at(y).at(x)) {
                return false;
            }
        }
        return true;
    }

    std::string solveNQueens(int n, std::pair<int, int> mandatoryQueenCoordinates)
    {
        Chessboard chessboard(n);
        return chessboard.PrintHardDecision(mandatoryQueenCoordinates);
    }
}


int main()
{
    cout << nQueens::solveNQueens(10, { 3,3 }) << endl;
    system("pause");
}