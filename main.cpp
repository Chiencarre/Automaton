#include <iostream>
#include "AutomatonHelper.h"
#include "utils.h"

int main(int argc, char **argv) {
    if (argc <= 2 || argc > 5) {
        std::cerr << "The program needs at least three arguments.\n";
        usage();
        return 1;
    } else {
        int mode = *argv[1] - '0';
        std::string automaton_file_name = argv[2];
        //Word test
        if (mode == 0) {
            if (argc == 5) {
                std::string words_file_name = argv[3];
                std::string result_file_name = argv[4];
                Automaton automaton = Automaton(automaton_file_name);
                readWordsFromFile(automaton, words_file_name, result_file_name);
                std::cout << "Words read perfectly fine. Check the file " << result_file_name << ".\n";
            } else {
                usage();
            }
        } else //Minimization
        if (mode == 1) {
            if(argc == 4){
                std::string result_file_name = argv[3];
                Automaton automaton = Automaton(automaton_file_name);
                automaton.minimize();
                writeAutomatonToFile(automaton, result_file_name);
                std::cout << "Minimization completed. Check the file " << result_file_name << ".\n";
            } else {
                usage();
            }

        } else //Make deterministic
        if (mode == 2) {
            if(argc == 4){
                std::string result_file_name = argv[3];
                Automaton automaton = Automaton(automaton_file_name);
                automaton.makeDeterministic();
                writeAutomatonToFile(automaton, result_file_name);
                std::cout << "Determinisation completed. Check the file " << result_file_name << ".\n";
            } else {
                usage();
            }

        } else //Removal of epsilon transitions
        if (mode == 3) {
            if(argc ==4){
                std::string result_file_name = argv[3];
                Automaton automaton = Automaton(automaton_file_name);
                automaton.makeEFree();
                writeAutomatonToFile(automaton, result_file_name);
                std::cout << "Epsilon transtions removed. Check the file " << result_file_name << ".\n";
            } else {
                usage();
            }

        } else {
            std::cerr << "This mode does not exist.\n";
            usage();
            return 1;
        };
        return 0;
    }
}
