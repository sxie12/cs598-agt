#include <bits/stdc++.h>

using namespace std;

typedef long long int LLI;
typedef pair<int,int> PII;

#define _ ios_base::sync_with_stdio(0);
#define debug
#define x first
#define y second
#define MXN 100005

const int inf = 0x3f3f3f3f;
const int mod = 1e9+7;
const double eps = 1e-8; 

// for each hand, play strat if H and if T

struct strat {
    string strats[2];
};

const int NUM_LEAVES = 9;

const int n[2] = {5, 9};
const string p1s[5] = {"21", "20", "1 2", "1 1", "1 0"};
const string p2s[9] = {"221", "220", "121", "120", "021", "020", "21 ", "11 ", "01 "};
const PII outcomes[4] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}}; // heads = 0, tails = 1
const int leaves[NUM_LEAVES] = {8, 9, 4, 5, 13, 14, 11, 12, 7};

int total[111][111];
double ev[111][111];
vector<string> v[2];
vector<strat> htv[2]; // H/T strat for player i 
int edges[22][3];
int part[22];
bool leaf[22];

// turn, state, player one strat, player two strat, player 1 flip, player 2 flip, player 1 bet, player 2 bet
void move(int turn, int cur, int p1a, int p2a, int p1f, int p2f, int p1b, int p2b) {
    // 0 = fold, 1 = check, 2 = raise
    assert(cur != -1);
    int pos = part[cur]-1, mov;
    cout << turn << " " << cur << " "  << pos << " " << p1a << " " << p2a << " " << p1f << " " << p2f << "\n";
    cout.flush();
    if (turn & 1) {
        // player 2
        mov = htv[1][p2a].strats[p2f][pos] - '0';
        if (!mov) {
            // fold
            total[p1a][p2a] += p2b;
        } else if (mov == 1) {
            // check
            if (leaf[cur]) {
                if (p1f != p2f) {
                    if (p1f > p2f) {
                        total[p1a][p2a] += p2b;
                    } else {
                        total[p1a][p2a] -= p1b;
                    }
                }
            } else {
                move(0, edges[cur][1], p1a, p2a, p1f, p2f, p1b, p2b);
            }
        } else {
            // raise
            assert(mov == 2);
            move(0, edges[cur][2], p1a, p2a, p1f, p2f, p1b, p2b+1);
        }
    } else {
        // player 1
        mov = htv[0][p1a].strats[p1f][pos] - '0';
        if (!mov) {
            // fold
            total[p1a][p2a] -= p1b;
        } else if (mov == 1) {
            // check
            if (leaf[cur]) {
                if (p1f != p2f) {
                    if (p1f > p2f) {
                        total[p1a][p2a] += p2b;
                    } else {
                        total[p1a][p2a] -= p1b;
                    }
                }
            } else {
                move(1, edges[cur][1], p1a, p2a, p1f, p2f, p1b, p2b);
            }
        } else {
            // raise
            assert(mov == 2);
            move(1, edges[cur][2], p1a, p2a, p1f, p2f, p1b+1, p2b);
        }
    }
}

void set_strats(int player) {
    for (int i = 0; i < n[player]; ++i)
        for (int j = 0; j < n[player]; ++j)
            htv[player].push_back(strat{{v[player][i], v[player][j]}});
    cout << "size of player " << player << " strats is " << htv[player].size() << "\n";
}

void get_ev() {
    for (int i = 0; i < htv[0].size(); ++i) {
        for (int j = 0; j < htv[1].size(); ++j) {
            ev[i][j] = (double) total[i][j] / 4.0;
            printf("%.3lf ", ev[i][j]);
        }
        printf("\n");
    }
}

int main() { _
    for (int i = 0; i < n[0]; ++i)
        v[0].push_back(p1s[i]);
    for (int i = 0; i < n[1]; ++i)
        v[1].push_back(p2s[i]);

    set_strats(0);
    set_strats(1);
    cout.flush();

    // create tree
    memset(edges, -1, sizeof(edges));
    edges[0][1] = 2;
    edges[0][2] = 1;
    edges[1][0] = 5;
    edges[1][1] = 4;
    edges[1][2] = 3;
    edges[2][1] = 7;
    edges[2][2] = 6;
    edges[3][0] = 9;
    edges[3][1] = 8;
    edges[6][0] = 12;
    edges[6][1] = 11;
    edges[6][2] = 10;
    edges[10][0] = 14;
    edges[10][1] = 13;
    // player 1
    part[0] = 1;
    part[3] = 2;
    part[6] = 3;
    // player 2
    part[1] = 1;
    part[2] = 2;
    part[10] = 3;
    // leaves
    for (int i = 0; i < NUM_LEAVES; ++i) leaf[leaves[i]] = 1;

    for (int i = 0; i < htv[0].size(); ++i)
        for (int j = 0; j < htv[1].size(); ++j)
            for (int k = 0; k < 4; ++k)
                move(0, 0, i, j, outcomes[k].x, outcomes[k].y, 1, 1);

    get_ev();

    return 0;
}
