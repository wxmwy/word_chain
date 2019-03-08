#include<fstream>
#include<iostream>
#include<string>
#include "Core.h"
using namespace std;

int main(int argc, char *argv[]) {

    char* input[] = { "Algebra",
    "Apple",
    "Zoo",
    "Elephant",
    "Under",
    "Fox",
    "Dog",
    "Moon",
    "Leaf",
    "Trick",
    "Pseudopseudohypoparathyroidism" };

    char* result[11] = { 0 };
    /* 调用Core中封装好的函数 */
    int len = gen_chain_char(input, 11, result, 0, 'm', false);
    cout << len << endl;
    for (int i = 0; i < len; i++) {
        cout << result[i] << endl;
    }
    return 0;
}
