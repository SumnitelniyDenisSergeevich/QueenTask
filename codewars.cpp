#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <optional>

using namespace std;

namespace nQueens
{
    struct chessboard_map {
        std::vector<std::vector<uint16_t>> map;
        size_t ferz_count = 0;
    };
    //using chessboard_map = std::vector<std::vector<uint16_t>>;//(y,x)

    inline bool operator< (const chessboard_map& lhs, const chessboard_map& rhs);

    class Chessboard {
    public:
        Chessboard(std::size_t chessboard_size);

       // bool SimpleDecision();
        string PrintHardDecision(std::pair<int, int> mandatoryQueenCoordinates);
    private:
        chessboard_map chessboard_map_;      //в конструкторе заполняется еденицами(true), по дефолту нулями;
        std::optional<chessboard_map> result_; // здесь храним удачные результаты

        void AddMagaraja(chessboard_map initial_chessboard_map, size_t x, size_t y);

        void CloseTargetFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const;

        void CloseVerticalAndHorizontalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const;
        void CloseDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const;
        void CloseTopLeftDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const;
        void CloseTopRightDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const;
        void CloseDownLeftDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const;
        void CloseDownRightDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const;
    };

    Chessboard::Chessboard(size_t chessboard_size) {
        chessboard_map_.map.resize(chessboard_size); // создали chessboard_size строк(y)
        for (vector<uint16_t>& x : chessboard_map_.map) { // создали chessboard_size столбцов(x)
            x.resize(chessboard_size, 1);    //заполнили true
        }
    }

    /*bool Chessboard::SimpleDecision() {
        size_t chessboard_size = chessboard_map_.map.size();
        if (chessboard_size <= 8) {
            return true;
        }
        return false;
    }*/

    void PrintChess(const chessboard_map& chessboard) {
        for (const auto& y : chessboard.map) {
            for (const auto& x : y) {
                cout << x << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    string Chessboard::PrintHardDecision(std::pair<int, int> mandatoryQueenCoordinates) {
        try {
            AddMagaraja(chessboard_map_, mandatoryQueenCoordinates.first, mandatoryQueenCoordinates.second);
        }
        catch (runtime_error) {

        }
        string result = "";
        if (result_) {
            for (const auto& y : result_->map) {
                for (const auto& x : y) {
                    if (x == 2) {
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

    void Chessboard::AddMagaraja(chessboard_map initial_chessboard_map, size_t x, size_t y) { // Здесь мы КОПИРУЕМ initial_chessboard
        CloseTargetFields(initial_chessboard_map, x, y);
        initial_chessboard_map.map[y][x] = 2;// Cтавим Ферзя
        ++initial_chessboard_map.ferz_count;
        if (initial_chessboard_map.ferz_count == chessboard_map_.map.size()) {
            result_ = initial_chessboard_map;
            throw runtime_error("go");
        }
        ////print--------------------------------------------
        //for (const auto& y : initial_chessboard_map.map) {
        //    for (const auto& x : y) {
        //        cout << x << " ";
        //    }
        //    cout << endl;
        //}
        //cout << endl;
        ////endprint--------------------------------------------
        //system("pause");

        for (size_t i = 0; i < initial_chessboard_map.map.size(); ++i) {//y
            for (size_t j = 0; j < initial_chessboard_map.map.size(); ++j) {//x
                if (initial_chessboard_map.map[i][j] == 1) {
                    AddMagaraja(initial_chessboard_map, j, i);
                }
            }
        }
    }

    void Chessboard::CloseTargetFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const {
        CloseVerticalAndHorizontalFields(initial_chessboard_map, x, y);
        CloseDiagonalFields(initial_chessboard_map, x, y);
    }


    void Chessboard::CloseVerticalAndHorizontalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const {
        for (size_t i = 0; i < initial_chessboard_map.map.size(); ++i) {
            initial_chessboard_map.map[i][x] = 0;
            initial_chessboard_map.map[y][i] = 0;
        }
    }

    void Chessboard::CloseDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const {
        CloseTopLeftDiagonalFields(initial_chessboard_map, x, y);
        CloseTopRightDiagonalFields(initial_chessboard_map, x, y);
        CloseDownLeftDiagonalFields(initial_chessboard_map, x, y);
        CloseDownRightDiagonalFields(initial_chessboard_map, x, y);
    }

    void Chessboard::CloseTopLeftDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const {
        while ((x != 0) && (y != 0)) {
            initial_chessboard_map.map[y][x] = 0;//левую верхнюю диагональ
            x -= 1;
            y -= 1;
        }
        initial_chessboard_map.map[y][x] = 0;
    }

    void Chessboard::CloseTopRightDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const {
        while ((x < initial_chessboard_map.map.size()) && (y != 0)) {
            initial_chessboard_map.map[y][x] = 0;//заполняет правую верхнюю диагональ
            x += 1;
            y -= 1;
        }

        if (y == 0 && x != initial_chessboard_map.map.size()) {
            initial_chessboard_map.map[y][x] = 0;
        }
    }

    void Chessboard::CloseDownLeftDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const {
        while ((x != 0) && (y < initial_chessboard_map.map.size())) {
            initial_chessboard_map.map[y][x] = 0;//заполняет левую нижнюю диагональ
            x -= 1;
            y += 1;
        }

        if (x == 0 && y != initial_chessboard_map.map.size()) {
            initial_chessboard_map.map[y][x] = 0;
        }
    }

    void Chessboard::CloseDownRightDiagonalFields(chessboard_map& initial_chessboard_map, size_t x, size_t y) const {
        while (((x += 1) < initial_chessboard_map.map.size()) && ((y += 1) < initial_chessboard_map.map.size())) {
            initial_chessboard_map.map[y][x] = 0;//заполняет правую нижнюю диагональ нулями
        }
    }

    std::string solveNQueens(int n, std::pair<int, int> mandatoryQueenCoordinates)
    {
        size_t chessboard_size = n;
        Chessboard chessboard(chessboard_size);
        /*if (chessboard.SimpleDecision()) {
            return "";
        }*/
        return chessboard.PrintHardDecision(mandatoryQueenCoordinates);
    }
}


int main()
{
    cout << nQueens::solveNQueens(16, { 5,7 }) << endl;
    system("pause");
}