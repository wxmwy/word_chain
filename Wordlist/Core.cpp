#include <cstdio>
#include <cstring>
#include <cctype>
#include <queue>
#include <iostream>
#include "Core.h"
using namespace std;

int graph[30][30], words_index[30][30];
int in_degree[30], length[30], last[30];
queue<int> vertex;

int init_unweighted_graph(char* words[], int len)
{
    char *word;
    int start, end;

    for (int i = 0; i < len; i++) {
        word = words[i];
        start = tolower(word[0]) - 'a';
        end = tolower(word[strlen(word) - 1]) - 'a';
        if (start < 0 || start>29 || end < 0 || end >29) {
            cout << "The word contains illegal characters." << endl;
            exit(1);
        }
        if (start == end && graph[start][end]) {
            return -1; //带多个自环，报错
        }
        if (!graph[start][end]) {
            graph[start][end] = 1;
            words_index[start][end] = i;
            if (start != end) { //对于自环，入度不增加
                in_degree[end]++;
            }
        }
    }
    return 0;
}


int init_weighted_graph(char* words[], int len)
{
    char *word;
    int start, end, len_word;

    for (int i = 0; i < len; i++) {
        word = words[i];
        len_word = (int)strlen(word);
        start = tolower(word[0]) - 'a';
        end = tolower(word[len_word - 1]) - 'a';
        if (start < 0 || start>29 || end < 0 || end >29) {
            cout << "The word contains illegal characters." << endl;
            exit(1);
        }

        if (start == end && graph[start][end]) {
            return -1; //带多个自环，报错
        }
        if (!graph[start][end]) {
            graph[start][end] = len_word;
            words_index[start][end] = i;
            if (start != end) { //对于自环，入度不增加
                in_degree[end]++;
            }
        }
        else if (graph[start][end] < len_word) {
            graph[start][end] = len_word;
            words_index[start][end] = i;
        }
    }
    return 0;
}


int topo_sort()
{
    int j;

    for (int i = 0; i < 26; i++) {
        for (j = 0; j < 26 && in_degree[j] != 0; j++);
        if (j == 26) {
            return -1; //有环
        }
        in_degree[j] = -1;
        vertex.push(j);
        for (int k = 0; k < 26; k++) {
            if (j != k && graph[j][k]) {
                in_degree[k]--;
            }
        }
    }
    return 0;
}


void dp(char head, char tail)
{
    int v;

    memset(last, -1, sizeof(last));
    if (head) {
        v = tolower(head) - 'a';
        while (vertex.front() != v) {
            vertex.pop();
        }
        vertex.pop();
    }

    while (!vertex.empty()) {
        v = vertex.front();
        vertex.pop();
        for (int j = 0; j < 26; j++) {
            if (j != v && graph[j][v] && length[j] + graph[j][v] > length[v]) {
                if (!head || j == tolower(head) - 'a' || length[j] > 0) {
                    length[v] = length[j] + graph[j][v];
                    last[v] = j;
                }
            }
        }
        if (graph[v][v]) {
            length[v] += graph[v][v];
        }
        if (tail && v == tolower(tail) - 'a') {
            break;
        }
    }
}


int get_last_vertex(char tail)
{
    int max_length = -1, last_v;

    if (tail) {
        return tolower(tail) - 'a';
    }

    for (int i = 0; i < 26; i++) {
        if (length[i] > max_length) {
            max_length = length[i];
            last_v = i;
        }
    }
    return last_v;
}


int gen_result(char* words[], char* result[], char head, char tail)
{
    int v, count = 0;

    v = get_last_vertex(tail);
    while (last[v] != -1) {
        if (graph[v][v]) {
            result[count++] = words[words_index[v][v]];
        }
        result[count++] = words[words_index[last[v]][v]];
        v = last[v];
    }
    if (graph[v][v]) {
        result[count++] = words[words_index[v][v]];
    }

    if (head && tolower(head) - 'a' != v) {
        return 0;
    }

    for (int i = 0, j = count - 1; i < j; i++, j--) {
        char *tmp;
        tmp = result[i];
        result[i] = result[j];
        result[j] = tmp;
    }
    return count;
}


int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
    int ret;

    if ((ret = init_unweighted_graph(words, len)) < 0) {
        return ret;
    }

    if ((ret = topo_sort()) < 0) {
        return ret;
    }

    dp(head, tail);

    return gen_result(words, result, head, tail);
}


int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop)
{
    int ret;

    if ((ret = init_weighted_graph(words, len)) < 0) {
        return ret;
    }

    if ((ret = topo_sort()) < 0) {
        return ret;
    }

    dp(head, tail);

    return gen_result(words, result, head, tail);
}
