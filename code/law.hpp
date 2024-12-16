#ifndef LAW_HPP
#define LAW_HPP

#include "boards.hpp"

class Judge {
public:

    static int IsDisallowed(GomokuBoard board);

    static int CountLiveThree(GomokuBoard board, vector<vector<int>> converted_chess_data);

    static int CountFour(GomokuBoard board, vector<vector<int>> converted_chess_data);

    static int JudgeLongLink(GomokuBoard board, vector<vector<int>> converted_chess_data);

    

};

#endif