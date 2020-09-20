#include "minisat/core/Solver.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <list>
#include <string>
#include <queue>
#include <ostream>
#include <iostream>
#include <algorithm>

using namespace std;

struct Edge {
    int x;
    int y;
};
struct Edge edge1;
vector <Edge> edgecont;
vector<int> answer;

void collectedge(vector<int> bus, int mx) {
    for (int k = 0; k < mx; ++k) {
        Minisat::Solver solver;
        Minisat::vec <Minisat::Lit> assump1;
        vector <vector<Minisat::Lit>> container(mx);
        for (int i = 0; i < mx; ++i) {
            for (int j = 0; j < k; ++j) {
                container[i].push_back(Minisat::mkLit(solver.newVar()));
            }
        }
        for (int l = 0; l < k; ++l) {
            for (int i = 0; i < mx; ++i) {
                assump1.push(container[i][l]);
            }
            solver.addClause(assump1);
            assump1.clear();
        }
        for (int m = 0; m < mx; ++m) {
            for (int i = 0; i < k; ++i) {
                for (int j = i + 1; j < k; ++j) {
                    solver.addClause(~container[m][i], ~container[m][j]);
                }
            }
        }
        for (int n = 0; n < k; ++n) {
            for (int i = 0; i < mx; ++i) {
                for (int j = i + 1; j < mx; ++j) {
                    solver.addClause(~container[i][n], ~container[j][n]);
                }
            }
        }
        for (unsigned int h = 0; h < edgecont.size(); ++h) {
            for (int i = 0; i < k; ++i) {
                assump1.push(container[edgecont[h].x][i]);
                assump1.push(container[edgecont[h].y][i]);
            }
            solver.addClause(assump1);
            assump1.clear();

        }
        bool res = solver.solve();
        if (res) {
            for (int i = 0; i < mx; ++i) {
                for (int j = 0; j < k; ++j) {
                    if (Minisat::toInt(solver.modelValue(container[i][j])) == 0) {
                        answer.push_back(i);
                    }
                }
            }
            break;
        }
    }
}



void BuildAdj(vector <vector<int>> &adj, vector<int> taxi) {
    vector <pair<int, int>> edges;

    for (unsigned int i = 0; i < taxi.size() - 1; i += 2) {
        edges.push_back({taxi[i], taxi[i + 1]});
    }

    for (auto e : edges) {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }
}

bool path(vector<int> &par, int v1, int v2) {
    if (par[v2] != -1) {
        path(par, v1, par[v2]);
        cout << "-" << v2;
    } else {
        if (v2 == v1) {
            cout << v1;
        } else {
            cerr << "Error: Wrong edge" << endl;
            return false;
        }
    }
    return true;
}

int main() {
    string start;
    int VertexNum = -1;
    vector<int> taxi;
    vector <vector<int>> adj;
    bool flag = true;
    while (getline(cin, start)) {

        if (start[0] == 'V') {
            flag = true;
            taxi.clear();
            adj.clear();
            string max_num = start.substr(2, 25);
            VertexNum = stoi(max_num);
        }
        else if (start[0] == 'E' && VertexNum > 0) {
            char buf[999];
            strcpy(buf, start.c_str());
            char delim[] = "E{<,>}";
            char *token = strtok(buf, delim);
            while (token != NULL) {
                token = (strtok(nullptr, delim));
                if (token == NULL) break;
                int a;
                sscanf(token, "%d", &a);
                taxi.push_back(a);
            }

            if (taxi.size() == 0) {
                cerr << "Error" << endl;
                continue;
            }
            for (unsigned int t = 0; t < taxi.size(); t++) {
                if (taxi[t] >= VertexNum) {
                    cerr << "Error: wrong format" << endl;
                    flag = false;
                    taxi.clear();
                    break;
                }
            }
            if (!flag)continue;
            //adj = vector<vector<int>>(VertexNum);
            //BuildAdj(adj, taxi);
            for (unsigned int i = 0; i < taxi.size()-1; i = i + 2) {
                edge1.x = taxi[i];
                edge1.y = taxi[i + 1];
                edgecont.push_back(edge1);

            }
            collectedge(taxi, VertexNum);
            sort(answer.begin(),answer.end());
            for (unsigned int i = 0; i < answer.size(); i++) {
                cout << answer[i] <<" ";
            }cout<<endl;
            answer.clear();
            edgecont.clear();
            taxi.clear();
            start.clear();
            adj.clear();
        }
    }
}

