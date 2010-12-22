#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <string>
#include <functional>

using std::ostream;
using std::cout;
using std::endl;
using std::vector;
using std::string;

static const unsigned short BOARDSIZE = 9;
static const string GOAL = "123456780";

static const short manhattan(short x1, short y1, short x2, short y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

class Node {
public:
	vector<Node> mChildren;
	Node* mParent;
	string mBoardLayout;
	string mDirection;
	short mG, mBlankTilePos[2];
	float mH, mF;
	bool inOpen;

	Node() {
		mBoardLayout = "";
		mDirection = ' ';
		mG, mH, mF = -1;
		mParent = NULL;
	}

	//Constructor for initalizing the first node in the tree
	Node(string boardLayout) {
		mBoardLayout = boardLayout;
		findBlank();
		mDirection = ' ';
		mParent = NULL;
		mG = 0;
		mH = hValue();
		mF = mG + mH;
	}

	//Constructor for all nodes other than the head of the tree
	Node(string boardLayout, string direction, Node* parent, short g) {
		mBoardLayout = boardLayout;
		mDirection = direction;
		findBlank();
		mParent = parent;
		mG = parent->mG + 1;
		mH = hValue();
		mF = mH + mG;
		inOpen = false;
	}

	~Node() {
	}

	//Store position of blank tile
	void findBlank() {
		int index;
		index = mBoardLayout.find("0");

		mBlankTilePos[0] = index / 3;
		mBlankTilePos[1] = index % 3;
	}

	//Store position of selected tile
	short* getTileCoords(string board, char tile) {
		short index;
		short* coords = new short[2];

		index = board.find(tile);
		coords[0] = index / 3;
		coords[1] = index % 3;

		return coords;
	}

	// How far we get in our board before we find a misplaced tile
	short placed() {
		bool misplaced = false;
		short i = 0;

		while(misplaced == false && i < BOARDSIZE) {
			if(mBoardLayout[i] != GOAL[i])
				misplaced = true;
			else
				i++;
		}

		return i;
	}

	// 9 - Placed(s)
	short misplaced() {
		short pl = placed();
		return 9 - pl;
	}

	// Manhattan distance of the first tile that is not in place in board configuration
	short manhattanOfFirstMisplacedTile() {
		short manhattanToGoal = 0;
		short* misplacedTile = NULL;
		short* goalTile = NULL;

		for(short i = 0; i < BOARDSIZE; i++) {		
			if(mBoardLayout[i] != GOAL[i]) {

				for(short j = 0 + i; j < BOARDSIZE; j++) {			
					if(mBoardLayout[j] == GOAL[i]) {

						misplacedTile = getTileCoords(mBoardLayout, mBoardLayout[j]);
						goalTile = getTileCoords(GOAL, GOAL[i]);

						manhattanToGoal = manhattan(misplacedTile[0], misplacedTile[1], goalTile[0], goalTile[1]);
						break;
					}
				}

				break;
			}
		}

		delete [] misplacedTile;
		delete [] goalTile;

		return manhattanToGoal;
	}

	// Manhattan distance of the first misplaced tile from the blank tile
	short manhattanOfFirstFromBlank() {
		short manhattanToBlank = 0;
		short* goalTile = NULL;

		for(short i = 0; i < BOARDSIZE; i++) {
			if(mBoardLayout[i] != GOAL[i]) {
				goalTile = getTileCoords(GOAL, GOAL[i]);

				manhattanToBlank = manhattan(mBlankTilePos[0], mBlankTilePos[1], goalTile[0], goalTile[1]);
				break;
			}
		}

		delete [] goalTile;

		return manhattanToBlank;
	}

	short hValue() {
		return 36 * misplaced() + 18 * manhattanOfFirstMisplacedTile() + manhattanOfFirstFromBlank();
	}
	
	bool operator<(const Node& n) const {
		return this->mF < n.mF;
	}
	
	bool operator>(const Node& n) const {
		return this->mF > n.mF;
	}

	bool operator!=(const Node& n) const {
		return !(this->mF == n.mF);
	}

	friend ostream& operator<<(ostream& out, Node* n) {
		cout << n->mBoardLayout;
		return out;
	}
	
};

class NodeComp {
public:
	bool operator()(Node* n1, Node* n2) const {
		return n1->mF > n2->mF;
	}
};

#endif