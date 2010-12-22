#ifndef EIGHTPUZZLE_H
#define EIGHTPUZZLE_H

#include <queue>
#include <string>
#include <cmath>
#include "Node.h"

using std::priority_queue;
using std::greater;

class EightPuzzle {
public:
	Node* mHead;
	Node* mTail;
	string mBoardValue, mDirections;
	short mG;
	
	vector<Node*> closedList, solvedList;
	priority_queue<Node*, vector<Node*>, NodeComp> openList;

	EightPuzzle() {
		mBoardValue, mDirections = "";
		mG = 0;
		mHead, mTail = NULL;
	}

	EightPuzzle(string boardLayout) {
		mBoardValue = boardLayout;
		mHead = new Node(boardLayout);
		mG = 0;
		mDirections = "";
		mTail = NULL;
	}

	//Populate tree and store solution
	void solve() {
		populateTree();

		getPath();
	}

	//Move the blank tile either horizontally or vertically
	string moveBlank(string boardLayout, short row, short col) {
		short src = row * 3 + col;
		short dst = boardLayout.find('0');
	
		boardLayout[dst] = boardLayout[src];
		boardLayout[src] = '0';
	
		return boardLayout;
	}

	//Check if new direction is legal
	bool checkBounds(short row, short col) {
		return (row < 3) && (row >= 0) && (col < 3) && (col >= 0);
	}

	//Get direction of move
	string getDirection(short row, short col) {
		if (row == 1 && col == 0)
			return "Down";

		if (row == -1 && col == 0)
			return "Up";

		if (row == 0 && col == 1)
			return "Right";

		if (row == 0 && col == -1)
			return "Left";

		return "";
	}

	//Populate tree until goal is reached
	void populateTree() {
		Node* curNode = NULL;
		Node* curChild = NULL;
		vector<short>::size_type childrenSize = NULL;
	
		openList.push(mHead);
		mG++;

		while(!openList.empty()) {
			curNode = openList.top();

			closedList.push_back(curNode);
			openList.pop();
			
			if(curNode->mBoardLayout == GOAL) {
				mTail = curNode;
				return;
			} 

			possibleMoves(curNode);

			childrenSize = curNode->mChildren.size();

			for(size_t i = 0; i < childrenSize ; i++) {
				curChild = &curNode->mChildren.at(i);

				if(!inClosed(curChild)) {
					if(!curChild->inOpen) {
						openList.push(curChild);
						curChild->inOpen = true;
					} 
				}

			}
			
			curChild = NULL;
			mG++;
		}
	}

	//Check if board has been examined
	bool inClosed(Node* node) {
		vector<short>::size_type closedSize = closedList.size();

		for(size_t i = 0; i < closedSize; i++) {
			if(node->mBoardLayout.compare(closedList.at(i)->mBoardLayout) == 0)
				return true;
		}

		return false;
	}

	//Generate moves reachable from current position
	void possibleMoves(Node* curNode) {
		short blank[2];
		string newBoard;

		blank[0] = curNode->mBlankTilePos[0];
		blank[1] = curNode->mBlankTilePos[1];
		
		for(short row = -1; row <= 1; row++) {
			for(short col = -1; col <= 1; col++) {
				
				if(abs(row) != abs(col)) {
					if(checkBounds(blank[0] + row, blank[1] + col)) {
						newBoard = moveBlank(curNode->mBoardLayout, blank[0] + row, blank[1] + col);

						curNode->mChildren.push_back(Node(newBoard, getDirection(row, col), curNode, mG));
					}
				}
			}
		}

		return;
	}

	//Search solution in tree and store path
	void getPath() {
		while(mTail->mParent != NULL) {
			solvedList.push_back(mTail);
			mDirections += mTail->mDirection;
			mTail = mTail->mParent;
		}

		reverse(mDirections.begin(), mDirections.end());
	}

	//Print a selected board 
	void printBoard(string board) {
		cout << "| " << board[0] << " | " << board[1] << " | " << board[2] << " |\n"
			 << "| " << board[3] << " | " << board[4] << " | " << board[5] << " |\n"
			 << "| " << board[6] << " | " << board[7] << " | " << board[8] << " |\n";
	}

	//Print best path in successive boards
	void printBoardPath() {
		vector<int>::size_type solvedSize = solvedList.size();

		cout << "\nSTART\n";
		printBoard(mHead->mBoardLayout);

		for(short i = solvedSize - 1; i >= 0; i--) {
			cout << "\n" << solvedList.at(i)->mDirection << endl;
			printBoard(solvedList.at(i)->mBoardLayout);
		}

		cout << endl;
	}

	//Print examined boards
	void printClosedList() {
		vector<short>::size_type closedSize = closedList.size();

		cout << "\n - Closed List - " << endl;
		cout << "LOOP/BOARD/G + H = F" << endl;
		for(size_t i = 0; i < closedSize; i++) {
			cout << i << ". " 
					  << closedList.at(i)->mBoardLayout << " " 
					  << closedList.at(i)->mG << "+" 
					  << closedList.at(i)->mH << "=" 
					  << closedList.at(i)->mF 
			<< endl;
		}

		cout << endl;
	}

};

#endif