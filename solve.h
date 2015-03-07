#ifndef SOLVE_H_INCLUDED
#define SOLVE_H_INCLUDED

#include <bits/stdc++.h>
#include "stemFile.h"
#include "air.h"

#define in freopen("solve_in.txt", "r", stdin);
#define out freopen("solve_out.txt", "w", stdout);
#define bug(x) printf("Line %d: >>>>>>>>>\n", (x));
#define pb push_back

using namespace std;

typedef vector<int> VI;
const int maxnode = 3*(int)1e5 + 100;//字典树结点个数
const int sigma_size = 28;//字符集大小
const int maxSen = 16555;

int ch[maxnode][sigma_size];
int cntWord;
int vis[maxSen][2], val[maxnode];
vector<int> WordPos[maxnode];//对应字典树每个结点1个vector<int> 存相应单词对应的倒排索引
vector<string> Sen;
int sz;
vector<vector<int> > res;
vector<int> tmp1, tmp2, tmp, ans;
vector<string> tinq;
char str[111];
int cnt;

struct Trie { //构建字典树
    //单词插入到字典树中
    int idx(char cch) {
        if(cch == '\'')
            return 26;
        if(cch == '-')
            return 27;
        return cch-'a';
    }
    void init() {
        sz = 1;
        val[0] = 0;
        cntWord = 0;
        memset(ch[0], 0, sizeof ch[0]);
        WordPos[0].clear();
    }
    void insert(string s, int v, int pos) {
        int u = 0;
        int len = s.size();
        for(int i = 0; i < len; i++) {
            int c = idx(s[i]);
            if(!ch[u][c]) {
                val[sz] = 0;
                memset(ch[sz], 0, sizeof ch[sz]);
                WordPos[sz].clear();
                ch[u][c] = sz++;
            }
            u = ch[u][c];
        }
        if(val[u] == 0)
            cntWord++;
        val[u] += v;
        if(WordPos[u].size() == 0 || WordPos[u][WordPos[u].size()-1] != pos)
            WordPos[u].pb(pos);
    }
    void insert(string s, int v, vector<int> res) {
        int u = 0;
        int len = s.size();
        for(int i(0); i < len; i++) {
            int c = idx(s[i]);
            if(!ch[u][c]) {
                val[sz] = 0;
                memset(ch[sz], 0, sizeof ch[sz]);
                WordPos[sz].clear();
                ch[u][c] = sz++;
            }
            u = ch[u][c];
        }
         if(val[u] == 0)
            cntWord++;
        val[u] += v;
        WordPos[u] = res;
    }
    bool search(string s, vector<int> &res) {
        res.clear();
        int u = 0;
        int len = s.size();
        for(int i = 0; i < len; i++) {
            int c = idx(s[i]);
            u = ch[u][c];
            if(!u)
                return false;
        }
        res = WordPos[u];
        return val[u];
    }
}solver;

extern vector<int> query(vector<string> inq);
extern void preProcess();
extern void dfs(int u, string str, FILE *f1);

extern bool isLetter(char cch) {
    return (cch >= 'a' && cch <= 'z') || (cch >= 'A' && cch <= 'Z');
}

extern char toLower(char cch) {
    return cch >= 'A' && cch <= 'Z' ? cch - 'A' + 'a' : cch;
}


//对文本进行分句同时构建字典树，返回句子个数
static void collectWord(string s, int pos) {
    char ch;
    string str;
    int len = s.size();
    for(int i = 0; i < len; ) {
        str = "";
        ch = s[i];
        if(isLetter(ch)) {
            ch = tolower(ch);
            str += ch;
        } else {
            i++;
            continue;
        }
        int j;
        for(j = i+1; j < len && (isLetter(ch = s[j]) || ch == '\'' || ch == '-'); j++) {
            char tch;
            if(isLetter(ch))
                str += ch;
            if(j+1 >= len)
                break;
            tch = s[j+1];

            if(ch == '-' ||ch == '\'') {
                if(!isLetter(tch))
                    break;
                else str += ch;
            }
        }
        solver.insert(str, 1, pos);
        i = j+1;
    }
}
static int divideSen(FILE* f1) {
    string s;
    char ch;
    Sen.clear();
    int senCnt = 0;
    while(1) {
        int ok = 0;
        s = "";
        while((ch = fgetc(f1)) != EOF && (ch == ' ' || ch == '\n'))
            ;
        if(ch == EOF)
            break;
        char last = 0;
        while(1) {
            if(ch == EOF)
                break;
            s += ch;
            if(ch == '"') {
                if(!ok)
                    ok = 1;
                else {
                    if(last == '.' || last == '!' || last == '?')
                        break;
                    else ok = 0;
                }
            } else if(ch == '.' || ch == '!' || ch == '?' || ch == ';') {
                if(!ok)
                    break;
            }
            if(ch != ' ' && ch != '\n')
                last = ch;
            ch = fgetc(f1);
        }
        Sen.pb(s);
//        cout<<s<<endl;
        collectWord(s, Sen.size());
    }
    return Sen.size();
}
extern void preProcess() {

    FILE * f1 = fopen("solve_in.txt", "r");
    solver.init();
    cout<<divideSen(f1)<<endl;
    fclose(f1);
}
static bool getSen(string s, vector<int> &res) {
    return solver.search(s, res);
}
vector<int> operator + (const vector<int> &a, const vector<int> &b) {
    vector<int> c;
    for(int i = 0; i < (int)a.size(); i++)
        c.pb(a[i]);
    for(int i = 0; i < (int)b.size(); i++)
        c.pb(b[i]);
    sort(c.begin(), c.end());
    c.erase(unique(c.begin(), c.end()), c.end());
    return c;
}
extern void dfs(int u, string str, FILE *f1) {
    if(val[u]) {
        fprintf(f1, "%s", str.data());
        fprintf(f1, " %d", WordPos[u].size());
        for(int i = 0; i <(int) WordPos[u].size(); i++){
            fprintf(f1, " %d", WordPos[u][i]);
        }
        fprintf(f1, "\n");
    }
    for(int c = 0; c < sigma_size; c++) {
        if(!ch[u][c])
            continue;
        char cch;
        if(c <= 25)
            cch = (char)(c+'a');
        else cch = (c == 26 ? '\'' : '-');
        dfs(ch[u][c], str+cch, f1);
    }
}

extern vector<int> query(vector<string> inq) {
    tinq.clear();
    int n;
    stemmer z;
    cnt++;
    n = inq.size();
    ans.clear();
    res.clear();
    char *str = (char *)malloc(sizeof(char)*111);
    for(int i(0); i < n; i++) {
        tinq.pb(inq[i]);
        strcpy(str, inq[i].data());
        int len = strlen(str);
        for(int j = 0; j < len; j++)
            str[j] = toLower(str[j]);
        getSen(str, tmp1);
        str[stem(&z, str, len - 1) + 1] = 0;
        tinq.pb(str);
        getSen(str, tmp2);
        tmp = tmp1 + tmp2;
        res.pb(tmp);
    }
    for(int i = 0; i < n; i++) {
        tmp = res[i];
        for(int j = 0; j < (int)tmp.size(); j++) {
            int u = tmp[j];
            if(vis[u][1]!=cnt)
                vis[u][0] = 1, vis[u][1] = cnt;
            else vis[u][0]++;
            if(i == n-1) {
                if(vis[u][0] == n)
                    ans.pb(u);
            }
        }
    }
    return ans;
}

#endif // SOLVE_H_INCLUDED
