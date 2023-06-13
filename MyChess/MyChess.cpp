#include <cmath>
#include <iostream>
#include <vector>
#include <map>



//цвет
enum Color
{
    white = 'w',
    black = 'b'
};

//тип фигуры
enum FigureType
{
    Pawn = 'P', //пешка
    Bishop = 'B', //слон
    Queen = 'Q', //ферзь
    King = 'K', //король
    Horse = 'H', //конь
    Rook = 'R', //ладья
    Empty = 'E' //нет фигуры
};

//проверка координаты, лежит ли она в заданном диапозоне
bool CheckCoord(int coord, int limit)
{
    return (coord >= 0 && coord < limit);
}


//класс фигуры
class Figure
{
private:
    Color color; // цвет фигуры
    FigureType type; //тип фигуры

public:
    //конструктор 
    Figure(FigureType figuretype) : color(white), type(figuretype) {}

    //дружественная функция (перегрузка <<) для вывода типа фигуры
    friend std::ostream& operator << (std::ostream& out, const Figure& figure)
    {
        char a = figure.type; //
        return out << a;
    }

    //проверка типа фигуры
    bool IsA(FigureType t) const
    {
        return t == type;
    }

    char CharGetTypeFigure() const
    {
        return (char)(type);
    }
};



//класс клетки
class Cell
{
private:
    Color color; // цвет клетки
    Figure figure; // фигура, стоящая на клетке
public:
    //конструктор по умолчанию
    Cell() : color(white), figure(Figure(Empty)) {}
    Cell(Color _colorCell, FigureType fig) : color(_colorCell), figure(Figure(fig)) {}
    //перегрузка оператора <<
    friend std::ostream& operator << (std::ostream& out, const Cell& cell)
    {
        //если клетка пуста вывести цвет
        char a = cell.color;
        cell.figure.IsA(Empty) ? out << a : out << cell.figure;
        return out;
    }

    char CellFigure() const
    {
        return figure.CharGetTypeFigure();
    }

};


//структура для доски
class Board
{
private:
    // доска представленная в виде вектора
    std::vector<Cell> cells;
    int size; // число столбцов

    //псевдокласс нужен для возможности обращения по [][]
    class Proxy
    {
        const std::vector<Cell>& cells;
        int d;
    public:
        Proxy(int h, const std::vector<Cell>& cells_) : d(h), cells(cells_) {}


        const Cell& operator[](int j) const
        {
            return cells[d * sqrt(cells.size()) + j];
        }
    };

public:
    int Size()
    {
        return size;
    }
    std::map<int, char > Position()
    {
        std::map<int, char > position;
        for (int i = 0; i < cells.size(); i++)
        {
            if (cells[i].CellFigure() != 'E')
            {
                position.insert({ i, cells[i].CellFigure() });
            }
        }
        return position;
    }

    std::vector<Cell>::iterator begin() {
        return cells.begin();
    }

    std::vector<Cell>::iterator end() {
        return cells.end();
    }
    Board(int bsize); //конструктор по умолчанию

//псевдокласс нужен для возможности обращения по [][]
// const Proxy operator[] (int i) const;

    const Proxy operator[] (int i) const

    {
        Proxy proxy = Proxy(i, cells);
        return proxy;
    }

    friend std::ostream& operator << (std::ostream& out, const Board& board); // печать только доски, пока занятых клеток не печатает
    
    void Set(int coord, Figure figure)
    {
        if ((coord/size + coord % size) % 2 == 0)
        {
            cells[coord] = Cell(black, (FigureType)(figure.CharGetTypeFigure()));
        }
        else
        {
            cells[coord] = Cell(white, (FigureType)(figure.CharGetTypeFigure()));
        }        
    }

    void Remove(int coord, Figure figure)
    {
        if ((coord / size + coord % size) % 2 == 0)
        {
            cells[coord] = Cell(black, Empty);
        }
        else
        {
            cells[coord] = Cell(white, (FigureType)(figure.CharGetTypeFigure()));
        }
    }
};

void PrintPosition(std::map<int, char> PositionList) {
    std::map <int, char> ::iterator it = PositionList.begin();
    for (; it != PositionList.end(); it++) {
        int num = ((it->first) % 8) + 1;
        int letter = (it->first) / 8;
        std::cout << (char)(letter + 65);
        std::cout << num << "\n";
    }
}

//конструктор по умолчанию для доски
Board::Board(int bsize) : size(bsize)
{
    // заполнение самой доски
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // переделать в тенарный оператор, когда убедимся, что тут нет ошибок
            if ((i + j) % 2 == 0)
            {
                cells.push_back(Cell(black, Empty));
            }
            else
            {
                cells.push_back(Cell(white, Empty));
            }
        }
    }
 //   cells[0] = Cell(black, Queen);
}

