#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <string.h>
class Game{
    private:
        static const int N = 11454; 
        char words[N][6], solution[6], input[6], precison[6];//matricea cu dictionarul -- cuvantul solutie -- input-ul jucatorului -- 
        int excludedLetters[26], numberOfLetters[26];
        const char bifa = char(251);

        bool isSolution(){
            for(int i = 0; i <= 4; i++){
                if(solution[i] != input[i]){
                    return false;
                }
            }
            return true;
        }

        void winGame(){
            std::cout<<"You guessed the word!";
        }

        int getInput(){
            std::cin.get(input,6);
            std::string overflow;
            std::getline(std::cin,overflow);
            for(int i = 0; i < 5; i++){
                if(input[i] >= 'a' && input[i] <= 'z'){
                    input[i] = input[i] - ('a' - 'A');
                }
            }
            return overflow.size();
            //std::cin.get();
        }

        bool isValidInput(){
            int i = 0;
            while(true){
                if(input[i] == '\0'){
                    break;
                }
                i++;
            }
            return((i==5) && wordBinarySearch(0, N-1, input));
        }

        bool wordBinarySearch(int st, int dr, char input[]){
            int m = (st + dr) / 2;
            if(st <= dr){
                if(!strcmp(input, words[m])){
                    return 1;
                }else if(strcmp(input, words[m]) > 0){
                    return wordBinarySearch(m + 1, dr, input);
                }else{
                    return wordBinarySearch(st, m - 1, input);
                }
            }
            return 0;
        }

        void getWord(){
            std::ifstream in("cuv.txt");
            for(int i = 0; i < N; i++){
                in.get(words[i],6);
                in.get();
            }
            srand(time(0));
            for(int i = rand()/10; i > 0; i--){
                rand();
            }
            float x = rand();
            x *= double(N) / float(RAND_MAX);
            for(int i = 0; i < 6; i++){
                solution[i] = words[int(x)][i];
            }
        }

        bool isInSolution(char a){
            for(int i = 0; i < 5; i++){
                if(solution[i] == a){
                    return true;
                }
            }
            return false;
        }

        void compareInput(){
            int copy[26];
            for(int i = 0; i < 26; i++){
                copy[i] = numberOfLetters[i];
            }
            for(int i = 0; i < 5; i++){
                precison[i] = 0;
            }

            for(int i = 0; i < 5; i++){
                if(input[i]==solution[i]){
                    precison[i] = bifa;
                    copy[input[i] - 65]--;
                }
            }

            for(int i = 0; i < 5; i++){
                if(!precison[i]){
                    if(isInSolution(input[i]) && (copy[input[i] - 65] != 0)){
                        precison[i] = '#';
                    }else{
                        excludedLetters[input[i] - 65] = 1;
                        precison[i] = 'x';
                    }
                }
            }
            for(int i = 0; i < 5; i++){
                std::cout<<precison[i];
            }
            std::cout<<std::endl;
            showPossibleLetters();
            //std::cout<<std::endl;
        }

        void showPossibleLetters(){
            for(int i = 0; i < 26; i++){
                if(!excludedLetters[i] || (numberOfLetters[i] != 0)){
                    std::cout<<char(i+65)<<' ';
                }
            }
            std::cout<<std::endl;
        }

    public:

        Game(){
            getWord();
            for(int i = 0; i < 26; i++){
                excludedLetters[i] = 0;
                numberOfLetters[i] = 0;
            }
            for(int i = 0; i < 5; i++){
                numberOfLetters[solution[i] - 65]++;
            }
        }

        void runGame(){
            std::cout<<"Guess a 5 letter word!"<<std::endl;
            while(true){
                if(getInput()){
                    std::cout<<"Invalid input"<<std::endl;
                    continue;
                }
                if(!isValidInput()){
                    std::cout<<"Invalid input"<<std::endl;
                    showPossibleLetters();
                }else if(isSolution()){
                    winGame();
                    std::cin.get();
                    break;
                }else{
                    compareInput();
                    //showPossibleLetters();
                }
            }
        }
};