#include<fstream>
#include<iostream>
#include<string>
#include "Core.h"
#define DEBUG 1
#define MAX_WORD_NUM 20000
#define MAX_WORD_LONG 30
#define BUF_SIZE 1000
using namespace std;

char words[MAX_WORD_NUM][MAX_WORD_LONG];
char *pwords[MAX_WORD_NUM];
char *result[MAX_WORD_NUM];
int wordnum = 0;
char head = '\0', tail = '\0';
bool enable_loop = false, maxword = false, maxchar = false;

void getwords(char *buff, int size) {
    int i, length, ifword;
    for (i = 0; i < size; i++) {
        length = 0;
        ifword = 0;
        while ((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z') && i < size) {
            if (buff[i] >= 'A' && buff[i] <= 'Z') buff[i] = buff[i] - 'A' + 'a';
            words[wordnum][length++] = buff[i];
            ifword = 1;
            i++;
        }
        if (ifword) {
            pwords[wordnum] = words[wordnum];
            wordnum++;
        }
    }
}


int main(int argc, char *argv[]) {
    string s;
    int cnt, size, i;

    ofstream outf;
    ifstream inf;

    cnt = 1;
    while (cnt < argc - 1) {
        if (DEBUG) {
            cout << argv[cnt] << argv[cnt][0] << argv[cnt][1] << argv[cnt][2] << endl;
        }
        if (argv[cnt][0] == '-' && argv[cnt][1] == 'w' && argv[cnt][2] == '\0') { //-w
            if (maxword) {
                cout << "Repeated parameter -w." << endl;
                exit(1);
            }
            else if (maxchar) {
                cout << "Conflicting parameters -w -c." << endl;
                exit(1);
            }
            maxword = true;
        }
        else if (argv[cnt][0] == '-' && argv[cnt][1] == 'c' && argv[cnt][2] == '\0') { //-c
            if (maxchar) {
                cout << "Repeated parameter -c." << endl;
                exit(1);
            }
            else if (maxword) {
                cout << "Conflicting parameters -w -c." << endl;
                exit(1);
            }
            maxchar = true;
        }
        else if (argv[cnt][0] == '-' && argv[cnt][1] == 'r' && argv[cnt][2] == '\0') { //-r
            if (enable_loop) {
                cout << "Repeated parameter -r." << endl;
                exit(1);
            }
            enable_loop = true;
        }

        else if (argv[cnt][0] == '-' && argv[cnt][1] == 'h' && argv[cnt][2] == '\0') { //-h
            if (head != '\0') {
                cout << "Repeated parameter -h." << endl;
                exit(1);
            }
            cnt++;
            if (cnt == argc) {
                cout << "missing parameter." << endl;
                exit(1);
            }
            head = argv[cnt][0];
            if (argv[cnt][1] != '\0' || !((head >= 'a' && head <= 'z') || (head >= 'A' && head <= 'Z'))) {
                cout << "Illegal parameter " << argv[cnt] << endl;
                exit(1);
            }
            if (head >= 'A' && head <= 'Z') head = head - 'A' + 'a';
        }
        else if (argv[cnt][0] == '-' && argv[cnt][1] == 't' && argv[cnt][2] == '\0') { //-t
            if (tail != '\0') {
                cout << "Repeated parameter -t." << endl;
                exit(1);
            }
            cnt++;
            if (cnt == argc) {
                cout << "missing parameter." << endl;
                exit(1);
            }
            tail = argv[cnt][0];
            if (argv[cnt][1] != '\0' || !((tail >= 'a' && tail <= 'z') || (tail >= 'A' && tail <= 'Z'))) {
                cout << "Illegal parameter " << argv[cnt] << endl;
                exit(1);
            }
            if (tail >= 'A' && tail <= 'Z') tail = tail - 'A' + 'a';
        }
        else {
            cout << "Illegal parameter." << endl;
            exit(1);
        }
        cnt++;
    }

    if (DEBUG) printf("input file path=%s\n", argv[cnt]);
    inf.open(argv[cnt]);
    if (!inf) {
        cout << "File does not exist." << endl;
        exit(1);
    }


    cnt = 0;
    while (getline(inf, s)) {
        //outf << s << '\n';
        //cout << s << endl;
        size = s.length();
        char buff[BUF_SIZE + 2];
        for (i = 0; i < size; i++) buff[i] = s[i];
        buff[i] = '\0';
        //char *buff = s.c_str();
        getwords(buff, size);
    }
    inf.close();


    /*for (int i = 0; i < wordnum; i++) {
        cout << words[i] << endl;
    }
    */

    cout << "head=" << head << "  tail=" << tail << "  enable_loop=" << enable_loop << "  maxword=" << maxword << "  maxchar=" << maxchar << endl;
    int len = 0;
    if (maxword) {
        len = gen_chain_word(pwords, wordnum, result, head, tail, false);
    }
    else if (maxchar) {
        len = gen_chain_char(pwords, wordnum, result, head, tail, false);
    }
    else {
        cout << "missing parameter -w or -c." << endl;
        exit(1);
    }

    outf.open("solution.txt");
    cout << len << endl;
    for (int i = 0; i < len; i++) {
        cout << result[i] << endl;
        outf << result[i] << '\n';
    }
    outf.close();
    return 0;
}