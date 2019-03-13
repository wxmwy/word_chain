#pragma once
#include <queue>
// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CREATDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CREATDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#define CREATDLL_API __declspec(dllexport)
#pragma warning( disable: 4251 )

using namespace std;

typedef struct {
    int index, start, end, weight;
    bool visit;
} WordInfo;


class CREATDLL_API Core {
private:

    int graph[30][30], words_index[30][30];
    int in_degree[30], length[30], last[30];
    queue<int> vertex;

    vector<WordInfo> start_words[26], end_words[26];
    deque<int> longest_word_chain, word_chain;
    int max_word_chain_length = 0;
    
    int get_last_vertex(char tail);
    void init_word_info(char* words[], int len, bool weighted);
    void dfs(int start, int depth);
    void dfs_with_end(int start, int depth, int end);
    void dfs_reverse(int end, int depth);
    
public:
    Core();
    int init_unweighted_graph(char* words[], int len);
    int init_weighted_graph(char* words[], int len);
    int topo_sort();
    void dp(char head, char tail);
    int gen_result(char* words[], char* result[], char head, char tail);
    int gen_loop_chain(char* words[], int len, char* result[], char head, char tail, bool weighted);
};

extern "C" CREATDLL_API int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
extern "C" CREATDLL_API int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
