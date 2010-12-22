#include <iomanip>
#include "EightPuzzle.h"

using std::setw;
using std::cin;

int main()
{
	EightPuzzle puzzle("724506831");
	short commandSelect = 0;

	cout << "Eight Puzzle\n"
		 << "\n\n";

	puzzle.solve();

	do {
		cout << "*** Commands ***\n"
			 << "1. Board Path" << setw(4) << "" << "2. Closed List\n"
			 << "\n"
			 << "Enter Num: ";
		cin >> commandSelect;

		switch(commandSelect) {
			case 1:
				puzzle.printBoardPath();
				break;
			case 2:
				puzzle.printClosedList();
				break;
		}

	} while(true);

	return 0;
};
