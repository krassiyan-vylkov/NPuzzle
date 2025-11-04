#include <iostream>
#include <vector>
#include <cmath>
#include <string>

class BoardState{
public:
    std::vector<int> board;
    BoardState* parent;
    std::string lastMove;
    int numberOfColumns;
    int currSteps;
    int blankCoordinates;
    int blankGoal;
    int manhattanDist;

    BoardState(const std::vector<int>& board, BoardState* parent,int numberOfColumns, std::string lastMove,
        int currSteps, int blankCoordinates,int blankGoal, int manhattanDist):
        board(board), numberOfColumns(numberOfColumns), parent(parent), lastMove(lastMove),
        currSteps(currSteps), blankCoordinates(blankCoordinates), blankGoal(blankGoal), manhattanDist(manhattanDist)
    {
        
    }

    bool isSolution() {
        if (board[blankGoal] != 0) {
            return false;
        }
        for (int i = 0; i < blankGoal; i++) {
            if (board[i] != i + 1) {
                return false;
            }
        }
        for (int i = blankGoal + 1; i < board.size(); i++) {
            if (board[i] != i) {
                return false;
            }
        }
        return true;
    }
};

int calculateManhattanDist(std::vector<int>& board, int numberOfColumns, int blankGoal) {
    int distance = 0;
    for (int i = 0; i < blankGoal; i++){
        if(board[i] == 0) continue;
        distance = distance + abs((board[i] - 1) / numberOfColumns - i / numberOfColumns) +
        abs((board[i] - 1) % numberOfColumns - i %numberOfColumns);
    }
    if (board[blankGoal] != 0) {
        if (board[blankGoal] <= blankGoal) {
            distance = distance + abs((board[blankGoal] - 1) / numberOfColumns - blankGoal / numberOfColumns) +
            abs((board[blankGoal] - 1) % numberOfColumns - blankGoal % numberOfColumns);
        }
        else {
            distance = distance + abs(board[blankGoal] / numberOfColumns - blankGoal / numberOfColumns) +
            abs(board[blankGoal] % numberOfColumns - blankGoal % numberOfColumns);
        }
    }
    for (int i = blankGoal + 1; i < board.size(); i++) {
        if (board[i] == 0) continue;
        distance = distance + abs(board[i] / numberOfColumns - i / numberOfColumns) +
        abs(board[i] % numberOfColumns - i % numberOfColumns);
    }
    return distance;
}

bool isSolvable(std::vector<int>& board, int numberOfColumns,int blankPosition) {

    int numberOfInversion = 0;
    
    for (int i = 0; i < board.size() - 1; i++) {
        for (int j = i + 1; j < board.size(); j++) {
            if (board[i] != 0 && board[j] != 0 && board[i] > board[j]) {
                numberOfInversion++;
            }
        }
    }

    if (numberOfColumns % 2 == 0) {
        int blankRow = blankPosition / numberOfColumns + 1;
        return ((blankRow + numberOfInversion) % 2 == 0);

    }
    else {
        return (numberOfInversion % 2 == 0);
    }
}


int main()
{
    int size, blankGoalPosition;
    std::vector<int> startingBoard;
    std::cin >> size;
    std::cin >> blankGoalPosition;
    if (blankGoalPosition == -1) blankGoalPosition = size;
    int blankStartPosition;
    int numOfColumns = sqrt(size + 1);

    for (int i = 0; i <= size; i++) {
        int num;
        std::cin >> num;
        if (num == 0) blankStartPosition = i;
        startingBoard.push_back(num);
    }

    int manhattanDistanceStart = calculateManhattanDist(startingBoard, numOfColumns, blankGoalPosition);
    std::cout << manhattanDistanceStart;

    BoardState startingBoardState(startingBoard, nullptr, numOfColumns, "", 0, blankStartPosition, blankGoalPosition, manhattanDistanceStart);
    


    return 0;
}