//Owen Youngblood and Charlie Boddicker
//Project 5 word dice
//this code takes a list of letters and a word and determines if one letter from each of those lists can spell the word.
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <algorithm>
#include <climits>

using namespace std;

bool bfs(vector<vector<int>> graph, vector<vector<int>> capacity, vector<int>& parent, int source, int sink) 
{
    deque<int> frontier;
    vector<bool> visited(graph.size(), false);
    frontier.push_back(source);
    visited[source] = true;

    while (!frontier.empty()) 
    {
        int node = frontier.front();
        frontier.pop_front();

        for (int neighbor : graph[node]) 
	{
            if (!visited[neighbor] && capacity[node][neighbor] > 0) 
	    {
                parent[neighbor] = node;
                visited[neighbor] = true;

                if (neighbor == sink) 
		{
			return true;
		}
                frontier.push_back(neighbor);
            }
        }
    }
    return false;
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

    for (int i = 0; i < words.size(); i++) 
    {
        word = words[i];
        int numDice = dice.size();
        int wordLen = word.size();
        int totalNodes = numDice + wordLen + 2;
        vector<vector<int>> graph(totalNodes);
        vector<vector<int>> capacity(totalNodes, vector<int>(totalNodes, 0));
        int source = 0;
        int sink = totalNodes - 1;

        //add edges from source to each die node
        for (int j = 0; j < numDice; j++) 
	{
            graph[source].push_back(j + 1);
            graph[j + 1].push_back(source);
            capacity[source][j + 1] = 1;
        }

        //for each letter in the word, create a unique letter node
        unordered_map<char, vector<int>> letterNodeMap;
        for (int j = 0; j < wordLen; j++) 
	{
            letterNodeMap[word[j]].push_back(numDice + j + 1);
        }

        //add edges from dice nodes to the letter nodes if the dice contain the letter
        for (int j = 0; j < numDice; j++) 
	{
            for (int k = 0; k < dice[j].size(); k++) 
	    {
                char letter = dice[j][k];
                if (letterNodeMap.find(letter) != letterNodeMap.end()) 
		{
                    for (int letterNode : letterNodeMap[letter]) 
		    {
                        graph[j + 1].push_back(letterNode);
                        graph[letterNode].push_back(j + 1);
                        capacity[j + 1][letterNode] = 1;
                    }
                }
            }
        }

        //add edges from each letter node to the sink
        for (int j = 0; j < wordLen; j++) 
	{
            int letterNode = numDice + j + 1;
            graph[letterNode].push_back(sink);
            graph[sink].push_back(letterNode);
            capacity[letterNode][sink] = 1;
        }
        vector<int> parent(totalNodes);
        int maxFlow = 0;

        while (bfs(graph, capacity, parent, source, sink)) 
	{
            int pathFlow = INT_MAX;

            //find minimum capacity in the augmenting path
            for (int v = sink; v != source; v = parent[v]) 
	    {
                int u = parent[v];
                pathFlow = min(pathFlow, capacity[u][v]);
            }

            //update capacities along the path
            for (int v = sink; v != source; v = parent[v]) 
	    {
                int u = parent[v];
                capacity[u][v] -= pathFlow;
                capacity[v][u] += pathFlow;
            }

            maxFlow += pathFlow;
        }

        //check if the word can be spelled
        if (maxFlow < wordLen) 
	{
            cout << "Cannot spell " << word << endl;
        } 
	else 
	{
            vector<int> diceUsed(wordLen, -1);

            //determine the dice used for each letter
            for (int j = 0; j < numDice; j++) 
	        {
                for (int letterNode : graph[j + 1]) 
		        {
                    if (letterNode >= numDice + 1 && letterNode <= numDice + wordLen) 
		            {
                        if (capacity[letterNode][j + 1] > 0) 
			            {
                            diceUsed[letterNode - numDice - 1] = j;
                        }
                    }
                }
            }
            for (int j = 0; j < diceUsed.size(); j++) 
	        {
                cout << diceUsed[j];
                if (j < diceUsed.size() - 1) 
		        {
                    cout << ",";
                }
            }
            cout << ": " << word << endl;
        }
    }

    return 0;
}

