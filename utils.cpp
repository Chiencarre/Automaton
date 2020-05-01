#include <vector>
#include <sstream>
#include <iostream>
#include "utils.h"


void splitString(std::string p_string, std::vector<std::string> &result, char delim) {
    std::stringstream ss(p_string);
    std::string token;
    while (std::getline(ss, token, delim)) {
        result.push_back(token);
    }
}

void usage() {
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
}



