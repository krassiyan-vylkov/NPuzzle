#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <climits>
#include <deque>

const int FOUND = -1;

class BoardState{
public:
    std::vector<int> board;
    std::string lastMove;
    int boardSideSize;
    int currSteps;
    int blankCoordinates;
    int blankGoal;
    int manhattanDist;

    BoardState(const std::vector<int>& board, int boardSideSize, std::string& lastMove,
        int currSteps, int blankCoordinates,int blankGoal, int manhattanDist):
        board(board), boardSideSize(boardSideSize),  lastMove(lastMove),
        currSteps(currSteps), blankCoordinates(blankCoordinates), blankGoal(blankGoal), manhattanDist(manhattanDist)
    {
        
    }

    bool isSolution() {
        return manhattanDist == 0;
    }
};

/*
Since a single dimension vector is used:
With -value- % boardSideSize we get column index (starting from 0)
With -value- / boardSideSize we get row index (starting from 0)
Special treatment of blankGoal position based on the value of the tile in it
*/

int calculateManhattanDist(std::vector<int>& board, int boardSideSize, int blankGoal) {
    int distance = 0;
    for (int i = 0; i < blankGoal; i++){
        if (board[i] == 0) {
            continue;
        }
        distance = distance + abs((board[i] - 1) / boardSideSize - i / boardSideSize) +
        abs((board[i] - 1) % boardSideSize - i %boardSideSize);
    }
    if (board[blankGoal] != 0) {
        if (board[blankGoal] <= blankGoal) {
            distance = distance + abs((board[blankGoal] - 1) / boardSideSize - blankGoal / boardSideSize) +
            abs((board[blankGoal] - 1) % boardSideSize - blankGoal % boardSideSize);
        }
        else {
            distance = distance + abs(board[blankGoal] / boardSideSize - blankGoal / boardSideSize) +
            abs(board[blankGoal] % boardSideSize - blankGoal % boardSideSize);
        }
    }
    for (int i = blankGoal + 1; i < board.size(); i++) {
        if (board[i] == 0) {
            continue;
        }
        distance = distance + abs(board[i] / boardSideSize - i / boardSideSize) +
        abs(board[i] % boardSideSize - i % boardSideSize);
    }
    return distance;
}

bool isSolvable(std::vector<int>& board, int boardSideSize,int blankPosition, int blankGoal) {

    int numberOfInversion = 0;
    
    for (int i = 0; i < board.size() - 1; i++) {
        for (int j = i + 1; j < board.size(); j++) {
            if (board[i] != 0 && board[j] != 0 && board[i] > board[j]) {
                numberOfInversion++;
            }
        }
    }

    
    if (boardSideSize % 2 == 0) {
        //different solvability checks if goal on even index row (top left) and odd index row (bottom right)
        if ( (blankGoal / boardSideSize) % 2 == 0 ) {
            int blankRow = blankPosition / boardSideSize + 1;
            return ((blankRow + numberOfInversion) % 2 == 1);
        }
        else {
            int blankRow = blankPosition / boardSideSize + 1;
            return ((blankRow + numberOfInversion) % 2 == 0);
        }
    }
    else {
        return (numberOfInversion % 2 == 0);
    }
}


