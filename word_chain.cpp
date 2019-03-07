#include<fstream>
#include<iostream>
#include<string>
#define DEBUG 1
#define MAX_WORD_NUM 2000
#define MAX_WORD_LONG 30
#define BUF_SIZE 1000
using namespace std;

char words[MAX_WORD_NUM][MAX_WORD_LONG];
int wordnum = 0;

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
        if (ifword) wordnum++;
    }
}


int main(int argc, char *argv[]) {
    string s;
    int cnt, size, i;

    //璇诲啓鏂囦欢
    ofstream outf;
    ifstream inf;

    if (DEBUG) printf("argc=%d\n", argc);
    if (argc < 2) {
        cout << "The number of parameters is incorrect." << endl;
        exit(1);
    }

    cnt = 1;
    while (argv[cnt][0] == '-' && cnt <= argc) {
        cnt++;
        cout << cnt << '\n' << endl;
    }

    if (DEBUG) printf("input file path=%s\n", argv[cnt]);
    inf.open(argv[cnt]);
    if (!inf) {
        cout << "File does not exist." << endl;
        exit(1);
    }

    outf.open("solution.txt");
    cnt = 0;
    while (getline(inf, s)) {
        outf << s << '\n';
        cout << s << endl;
        size = s.length();
        char buff[BUF_SIZE + 2];
        for (i = 0; i < size; i++) buff[i] = s[i];
        buff[i] = '\0';
        //char *buff = s.c_str();
        getwords(buff, size);
    }
    inf.close();
    outf.close();

    for (int i = 0; i < wordnum; i++) {
        cout << words[i] << endl;
    }


    return 0;
}
