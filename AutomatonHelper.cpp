//
// Created by lenna on 19/04/2020.
//

#include "AutomatonHelper.h"
#include <fstream>
#include <iostream>

AutomatonHelper::AutomatonHelper() {}


void AutomatonHelper::readWordsFromFile(Automaton p_automaton, std::string p_words_file_name,
                                        std::string p_results_file_name) {
    std::ifstream words_file(p_words_file_name);
    std::ofstream results_file(p_results_file_name);
    std::string word;
    bool isWordAccepted;
    if (words_file) {
        while (std::getline(words_file, word)) {
            isWordAccepted = p_automaton.isWordAccepted(word);
            results_file << isWordAccepted << "\n";
        }
    } else {
        std::cout << "Reading words : opening failed";
    }
}


AutomatonHelper::~AutomatonHelper() {}

void AutomatonHelper::writeAutomatonToFile(Automaton p_automaton, std::string p_result_file_name) {
    std::ofstream result_file(p_result_file_name);
    if(result_file){
        result_file << p_automaton.print();
    } else {
        std::cout << "Writing to file : opening failed";
    }
}





