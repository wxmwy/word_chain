#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <Windows.h>

#define MAX_WORD_NUM 100000
#define MAX_WORD_LONG 300
using namespace std;

typedef int(*p_gen_chain_word)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
typedef int(*p_gen_chain_char)(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

char words[MAX_WORD_NUM][MAX_WORD_LONG];
char *pwords[MAX_WORD_NUM];
char *result[MAX_WORD_NUM];
int wordnum = 0;
char head = '\0', tail = '\0';
bool enable_loop = false, maxword = false, maxchar = false;

clock_t start, finish;

class FileHandle {
private:
    static void get_words(string buff) {
        int size = (int)buff.size();
        int i, length, ifword;
        for (i = 0; i < size; i++) {
            length = 0;
            ifword = 0;
            while (i < size && isalpha(buff[i])) {
                words[wordnum][length++] = (char)tolower(buff[i]);
                if (length > MAX_WORD_LONG - 2) {
                    cout << "Error: the length of some words is too long." << endl;
                    exit(1);
                }
                ifword = 1;
                i++;
            }
            if (ifword) {
                words[wordnum][length] = '\0';
                pwords[wordnum] = words[wordnum];
                wordnum++;
                if (wordnum > MAX_WORD_NUM - 2) {
                    cout << "Error: the word list is too long." << endl;
                    exit(1);
                }
            }

        }
    }

public:
    static void deal_file(int cnt, char *argv[]) {
        string s;
        ifstream inf;
        inf.open(argv[cnt]);
        if (!inf) {
            cout << "Error: file does not exist." << endl;
            exit(1);
        }
        while (getline(inf, s)) {
            get_words(s);
        }
        inf.close();
    }
};

class ParaHandle {
public:
    static int deal_par(int argc, char *argv[]) {
        int cnt;
        cnt = 1;
        if (argc == 1) {
            cout << "Error: Missing parameter." << endl;
            exit(1);
        }
        while (cnt < argc - 1) {
            if (argv[cnt][0] == '-' && argv[cnt][1] == 'w' && argv[cnt][2] == '\0') { //-w
                if (maxword) {
                    cout << "Error: Repeated parameter -w." << endl;
                    exit(1);
                }
                else if (maxchar) {
                    cout << "Error: Conflicting parameters -w -c." << endl;
                    exit(1);
                }
                maxword = true;
            }
            else if (argv[cnt][0] == '-' && argv[cnt][1] == 'c' && argv[cnt][2] == '\0') { //-c
                if (maxchar) {
                    cout << "Error: Repeated parameter -c." << endl;
                    exit(1);
                }
                else if (maxword) {
                    cout << "Error: Conflicting parameters -w -c." << endl;
                    exit(1);
                }
                maxchar = true;
            }
            else if (argv[cnt][0] == '-' && argv[cnt][1] == 'r' && argv[cnt][2] == '\0') { //-r
                if (enable_loop) {
                    cout << "Error: Repeated parameter -r." << endl;
                    exit(1);
                }
                enable_loop = true;
            }

            else if (argv[cnt][0] == '-' && argv[cnt][1] == 'h' && argv[cnt][2] == '\0') { //-h
                if (head != '\0') {
                    cout << "Error: Repeated parameter -h." << endl;
                    exit(1);
                }
                cnt++;
                if (cnt == argc - 1) {
                    cout << "Error: Missing parameter." << endl;
                    exit(1);
                }
                head = argv[cnt][0];
                if (argv[cnt][1] != '\0' || !((head >= 'a' && head <= 'z') || (head >= 'A' && head <= 'Z'))) {
                    cout << "Error: Illegal parameter " << argv[cnt] << endl;
                    exit(1);
                }
                if (head >= 'A' && head <= 'Z') head = head - 'A' + 'a';
            }
            else if (argv[cnt][0] == '-' && argv[cnt][1] == 't' && argv[cnt][2] == '\0') { //-t
                if (tail != '\0') {
                    cout << "Error: Repeated parameter -t." << endl;
                    exit(1);
                }
                cnt++;
                if (cnt == argc - 1) {
                    cout << "Error: Missing parameter." << endl;
                    exit(1);
                }
                tail = argv[cnt][0];
                if (argv[cnt][1] != '\0' || !((tail >= 'a' && tail <= 'z') || (tail >= 'A' && tail <= 'Z'))) {
                    cout << "Error: Illegal parameter " << argv[cnt] << endl;
                    exit(1);
                }
                if (tail >= 'A' && tail <= 'Z') tail = tail - 'A' + 'a';
            }
            else {
                cout << "Error: Illegal parameter " << argv[cnt] << endl;
                exit(1);
            }
            cnt++;
        }
        return cnt;
    }
};


int main(int argc, char *argv[])
{
    //start = clock();
    HINSTANCE CoreDLL = LoadLibrary("Core.dll");
    if (CoreDLL == NULL) {
        cout << "Error: File 'Core.dll' not found." << endl;
        exit(1);
    }

    p_gen_chain_word gen_chain_word = (p_gen_chain_word)GetProcAddress(CoreDLL, "gen_chain_word");
    p_gen_chain_char gen_chain_char = (p_gen_chain_char)GetProcAddress(CoreDLL, "gen_chain_char");
    if (gen_chain_word == NULL || gen_chain_char == NULL) {
        cout << "Error: Invalid file 'Core.dll'." << endl;
        exit(1);
    }

    int cnt;
    cnt = ParaHandle::deal_par(argc, argv);
    FileHandle::deal_file(cnt, argv);
    int len = 0;
    if (maxword) {
        try {
            len = gen_chain_word(pwords, wordnum, result, head, tail, enable_loop);
            ofstream outf;
            outf.open("solution.txt");
            //cout << len << endl;
            for (int i = 0; i < len; i++) {
                //cout << result[i] << endl;
                outf << result[i] << '\n';
            }
            outf.close();
        }
        catch (const char *erroe_message) {
            cout << erroe_message << endl;
        }
    }
    else if (maxchar) {
        try {
            len = gen_chain_char(pwords, wordnum, result, head, tail, enable_loop);
            ofstream outf;
            outf.open("solution.txt");
            //cout << len << endl;
            for (int i = 0; i < len; i++) {
                //cout << result[i] << endl;
                outf << result[i] << '\n';
            }
            outf.close();
        }
        catch (const char *erroe_message) {
            cout << erroe_message << endl;
        }
    }
    else {
        cout << "Error: Missing parameter -w or -c." << endl;
        exit(1);
    }


    FreeLibrary(CoreDLL);
    //finish = clock();
    //printf("%f seconds cost.\n", (double)(finish - start) / CLOCKS_PER_SEC);
    return 0;
}