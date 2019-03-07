#include<fstream>
#include<iostream>
#include<string>
#define DEBUG 1
#define MAX_WORD_NUM 2000
#define MAX_WORD_LONG 30

using namespace std;

char words[MAX_WORD_NUM][MAX_WORD_LONG];
char result[MAX_WORD_NUM][MAX_WORD_LONG];
int wordnum = 0;
char head = '\0', tail = '\0';
bool enable_loop = false, maxword = false, maxchar = false;


int gen_chain_word(char* words[], int len, char* result[], char head, char tail, bool enable_loop);
int gen_chain_char(char* words[], int len, char* result[], char head, char tail, bool enable_loop);

void getwords(char *buff, int size){
	int i, length, ifword;
	for(i=0; i<size; i++){
		length = 0;
		ifword = 0;
		while((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z') && i<size){
			if(buff[i] >= 'A' && buff[i] <= 'Z') buff[i] = buff[i]-'A'+'a';
			words[wordnum][length++] = buff[i];
			ifword = 1;
			i++;
		}
		if(ifword) wordnum++;
	}
}

int main(int argc, char *argv[]){
    string s;
    int cnt, size, i;
    
    ofstream outf;
    ifstream inf;
    
    if(DEBUG) printf("argc=%d\n", argc);
	if(argc	< 2){
		cout << "The number of parameters is incorrect." <<endl;
		exit(1); 
	}

	cnt = 1;
    /*while(argv[cnt][0] == '-' && cnt <= argc){
    	cnt++;
    	cout << cnt << '\n' <<endl;
	}*/
	while(cnt < argc){
		if(argv[cnt][0] == '-' && argv[cnt][1] == 'w' && argv[cnt][2] == '\0'){ //-w
			if(maxword){
				cout << "Repeated parameter -w." <<endl;
				exit(1); 
			}
			else if(maxchar){
				cout << "Conflicting parameters -w -c." <<endl;
				exit(1); 
			}
			maxword = true;
		}
		else if(argv[cnt][0] == '-' && argv[cnt][1] == 'c' && argv[cnt][2] == '\0'){ //-c
			if(maxchar){
				cout << "Repeated parameter -c." <<endl;
				exit(1); 
			}
			else if(maxword){
				cout << "Conflicting parameters -w -c." <<endl;
				exit(1); 
			}
			maxchar = true;
		}
		else if(argv[cnt][0] == '-' && argv[cnt][1] == 'r' && argv[cnt][2] == '\0'){ //-r
			if(enable_loop){
				cout << "Repeated parameter -t." <<endl;
				exit(1); 
			}
			enable_loop = true;
		}

		else if(argv[cnt][0] == '-' && argv[cnt][1] == 'h' && argv[cnt][2] == '\0'){ //-h
			if(head != '\0'){
				cout << "Repeated parameter -h." <<endl;
				exit(1); 
			}
			cnt++;
			if(cnt==argc){
				cout << "missing parameter." <<endl;
				exit(1); 
			}
			head = argv[cnt][0];
			if(argv[cnt][1] != '\0' || !(head >= 'a' && head <= 'z') || (head >= 'A' && head <= 'Z')){
				cout << "Illegal parameter." <<endl;
				exit(1); 
			}
			if(head >= 'A' && head <= 'Z') head = head - 'A' + 'a';
		}
		else if(argv[cnt][0] == '-' && argv[cnt][1] == 't' && argv[cnt][2] == '\0'){ //-t
			if(tail != '\0'){
				cout << "Repeated parameter -t." <<endl;
				exit(1); 
			}
			cnt++;
			if(cnt==argc){
				cout << "missing parameter." <<endl;
				exit(1); 
			}
			tail = argv[cnt][0];
			if(argv[cnt][1] != '\0' || !(tail >= 'a' && tail <= 'z') || (tail >= 'A' && tail <= 'Z')){
				cout << "Illegal parameter." <<endl;
				exit(1); 
			}
			if(tail >= 'A' && tail <= 'Z') tail = tail - 'A' + 'a';
		}
		else{
			cout << "Illegal parameter." <<endl;
			exit(1); 
		}
		cnt++;
	}
	
	if(DEBUG) printf("input file path=%s\n", argv[cnt]);
    inf.open(argv[cnt]);
    if(!inf){
    	cout << "File does not exist." <<endl;
		exit(1); 
	}   
	
	outf.open("solution.txt");
	cnt = 0;
    while (getline(inf, s)){
        outf << s << '\n';
        cout << s << endl;
        size = s.length();
        char buff[size+2];
        for(i=0;i<size;i++) buff[i]=s[i];
        buff[i] = '\0';
        //char *buff = s.c_str();
        getwords(buff, size);
    }
    inf.close();
    outf.close();

    for(int i=0; i<wordnum; i++){
    	cout << words[i] <<endl;
    }


    return 0;
}
