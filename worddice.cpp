using namespace std;
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

int main(int argc, char **argv) 
{
    vector<string> dice;
    vector<string> words;
    string word;
    string dye;
    ifstream fin(argv[1]);
    while (fin >> dye) 
    {
        dice.push_back(dye);
    }
    fin.close();
    fin.open(argv[2]);
    while (fin >> word) 
    {
        words.push_back(word);
    }
    fin.close();

    //graph creation for each word
    for (int i = 0; i < words.size(); i++) 
    {
        word = words[i];
        int numDice = dice.size();
        int wordLen = word.size();
        int totalNodes = numDice + wordLen + 2;
        vector<vector<int>> graph(totalNodes);
        int source = 0;
        int sink = totalNodes - 1;
        //add edges from source to each die node
        for (int j = 0; j < numDice; j++) 
        {
            graph[source].push_back(j + 1);
        }
        //for each letter in the word, create a unique letter node
        //map each letter occurrence in the word to a unique node
        unordered_map<char, vector<int>> letterNodeMap;
        for (int i = 0; i < wordLen; i++) 
        {
            letterNodeMap[word[i]].push_back(numDice + i + 1);
        }
        //add edges from dice nodes to the letter nodes if the dice contain the letter
        unordered_map<int, unordered_set<int>> connectedNodes;//unordered set so that there are no duplicates
        for (int i = 0; i < numDice; i++) 
        {
            for (int j = 0; j < dice[i].size(); j++) 
            {
                char letter = dice[i][j];
                //for each occurrence of the letter in the word, add edges
                if (letterNodeMap.find(letter) != letterNodeMap.end()) 
                {
                    //for each unique letter position in the word
                    for (int letterNode : letterNodeMap[letter])
                    {
                        if (connectedNodes[i + 1].find(letterNode) == connectedNodes[i + 1].end()) 
                        {
                            graph[i + 1].push_back(letterNode);
                            connectedNodes[i + 1].insert(letterNode);
                        }
                    }
                }
            }
        }

        //add edges from each letter node to the sink
        for (int i = 0; i < wordLen; i++) 
        {
            int letterNode = numDice + i + 1;
            graph[letterNode].push_back(sink);
        }
        for (int i = 0; i < graph.size(); i++) 
        {
            cout << "Node " << i << ": ";
            if (i == source) 
            {
                cout << "SOURCE ";
            }
            else if (i == sink) 
            {
                cout << "SINK ";
            }
            else if (i <= numDice) 
            {
                cout << dice[i - 1] << " ";
            }
            else 
            {
                cout << word[i - numDice - 1] << " ";
            }
            cout << "Edges to";
            for (int j : graph[i]) 
            {
                cout << " " << j;
            }
            cout << endl;
        }
        cout << endl;
    }

    return 0;
}
