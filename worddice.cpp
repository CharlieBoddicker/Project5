using namespace std;
#include<iostream>
#include<vector>
#include<string>
#include<fstream>

int main(int argc, char **argv){
    vector<string> dice;
    vector<string> words;
    string word;
    string dye;

    ifstream fin(argv[1]);

    while(fin >> dye){
        dice.push_back(dye);
        cout << dye << endl;
    }
    fin.close();

    fin.open(argv[2]);

    while(fin >> word){
        words.push_back(word);
        cout << word << endl;
    }

    fin.close();




    return 0;
}
