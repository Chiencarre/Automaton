//
// Created by lenna on 19/04/2020.
//

#include <vector>
#include <string>
#include "State.h"
#include "Transition.h"
//#include "utils.h"


#ifndef UNTITLED_AUTOMATON_H
#define UNTITLED_AUTOMATON_H


class Automaton {
private:
    std::string alphabet;
    std::vector<State> states;
    std::vector<int> initial_states; //Value in the vector is the index of the state in the state vector
    std::vector<int> final_states; //Value in the vector is the index of the state in the state vector

public:

    Automaton();
    Automaton(std::string p_file_name);

    void setAlphabet(std::string p_alphabet);

    void setStates(std::vector<State> p_states);

    std::string getAlphabet();

    std::string print();

    bool isWordAccepted(std::string p_word);

    void minimize();

    void makeDeterministic();

    void makeEFree();

    ~Automaton();
};


#endif //UNTITLED_AUTOMATON_H
