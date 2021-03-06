#include "board.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
	cout << "Welcome to PP9K! By Scott Broeders and John Brockway, Fall 2015" << endl;
	string input;
	bool gameOn = true;
	while (gameOn) {                     // cycling through games
		char player = 'b';
		Board b;
		bool whiteHuman = true;
		bool blackHuman = true;
		if (argc == 1) {
			while (1) {
				b.clear();
				cout << endl << "To start a new game, enter ''game x y'' where x and y are either " << endl;
				cout << "''human'' or ''computer''. To enter setup, type ''setup''" << endl;
				cout << "To stop playing for right now, type ''quit''" << endl;
				getline(cin, input);
				istringstream str(input);
				input = "";
				str >> input;
				if (input == "game") {
					cout << endl << "--------------------------------------" << endl;
					cout << "Instructions:" << endl << "To move, type ''move {origin} {destination}''" << endl;
					cout << "> For example: move b2 b4 will (from starting position) move white's pawn two spaces forward" << endl;
					cout << "To resign the game on your turn, simply type ''resign''" << endl;
					b.defSetup();
					b.print();
					player = 'w';
					str >> input;
					if (input != "human") whiteHuman = false;
					str >> input;
					if (input != "human") blackHuman = false;
					break;
				}
				else if (input == "setup") {
					cout << endl << "--------------------------------------" << endl;
					cout << "Instructions:" << endl << "1. To add a piece, type ''+ {type} {location}''" << endl;
					cout << "> For example: + p b4 will add a black pawn at b4" << endl << "  (upper case letters for white, lower for black)" << endl;
					cout << "2. To remove a piece from the board, type ''- {location}''" << endl;
					cout << "3. To choose which player will start the game, type ''= {colour}''" << endl << "  where color is either black or white" << endl;
					cout << "4. When ready to begin, simply type ''done''" << endl;
					player = 'w';
					b.defSetup();
					b.print();
					while (1) {
						char addType;
						getline (cin, input);
						istringstream iss(input);
						iss >> input;
						if (input == "+") {
							iss >> addType;
							iss >> input;
							bool validAdd = b.setPiece(addType, input);
							if (validAdd) b.print();
							else {
								cout << "Not a valid add" << endl;
							}
						}
						else if (input == "-") {
							iss >> input;
							bool validRem = b.setPiece(0, input);
							if (validRem) b.print();
							else {
								cout << "Not a valid remove" << endl;
							}
						}
						else if (input == "=") {
							iss >> input;
							if (input == "black") player = 'b';
							else player = 'w';
						}
						else if (input == "done") {
							bool pawns = b.checkPawnsSetup();
							bool kings = b.checkKingsSetup();
							if (!pawns) {
								cout << "Pawns on last row are illegal" << endl;
							}
							else if (!kings) {
								cout << "Need exactly one of each king" << endl;
							}
							else break;
						}
					}
					break;
				}
				else if (input == "quit") {
					gameOn = false;
					break;
				}
				else {
					cout << "Enter valid input" << endl;
				}
			}
		}
		else {
			string line;
			ifstream save(argv[1]);
			for (int i = 0 ; i < 8 ; i++) {
				getline(save, line);
				for (int j = 0 ; j < 8 ; j++) {
					if (line[j] != '_') {
						string placement = "a1";
						placement[0] = j+'a';
						placement[1] = (8 - i) + '0';
						b.setPiece(line[j], placement);
					}
				}
			}
			getline(save, line);
			player = line[0] - 'A' +'a';
			argc = 1;
			b.print();
		}
		bool wCheck = false;
		bool bCheck = false;
		while (gameOn) {                      // cycling through turns
			cout << player << "'s Move: ";
			if (player == 'w')
				b.clearEPFlags(3);
			else
				b.clearEPFlags(2);
			if ((player == 'w' && whiteHuman) || (player == 'b' && blackHuman)) {
				input = "";
				getline(cin, input);
				if (input == "resign") {
					if (player == 'w') {
						cout << "Black wins!" << endl;
					}
					else {
						cout << "White wins!" << endl;
					}
					break;
				}
				string input2;
				istringstream ss(input);
				ss >> input2;
				if (input2 == "move") {
					string start;
					string end;
					char promotion;
					bool validTurn = false;
					ss >> start;
					ss >> end;
					bool rightTeam = b.rightTeam(player, start);
					if (rightTeam) {
						if (ss >> promotion) validTurn = b.move(player, start, end, promotion);
						else validTurn = b.move(player, start, end);
						if (validTurn) b.print();
						else {
							cout << "That was not a valid move!" << endl;
							continue;
						}
					}
					else {
						cout << "Pick your own piece!" << endl;
						continue;
					}
				}
				else {
					cout << "That was not a valid command." << endl;
					continue;
				}
			}
			else {
				cout << b.aiMove(player) << endl;
				b.print();
			}
			if (b.checkWhite(b.findKing('w'))) {
				cout << "White is in check!" << endl;
				wCheck = true;
			}
			else wCheck = false;
			if (b.checkBlack(b.findKing('b'))) {
				cout << "Black is in check!" << endl;
				bCheck = true;
			}
			else bCheck = false;
			if (!b.anyMoves('w') && wCheck) {
				cout << "Checkmate! Black wins!" << endl;
				break;
			}
			if (!b.anyMoves('b') && bCheck) {
				cout << "Checkmate! White wins!" << endl;
				break;
			}
			if (!b.anyMoves('w')) {
				cout << "Stalemate!" << endl;
				break;
			}
			if (!b.anyMoves('b')) {
				cout << "Stalemate!" << endl;
				break;
			}
			if (player == 'w') player = 'b';
			else if (player == 'b') player = 'w';
		}
	}
}