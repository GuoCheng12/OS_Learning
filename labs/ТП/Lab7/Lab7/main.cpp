#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    char *__char = new char[1];
    __char[0] = 't';
    cout << __char[0];
    return 0;
}