// печать только доски
std::ostream& operator << (std::ostream& out, const Board& board)
{
    out << "\n";
    for (int i = board.size - 1; i >= 0; i--)
    {
        //вывод цифр
        out << i + 1 << ' ';
        for (int j = 0; j < board.size; j++)
        {
            out << ' ' << board.cells[i + board.size * j];
        }
        out << "\n";
    }
    out << "   ";
    //вывод букв
    for (char letter = 65; letter <= 72; letter++)
    {
        out << letter << ' ';
    }
    out << "\n";
    return out;
}

class GameRules
{
public:
 //   virtual bool CheckMove(Figure figure, int coord1, int coord2) = 0;
    virtual bool CheckSet(Figure figure, int coord) = 0;
 //   virtual bool CheckRemove(Figure figure, int coord) = 0;
};

class Queens : public GameRules
{
private:
    Board board;
public:

    Queens(Board &board_) : GameRules(), board(board_) {}

    // поставить фигуру
    bool CheckSet(Figure figure, int coord) override
    {           
        int size = board.Size();
        if (figure.IsA(Queen) && CheckCoord(coord, size * size))
        {
 
            const std::map<int, char > &position = board.Position();
            std::cout << '\n' <<"pjoo" << board;
            std::cout << '\n' << &board;
 //           std::map <int, char> ::iterator it = position.begin();
 //           for (; it != position.end(); it++)
            for(auto it: position)
            {
                std::cout << "yu";
                if (it.first / size == coord / size || it.first % size == coord % size ||
                    (abs(it.first / size - coord / size) == abs(it.first % size - coord % size)))
                {
                    return false;
                }
            }
            board.Set(coord, figure);
            return true;
        }
        else
        {
            return false;
        }
        
        //return true;
    }
    
/* пока не работает
    bool CheckRemove(Figure figure, int coord) override
    {

        return true;
    }

    //переместить фигуру
    bool CheckMove(Figure figure, int coord1, int coord2) override
    {
        int size = board.Size();
        if (figure.IsA(Queen) && CheckCoord(coord1, size * size) && CheckCoord(coord2, size * size) && (coord1 != coord2))
        {
            if (board[coord1 / size][coord1 % size].CellFigure() == 'Q' && board[coord2 / size][coord2 % size].CellFigure() == 'E')
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    */
};


class Chess : public GameRules
{
private:
    Board board;

public:

    Chess(Board& board_) : GameRules(), board(board_) {}
    // поставить фигуру
    bool CheckSet(Figure figure, int coord) override
    {
        int size = board.Size();
        if (figure.IsA(Queen) && CheckCoord(coord, size * size))
        {
            board.Set(coord, figure);
            return true;
        }
    }
};

class Match
{
    //создать конструктор
private:
    GameRules* gr;
    Board board;
public:

    Match(GameRules* gr_, Board &board_): gr(gr_), board(board_)
    {        
    }    

    void Set(int coord, Figure figure)
    {
        if (gr->CheckSet(figure, coord))
        {
            board.Set(coord, figure);
        }
    }

    //вывод доски
    void Print()
    {
        std::cout << board;
        std::cout <<'\n'<< &board;
    }
    static Match* CreateQueens(Board &board)
    {
        GameRules* queens = new Queens(board);
        Match* match = new Match(queens, board);
        return match;
    }    
    static Match* CreateChess(Board& board)
    {
        GameRules* chess = new Chess(board);
        Match* match = new Match(chess, board);
        return match;
    }
    // нужен будет еще конструктор копирования и оператор присваивания
    ~Match()
    {
        delete gr;
    }
};

int main()
{
    Board board = Board(8);
 //   std::cout << board;
 //   std::cout << &board;
 //   board.Set(9, Figure(Queen));
    /*
    Cell a = board[1][0];
    std::cout << a;
    char checksymvol = board[1][0].CellFigure();
    std::cout << checksymvol << '\n';
    std::map<int, char> Dictionary = board.Position();
    PrintPosition(Dictionary);
    Queens queens(board);
    bool q = queens.CheckSet(Figure(Queen), 19);
    std::cout << q << '\n';
    q = queens.CheckMove(Figure(Queen), 0, 19);
    std::cout << q;
    */
    Match * queens = Match::CreateQueens(board);
 //   queens->Set(65, Figure(Queen));
    queens->Set(0, Figure(Queen));
    queens->Set(1, Figure(Queen));
    queens->Set(10, Figure(Queen));
    queens->Set(1, Figure(Queen));
    queens->Set(100, Figure(Queen));

    Match* chess = Match::CreateChess(board);
    chess->Set(0, Figure(Queen));
    chess->Set(1, Figure(Queen));
    chess->Set(10, Figure(Queen));
    chess->Set(1, Figure(Queen));
    chess->Set(100, Figure(Queen));
    queens->Print();
    chess->Print();
 //   std::cout << board;
    return 0;
}