int searchSolution(BoardState& currentState, int threshold, std::deque<std::string>& solutionPath) {
    if (currentState.isSolution()) {
        std::cout << currentState.currSteps << std::endl;
        return FOUND;
    }
    if ((currentState.manhattanDist + currentState.currSteps) > threshold) {
        return currentState.manhattanDist + currentState.currSteps;
    }

    int minHigherThanThreshold = INT_MAX;

    //successor next move 'right'
    if (currentState.lastMove != "left" && currentState.blankCoordinates % currentState.boardSideSize != 0) {
        std::vector<int> successorBoard = currentState.board;
        const int successorBlankCoordinates = currentState.blankCoordinates - 1;
        std::swap(successorBoard[currentState.blankCoordinates],successorBoard[successorBlankCoordinates]);
        std::string nextMove = "right";
        int successorManhattanAdjustment;
        int movedTileNewColumn = currentState.blankCoordinates % currentState.boardSideSize;
        int movedTileOldColumn = (successorBlankCoordinates) % currentState.boardSideSize;
        int correctTileColumn;
        if (successorBoard[currentState.blankCoordinates] > currentState.blankGoal) {
            correctTileColumn = successorBoard[currentState.blankCoordinates] % currentState.boardSideSize;
        }
        else {
            correctTileColumn = (successorBoard[currentState.blankCoordinates] - 1) % currentState.boardSideSize;
        }
        successorManhattanAdjustment = abs(movedTileNewColumn - correctTileColumn) - abs(movedTileOldColumn - correctTileColumn);

        BoardState successorBoardState(successorBoard, currentState.boardSideSize, nextMove, currentState.currSteps + 1,
            successorBlankCoordinates, currentState.blankGoal, currentState.manhattanDist + successorManhattanAdjustment);

        solutionPath.push_back(nextMove);
        int potentialNewThreshold = searchSolution(successorBoardState, threshold, solutionPath);
        if (potentialNewThreshold == FOUND) {
            return FOUND;
        }
        if (potentialNewThreshold < minHigherThanThreshold) {
            minHigherThanThreshold = potentialNewThreshold;
        }
        solutionPath.pop_back();
    }


    //successor next move 'left'
    if (currentState.lastMove != "right" && currentState.blankCoordinates % currentState.boardSideSize + 1 != currentState.boardSideSize) {
        std::vector<int> successorBoard = currentState.board;
        const int successorBlankCoordinates = currentState.blankCoordinates + 1;
        std::swap(successorBoard[currentState.blankCoordinates], successorBoard[successorBlankCoordinates]);
        std::string nextMove = "left";
        int successorManhattanAdjustment;
        int movedTileNewColumn = currentState.blankCoordinates % currentState.boardSideSize;
        int movedTileOldColumn = (successorBlankCoordinates) % currentState.boardSideSize;
        int correctTileColumn;
        if (successorBoard[currentState.blankCoordinates] > currentState.blankGoal) {
            correctTileColumn = successorBoard[currentState.blankCoordinates] % currentState.boardSideSize;
        }
        else {
            correctTileColumn = (successorBoard[currentState.blankCoordinates] - 1) % currentState.boardSideSize;
        }
        successorManhattanAdjustment = abs(movedTileNewColumn - correctTileColumn) - abs(movedTileOldColumn - correctTileColumn);

        BoardState successorBoardState(successorBoard, currentState.boardSideSize, nextMove, currentState.currSteps + 1,
            successorBlankCoordinates, currentState.blankGoal, currentState.manhattanDist + successorManhattanAdjustment);

        solutionPath.push_back(nextMove);
        int potentialNewThreshold = searchSolution(successorBoardState, threshold, solutionPath);
        if (potentialNewThreshold == FOUND) {
            return FOUND;
        }
        if (potentialNewThreshold < minHigherThanThreshold) {
            minHigherThanThreshold = potentialNewThreshold;
        }
        solutionPath.pop_back();
    }


    //successor next move 'down'
    if (currentState.lastMove != "up" && currentState.blankCoordinates / currentState.boardSideSize != 0) {
        std::vector<int> successorBoard = currentState.board;
        const int successorBlankCoordinates = currentState.blankCoordinates - currentState.boardSideSize;
        std::swap(successorBoard[currentState.blankCoordinates], successorBoard[successorBlankCoordinates]);
        std::string nextMove = "down";
        int successorManhattanAdjustment;
        int movedTileNewRow = currentState.blankCoordinates / currentState.boardSideSize;
        int movedTileOldRow = (successorBlankCoordinates) / currentState.boardSideSize;
        int correctTileRow;
        if (successorBoard[currentState.blankCoordinates] > currentState.blankGoal) {
            correctTileRow = successorBoard[currentState.blankCoordinates] / currentState.boardSideSize;
        }
        else {
            correctTileRow = (successorBoard[currentState.blankCoordinates] - 1) / currentState.boardSideSize;
        }
        successorManhattanAdjustment = abs(movedTileNewRow - correctTileRow) - abs(movedTileOldRow - correctTileRow);

        BoardState successorBoardState(successorBoard, currentState.boardSideSize, nextMove, currentState.currSteps + 1,
            successorBlankCoordinates, currentState.blankGoal, currentState.manhattanDist + successorManhattanAdjustment);

        solutionPath.push_back(nextMove);
        int potentialNewThreshold = searchSolution(successorBoardState, threshold, solutionPath);
        if (potentialNewThreshold == FOUND) {
            return FOUND;
        }
        if (potentialNewThreshold < minHigherThanThreshold) {
            minHigherThanThreshold = potentialNewThreshold;
        }
        solutionPath.pop_back();
    }


    //successor next move 'up'
    if (currentState.lastMove != "down" && (currentState.blankCoordinates / currentState.boardSideSize) + 1 != currentState.boardSideSize) {
        std::vector<int> successorBoard = currentState.board;
        const int successorBlankCoordinates = currentState.blankCoordinates + currentState.boardSideSize;
        std::swap(successorBoard[currentState.blankCoordinates], successorBoard[successorBlankCoordinates]);
        std::string nextMove = "up";
        int successorManhattanAdjustment;
        int movedTileNewRow = currentState.blankCoordinates / currentState.boardSideSize;
        int movedTileOldRow = successorBlankCoordinates / currentState.boardSideSize;
        int correctTileRow;
        if (successorBoard[currentState.blankCoordinates] > currentState.blankGoal) {
            correctTileRow = successorBoard[currentState.blankCoordinates] / currentState.boardSideSize;
        }
        else {
            correctTileRow = (successorBoard[currentState.blankCoordinates] - 1) / currentState.boardSideSize;
        }
        successorManhattanAdjustment = abs(movedTileNewRow - correctTileRow) - abs(movedTileOldRow - correctTileRow);

        BoardState successorBoardState(successorBoard, currentState.boardSideSize, nextMove, currentState.currSteps + 1,
            successorBlankCoordinates, currentState.blankGoal, currentState.manhattanDist + successorManhattanAdjustment);

        solutionPath.push_back(nextMove);
        int potentialNewThreshold = searchSolution(successorBoardState, threshold, solutionPath);
        if (potentialNewThreshold == FOUND) {
            return FOUND;
        }
        if (potentialNewThreshold < minHigherThanThreshold) {
            minHigherThanThreshold = potentialNewThreshold;
        }
        solutionPath.pop_back();
    }


    return minHigherThanThreshold;

}

