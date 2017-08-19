//
//  main.cpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/7/13.
//  Copyright (c) 2013 Anton Logunov. All rights reserved.
//
 
#include "util.hpp"
#include "board.hpp"
#include "solver.hpp"

int main(int argc, const char * argv[])
{
    command_line_parser parser(argv, argc);
    vector<int> peg_values;
    Grid<char> board;
    ReadProblem(cin, peg_values, board);
//    PegJumping jumper;
//    auto moves = jumper.getMoves(peg_values, board);
    
    Board b;
    b.Init(board);
    Solver s;
    auto moves = s.Solve(board);
    
    
    cout << moves.size() << endl;
    for (auto m : moves) {
        cout << m << endl;
    }
    cout.flush();
    
    
    
    
    
    return 0;
}

