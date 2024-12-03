using namespace std;
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <deque>

bool bfs(vector<vector<int>> graph)
{
    int sink = 0;
    int source = graph.size()-1;
    deque<int> frontier;
    vector<int> visited;
    vector<int> path;
    frontier.push_back(sink);
    while(!frontier.empty())
    {
        int node = frontier[0];

        frontier.pop_front();
        visited.push_back(node);

        if(node == source) return 1;

        for(int i = 0; i < graph[node].size(); i++)
        {
            bool cont = true;
            for(int j = 0; j < visited.size(); j++)
            {
                if(visited[j] == graph[node][i]) cont = false;
                if(cont) frontier.push_back(graph[node][i]);
            }
        }
    }
    return 0;

}

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
        int numSink = 0;

        //graph creation for each word
        for (int i = 0; i < words.size(); i++)
        {
                word = words[i];
                int numDice = dice.size();
                int wordLen = word.size();
                int totalNodes = numDice + wordLen + 2;
                vector<vector<int>> graph(totalNodes);
                vector<vector<int>> reverseGraph(totalNodes);
                int source = 0;
                int sink = totalNodes - 1;

                //add edges from source to each die node
                for (int j = 0; j < numDice; j++)
                {
                        graph[source].push_back(j + 1);
                }

                //for each letter in the word, create a unique letter node
                //map each letter occurence in the word to a unique node
                unordered_map<char, vector<int>> letterNodeMap;
                for (int i = 0; i < wordLen; i++)
                {
                        letterNodeMap[word[i]].push_back(numDice + i + 1);
                }

                //add edges from dice nodes to the letter nodes if the dice contain the letter
                unordered_map<int, unordered_set<int>> connectedNodes; //unordered set so that there are no duplicates
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
                                                        reverseGraph[letterNode].push_back(i + 1);
                                                        connectedNodes[i + 1].insert(letterNode);
                                                }
                                        }
                                }
                        }
                }
                vector<int> nodePointers(totalNodes, 0); //vector that counts how many edges(dice) are pointing to a letter node
                //loop through the graph and update the nodePointers vector
                for (int i = 0; i < graph.size(); i++)
                {
                        for (int j : graph[i])
                        {
                                nodePointers[j]++;
                        }
                }

                //removes dice to word connections from dice that point to multiple words if one of the words only has 1 dice pointing to it
                for (int i = 0; i < wordLen; i++)
                {
                        int OGnode = -1;
                        int node = -1;
                        int letterNode = numDice + i + 1;

                        if (nodePointers[letterNode] == 1)
                        {
                                OGnode = reverseGraph[letterNode][0];
                                node = letterNode;
                        }

                        if (node != -1)
                        {
                                for (int k = 0; k < graph[OGnode].size(); k++)
                                {
                                        int j = graph[OGnode][k];
                                        if (j != node && graph[OGnode].size() != 1)
                                        {
                                                auto it = remove(graph[OGnode].begin(), graph[OGnode].end(), j);
                                                graph[OGnode].erase(it, graph[OGnode].end());
                                                nodePointers[j]--;
                                                k--;
                                        }
                                }
                        }
                }

                //add edges from each letter node to the sink
                for (int i = 0; i < wordLen; i++)
                {
                        int letterNode = numDice + i + 1;
                        if (nodePointers[letterNode] > 0)
                        {
                                graph[letterNode].push_back(sink);
                        }
                }


                //output the dice used in order to spell the word
                vector<int> diceUsed;
                for (int i = 0; i < wordLen; i++)
                {
                        bool cont = true;
                        int letterNode = numDice + i + 1;
                        for (int j = 0; j < numDice; j++)
                        {
                                if (find(graph[j + 1].begin(), graph[j + 1].end(), letterNode) != graph[j + 1].end())
                                {
                                        diceUsed.push_back(j);
                                        //cout << j;
                                        for(int k = 0; k < graph[j+1].size(); k++)
                                        {
                                                int letNode = graph[j+1][k];
                                                if(letNode != letterNode){
                                                        auto it = remove(graph[j+1].begin(), graph[j+1].end(), letNode);
                                                        graph[j+1].erase(it, graph[j+1].end());
                                                        nodePointers[letNode]--;
                                                        k--;
                                                }
                                        }
                                        break;
                                }

                        }
                        //check if the word can be spelled
                        numSink = 0;
                        for (int i = 0; i < wordLen; i++)
                        {
                                int letterNode = numDice + i + 1;
                                if (nodePointers[letterNode] > 0)
                                {
                                        numSink++;
                                }
                        }

                }
                        if (numSink != wordLen)
                        {
                                cout << "Cannot spell " << word << endl;
                        }
                        else
                        {
                                //output the dice used in order
                                for (int i = 0; i < diceUsed.size(); i++)
                                {
                                        cout << diceUsed[i];
                                        if (i < diceUsed.size() - 1)
                                        {
                                                cout << ",";
                                        }
                                }
                                cout << ": " << word << endl;
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