void solve(BoardState& startingBoardState, std::deque<std::string>& solutionPath) {
    int threshold = startingBoardState.manhattanDist;
    
    //since we call it after solvability check, this should break out of loop
    while (true) {
        int newThreshold = searchSolution(startingBoardState, threshold, solutionPath);
        if (newThreshold == FOUND) {
            break;
        }
        threshold = newThreshold;
    }
}

int main()
{
    int size, blankGoalPosition;
    std::vector<int> startingBoard;

    std::cin >> size;
    std::cin >> blankGoalPosition;

    if (blankGoalPosition == -1) blankGoalPosition = size;
    int numOfColumns = sqrt(size + 1);
    int blankStartPosition;

    //with correct input we should always have a value for blankStartPosition
    for (int i = 0; i <= size; i++) {
        int num;
        std::cin >> num;
        if (num == 0) blankStartPosition = i;
        startingBoard.push_back(num);
    }

    int manhattanDistanceStart = calculateManhattanDist(startingBoard, numOfColumns, blankGoalPosition);
    int startingSteps = 0;
    std::string startingMove = "";
    BoardState startingBoardState(startingBoard, numOfColumns, startingMove, startingSteps, blankStartPosition, blankGoalPosition, manhattanDistanceStart);

    std::deque<std::string> solutionPath;

    if (isSolvable(startingBoard, numOfColumns, blankStartPosition, blankGoalPosition)) {
        solve(startingBoardState, solutionPath);

        while (!solutionPath.empty()) {
            std::cout << solutionPath.front() << std::endl;
            solutionPath.pop_front();
        }
    }
    else {
        std::cout << -1;
    }
    
    return 0;
}