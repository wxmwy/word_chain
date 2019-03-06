#include<fstream>
#include<iostream>
#include<string>   
#define DEBUG 1

using namespace std;

int main(int argc, char *argv[]){
    string s;
    int cnt = 1;
    
    ofstream outf;
    ifstream inf;
    
    if(DEBUG) printf("argc=%d\n", argc);
	if(argc	< 2){
		cout << "The number of parameters is incorrect." <<endl;
		exit(1); 
	}

    while(argv[cnt][0] == '-' && cnt <= argc){
    	cnt++;
    	cout << cnt << '\n' <<endl;
	}
	
	if(DEBUG) printf("input file path=%s\n", argv[cnt]);
    inf.open(argv[cnt]);
    if(!inf){
    	cout << "File does not exist." <<endl;
		exit(1); 
	}   
	
	outf.open("solution.txt"); 
    while (getline(inf, s)){
        outf << s << '\n';
        cout << s << endl;
    }
    inf.close();
    outf.close();
    return 0;
}

