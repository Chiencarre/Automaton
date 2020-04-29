#include <iostream>
#include "AutomatonHelper.h"

int main(int argc, char **argv) {
    if (argc == 0) {
        //Log
        return 1;
    } else {
        int mode = *argv[1] - '0';
        std::string automaton_file_name = argv[2];

        //Word test
        if (mode == 0) {
            std::string words_file_name = argv[3];
            std::string result_file_name = argv[4];
            Automaton automaton = Automaton(automaton_file_name);
            automaton.print();
            AutomatonHelper::readWordsFromFile(automaton, words_file_name, result_file_name);

        } else //Minimization
        if (mode == 1) {
            std::string result_file_name = argv[3];
            Automaton automaton = Automaton(automaton_file_name);
            automaton.minimize();
            AutomatonHelper::writeAutomatonToFile(automaton,result_file_name);
        } else //Make deterministic
        if(mode == 2){
            std::string result_file_name = argv[3];
            Automaton automaton = Automaton(automaton_file_name);
            automaton.makeDeterministic();
            std::cout << automaton.print();
            AutomatonHelper::writeAutomatonToFile(automaton, result_file_name);
        } else //Removal of epsilon transitions
        if(mode ==3){
            std::string result_file_name = argv[3];
            Automaton automaton = Automaton(automaton_file_name);
            automaton.makeEFree();
            std::cout << automaton.print();
        }
        return 0;
    }
}
