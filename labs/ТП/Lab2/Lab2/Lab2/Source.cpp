#include "Tree.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <Windows.h>

using namespace std;

const int NUMBER_FIELD = 5;

void print_menu(string* menu, int n, int pos) {
	system("cls");
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
	for (int i = 0; i < pos; i++) cout << menu[i] << endl;

	SetConsoleTextAttribute(handle, FOREGROUND_RED);
	cout << menu[pos] << endl;

	SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
	for (int i = pos + 1; i < n; i++) cout << menu[i] << endl;
}


int main() {
	int pos = 0;
	string menu[NUMBER_FIELD] = { "1 - Add node in the tree", "2 - Search select node in the tree", 
		"3 - Delete tree", "4 - Print tree", "5 - Exit"};
	print_menu(menu, NUMBER_FIELD, pos);

	Tree *t = new Tree;

	char ch;
	int code, temp;
	double *arr;
	bool b;
	do {
		ch = getch();
		code = static_cast<int> (ch);
		switch (code)
		{
		case 80:
			if (pos < NUMBER_FIELD - 1) {
				++pos;
				print_menu(menu, NUMBER_FIELD, pos);
			}
			break;

		case 72:
			if (pos > 0) {
				--pos;
				print_menu(menu, NUMBER_FIELD, pos);
			}
			break;

		case 13:
			system("cls");
			switch (pos)
			{
			case 0:
				t->insert();
				break;
			
			case 1:
				cout << "Enter key-value node: ";
				cin >> temp;
				TreeNode *res;
				res = t->search(temp);
				if (res == nullptr)
					cout << "Node there is no in tree with current key-value";
				else {
					cout << "Node there is in tree with current key-value\n";
					cout << "Value: " << res->value << endl;
					cout << "Array: ";
					for (int i = 0; i < res->size; i++)
						cout << res->array_d[i] << " ";
				}
				getch();
				break;

			case 2:
				t->delete_tree();
				break;

			case 3:
				if (t->get_root() != nullptr)
					t->print();
				else
					cout << "Tree is free";
				getch();
				break;

			case 4:
				return 0;
				break;
			default:
				break;
			}
			print_menu(menu, NUMBER_FIELD, pos);
			break;
		default:
			break;
		}
	} while (code != 27);

	getch();
	return 0;
}