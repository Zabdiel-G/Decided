//3350
//progam: mangeles.cpp
//Author: Marc Angeles
//2/27/2023

#include <iostream>
using namespace std; 

void messageFire()
{
    printf("pew");    
}

bool startMenu() {
    bool exit = false;
    int num = 0;
    while (exit != true) {
        cout << "Start Menu" << endl;
        cout << "1: Start Game 2:End Game" << endl;
        cin >> num;
        switch (num) {
            case 1:
                return true;
                exit = true;
                break;
            case 2:
                return false;
                exit = true;
                break;
            default:
                cout << "Please Choose A Different Number" << endl;
                break;
        }
    }
    return 0;
}


