//
// Created by lenna on 19/04/2020.
//

#ifndef UNTITLED_AUTOMATONHELPER_H
#define UNTITLED_AUTOMATONHELPER_H


#include <string>
#include "Automaton.h"

class AutomatonHelper {
private:
public:

    AutomatonHelper();

    static void readWordsFromFile(Automaton p_automaton, std::string p_words_file_name, std::string p_results_file_name);

    static void writeAutomatonToFile(Automaton p_automaton, std::string p_result_file_name);

    ~AutomatonHelper();
};


#endif //UNTITLED_AUTOMATONHELPER_H
