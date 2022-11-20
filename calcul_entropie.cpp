#include <iostream>
#include <fstream>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

struct cuvant{
    char cuv[6];
    bool isValid;
    double entropy;
};

static const int N = 11454;
cuvant cuvinte[N];
int feedback[5];
char lastWordTried[6];
int appearaceOnPosition[26][5];
int orderOnPosition[4][26][26];//pozitii(toate inafara de prima, idicii 0-3, pozitiile 2-5) - litera care apare - litera dupa care apare

char litereGhicite[6] = "\0";
int pozitiiLitereGhicite[5];
int nrLitereGhicite = 0;

void fillInMatrix(){
    for(int i = 0; i < N; i++){
        if(cuvinte[i].isValid){
            for(int j = 0; j < 5; j++){
                int currentLetter = cuvinte[i].cuv[j] - 'A';
                appearaceOnPosition[currentLetter][j]++;
                if(j){
                    int prevLetter = cuvinte[i].cuv[j-1] - 'A';
                    orderOnPosition[j-1][currentLetter][prevLetter]++;
                }
            }
        }
    }
}

void resetMatrix(){
    for(int i = 0; i<4; i++){
        for(int j = 0 ;j < 26; j++){
            for(int k = 0; k < 26; k++){
                orderOnPosition[i][j][k] = 0;
            }
        }
    }
    for(int i = 0; i < 26; i++){
        for(int j = 0; j < 5; j++){
            appearaceOnPosition[i][j] = 0;
        }
    }
}

void calculateEntropy(){
    double ent;
    for(int i = 0; i < N; i++){
        if(cuvinte[i].isValid){
            ent = 0;
            for(int j = 0; j < 5; j++){
                int currentLetter = cuvinte[i].cuv[j] - 'A';
                if(j == 0){
                    double chance = double(appearaceOnPosition[currentLetter][j])/N;
                    if(chance != 0){
                        ent += chance * log2(1/chance);
                    }
                }else{
                    int prevLetter = cuvinte[i].cuv[j-1] - 'A';
                    //literele de pe poz j care au inaintea lor o litera specifica / nr de aparitii a literei curente pe poz j
                    double chance = double(orderOnPosition[j-1][currentLetter][prevLetter])/appearaceOnPosition[currentLetter][j];
                    if(chance != 0){
                        ent += chance * log2(1/chance);
                    }
                }
            }
            cuvinte[i].entropy = ent;
        }
    }
}

void calculateEntropy2(){
    double ent;
        for(int i = 0; i < N; i++){
            if(cuvinte[i].isValid){
                ent = 0;
                for(int j = 0; j < 5; j++){
                    int currentLetter = cuvinte[i].cuv[j] - 'A';
                    double chance = double(appearaceOnPosition[currentLetter][j])/N;
                    if(chance != 0){
                        ent += chance * log2(1/chance);
                    }
                }
                cuvinte[i].entropy = ent;
            }
        }
}

void sortByEntropy(){
    for (int i = 0; i < N - 1; i++){
        for(int j = i + 1; j < N; j++){
            if(cuvinte[i].isValid && cuvinte[j].isValid){
                if(cuvinte[i].entropy < cuvinte[j].entropy){
                    cuvant aux = cuvinte[i];
                    cuvinte[i] = cuvinte[j];
                    cuvinte[j] = aux;
                }
            }
        }
    }
}

void getFeeback(){
    ifstream in("temp_feedback.txt");
    in>>feedback[0];
    in>>feedback[1];
    in>>feedback[2];
    in>>feedback[3];
    in>>feedback[4];
    in.close();
}

void filterWords(){
    for(int i = 0; i < N; i++){
        if(cuvinte[i].isValid){
            for(int j = 0; j < 5; j++){
                if(feedback[j] == 2){
                    if(cuvinte[i].cuv[j] != lastWordTried[j]){
                        cuvinte[i].isValid = false;
                    }
                }
                else if(feedback[j] == 1){
                    if((cuvinte[i].cuv[j] == lastWordTried[j])||(strchr(cuvinte[i].cuv,lastWordTried[j]) == NULL)){
                        cuvinte[i].isValid = false;
                    }
                }else if(feedback[j] == 0){
                    char litera = lastWordTried[j];
                    int nrApar = 0;
                    for(int x = 0;  x < 5; x++){
                        if(litera == lastWordTried[x]){
                            nrApar++;
                        }
                    }
                    if(nrApar == 1){
                        if(strchr(cuvinte[i].cuv,litera)!=NULL){
                            cuvinte[i].isValid = false;
                        }
                    }else{
                        for(int ind = 0; ind < 5; ind++){
                            if(cuvinte[i].cuv[ind] == litera && ind == j){
                                cuvinte[i].isValid = false;
                            }
                        }
                    }
                }
            }
        }
    }
}

void fillInGuessedLetters(){
    for(int i = 0; i < 5; i++){
        if(feedback[i] == 2){
            litereGhicite[nrLitereGhicite] = lastWordTried[i];
            pozitiiLitereGhicite[nrLitereGhicite] = i;
            nrLitereGhicite++;
            litereGhicite[nrLitereGhicite] = '\0';
        }
    }
}

void readWordList(){
    ifstream in("temp_entropy.txt");
    for(int i = 0; i < N; i++){
        in.get(cuvinte[i].cuv,6);
        in>>cuvinte[i].isValid>>cuvinte[i].entropy;
        in.get();
    }
    in.close();
}

void getLastWordTried(){
    for(int i = 0; i < N; i++){
        if(cuvinte[i].isValid){
            strcpy(lastWordTried,cuvinte[i].cuv);
            return;
        }
    }
}

int main(){
    getFeeback();
    readWordList();
    getLastWordTried();
    filterWords();
    resetMatrix;
    fillInMatrix();
    calculateEntropy();
    sortByEntropy();
    ofstream out("temp_entropy.txt");
    for(int i = 0; i < N; i++){
        out<<cuvinte[i].cuv<<' '<<cuvinte[i].isValid<<' '<<cuvinte[i].entropy<<endl;
    }
    out.close();
}