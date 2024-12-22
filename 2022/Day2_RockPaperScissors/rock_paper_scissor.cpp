#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <utility>

using namespace std;

bool IsRock(const char &ch)
{
    return 'A' == ch || 'X' == ch;
}

bool IsPaper(const char &ch)
{
    return 'B' == ch || 'Y' == ch;
}

bool IsScissor(const char &ch)
{
    return 'C' == ch || 'Z' == ch;
}

enum class Shapes
{
    kRock,
    kPaper,
    kScissor
};

enum class GameResult
{
    kWin,
    kDraw,
    kLose
};

GameResult GetSingleMoveResult(const char &ch1, const char &ch2)
{
    if (IsRock(ch1))
    {
        if      (IsRock(ch2) ) { return GameResult::kDraw;  }
        else if (IsPaper(ch2)) { return GameResult::kWin;   } // ch2 wins
        else if (IsScissor(ch2)) { return GameResult::kLose; }
    }
    else if (IsPaper(ch1))
    {
        if (IsRock(ch2))        { return GameResult::kLose; }
        else if (IsPaper(ch2))   { return GameResult::kDraw; }
        else if (IsScissor(ch2)) { return GameResult::kWin;  }
    }
    else if (IsScissor(ch1))
    {
        if      (IsRock(ch2)) { return GameResult::kWin;    }
        else if (IsPaper(ch2)) { return GameResult::kLose;   }
        else if (IsScissor(ch2)) { return GameResult::kDraw; }
    }
}

Shapes MoveForRoundTo(const char &oponent_move, const GameResult &game_result)
{
    switch (game_result)
    {
        case GameResult::kWin : {
            if (IsRock(oponent_move))           { return Shapes::kPaper;    }
            else if (IsPaper(oponent_move))     { return Shapes::kScissor;  }
            else if (IsScissor(oponent_move))   { return Shapes::kRock;     }
            break;
        }
        case GameResult::kDraw:
        {
            if (IsRock(oponent_move))           { return Shapes::kRock;     }
            else if (IsPaper(oponent_move))     { return Shapes::kPaper;    }
            else if (IsScissor(oponent_move))   { return Shapes::kScissor;  }
            break;
        }
        case GameResult::kLose :
        {
            if (IsRock(oponent_move))           { return Shapes::kScissor;  }
            else if (IsPaper(oponent_move))     { return Shapes::kRock;     }
            else if (IsScissor(oponent_move))   { return Shapes::kPaper;    }
            break;
        }
    }
}

int main(int argc, const char *args[])
{
    const auto kCharToShapeUmap   = unordered_map<char, Shapes>{
        { 'X', Shapes::kRock },
        { 'Y', Shapes::kPaper },
        { 'Z', Shapes::kScissor }
    };
    const auto kShapeScoreUmap   = unordered_map<Shapes, int>{
        { Shapes::kRock, 1 },
        { Shapes::kPaper, 2 },
        { Shapes::kScissor, 3 }
    }; 
    const auto kOutcomeScoreUmap = unordered_map<GameResult, int>{
        { GameResult::kLose, 0 },
        { GameResult::kDraw, 3 },
        { GameResult::kWin, 6 }
    };
    auto moves = vector<pair<char, char>>{};
    
    for (auto [fin, opponent_move, my_move] = tuple{ ifstream{ args[1] }, '\0', '\0'}; fin >> opponent_move >> my_move ;)
    {
        moves.push_back({ opponent_move, my_move });
    }

    {
        auto total_score = 0;
        for (const auto &[oponent_move, my_move] : moves)
        {
            // cout << oponent_move << ' ' << my_move << endl;
            total_score += kShapeScoreUmap.at(kCharToShapeUmap.at(my_move)) + kOutcomeScoreUmap.at(GetSingleMoveResult(oponent_move, my_move));
        }
        cout << "Part 1: " << total_score << endl;
    }

    {
        const auto kNeededOutcomeUmap = unordered_map<char, GameResult>{ { 'X', GameResult::kLose }, { 'Y', GameResult::kDraw }, { 'Z', GameResult::kWin } };
        auto total_score2 = 0;
        for (const auto &[kOponentMove, kNeededGameResult] : moves)
        {
            const auto &kNeededOutcome = kNeededOutcomeUmap.at(kNeededGameResult);
            const auto kNeededMove = MoveForRoundTo(kOponentMove, kNeededOutcome);
            total_score2 += kOutcomeScoreUmap.at(kNeededOutcome) + kShapeScoreUmap.at(kNeededMove);
        }
        cout << total_score2 << endl;
    }


    return 0;
}