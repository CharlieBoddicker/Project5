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

        vector<vector<int>> graph;

        graph.resize(dice.size()+words.size()+2);
        for(int i = 0; i < dice.size()+words.size()+2; i++){
                graph[i].resize(words.size()+dice.size()+2);
                for(int j = 0; j < dice.size()+words.size()+2; j++){
                        if(i == 0 && j > 0 && j < dice.size()+1){
                                graph[i][j] = 1;
                        }
                        else graph[i][j] = -1;

                        cout << graph[i][j] << " ";
                }
                cout << endl;
        }







        return 0;
}
