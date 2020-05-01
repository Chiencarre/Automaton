#ifndef UNTITLED_AUTOMATONHELPER_H
#define UNTITLED_AUTOMATONHELPER_H


#include <string>
#include "Automaton.h"

void readWordsFromFile(Automaton p_automaton, std::string p_words_file_name, std::string p_results_file_name);

void writeAutomatonToFile(Automaton p_automaton, std::string p_result_file_name);


#endif //UNTITLED_AUTOMATONHELPER_H
