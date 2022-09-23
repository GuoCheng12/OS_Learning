#include <stdio.h>
#include <iostream>
#include <string>
#include "Person.h"

using namespace std;

int main(int, const char**)
{
    system("clear");
    Person* persPtr[100];
    int n = 0;
    string choice;
    do {
        cout << "\nasdStudent (s) or Professor (p): ";
        cin >> choice;
        if (choice.length() == 1 && (choice == "s" || choice == "p")) {
            if (choice == "s" )
                persPtr[n] = new Student;
            else if (choice == "p")
                persPtr[n] = new Professor;
            ((Student*)persPtr[n])->getGpa();
            persPtr[n++]->getData();
            if (n == 100) return 0;
        } else cout << "Unknow command\n";
        cout << "More enter (y/n)? ";
        cin >> choice;
    } while(choice == "y");
    
    cout << endl;
    for (int j = 0; j < n; j++)
    {
        persPtr[j]->getName();
        persPtr[j]->getSurName();
        persPtr[j]->getAge();
        cout << endl;
    }
    return 0;
}
