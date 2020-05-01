#include <iostream>
#include "AutomatonHelper.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "The program needs at least three arguments.\n";
        std::cout << "The program allows the creation of an automaton, reading words from a file, its determinisation, "
                     "its minimisation and the removal of epsilon transitions, if it has some.\n"
                     "Usage: lanceur <mode> <file_path_one> <file_path_two> <file_path_3>\n"
                     "\t mode 0: reading words from file\n"
                     "\t\t file_path_one: file with the automaton description.\n"
                     "\t\t file_path_two: file with the words to read.\n"
                     "\t\t file_path_three: file with the reading words results.\n\n"
                     "\t mode 1: determinisation\n"
                     "\t\t file_path_one: file with the automaton description.\n"
                     "\t\t file_path_two: file with the resulting automaton.\n"
                     "\t\t file_path_three: UNNECESSARY.\n\n"
                     "\t mode 2: minimisation\n"
                     "\t\t file_path_one: file with the automaton description.\n"
                     "\t\t file_path_two: file with the resulting automaton.\n"
                     "\t\t file_path_three: UNNECESSARY.\n\n"
                     "\t mode 3: removal of epsilon transitions\n"
                     "\t\t file_path_one: file with the automaton description.\n"
                     "\t\t file_path_two: file with the resulting automaton.\n"
                     "\t\t file_path_three: UNNECESSARY.\n\n";
        return 1;
    } else {
        int mode = *argv[1] - '0';
        std::string automaton_file_name = argv[2];

        //Word test
        if (mode == 0) {
            std::string words_file_name = argv[3];
            std::string result_file_name = argv[4];
            Automaton automaton = Automaton(automaton_file_name);
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
            AutomatonHelper::writeAutomatonToFile(automaton, result_file_name);
        } else //Removal of epsilon transitions
        if(mode ==3){
            std::string result_file_name = argv[3];
            Automaton automaton = Automaton(automaton_file_name);
            automaton.makeEFree();
            AutomatonHelper::writeAutomatonToFile(automaton,result_file_name);
        }
        return 0;
    }
}
