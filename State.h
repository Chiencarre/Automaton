//
// Created by lenna on 19/04/2020.
//

#ifndef UNTITLED_STATE_H
#define UNTITLED_STATE_H


#include <vector>
#include "Transition.h"

class State {
private:
    int number;
    bool isInitial;
    bool isFinal;
    std::vector<Transition> transitions;

    State();

public:
    State(int p_number, bool p_isInitial, bool p_isFinale);

    State(int p_number, bool p_isInitial, bool p_isFinale, std::vector<Transition> p_transitions);

    ~State();

    bool isInit();

    bool isFin();

    std::vector<Transition>& getTransitions();

    std::vector<Transition> getETransition();

    int getNumber();

    void setTransitions(std::vector<Transition> p_transitions);

    void addTransition(Transition p_transition);

    void setInit(bool p_init);

    void setFin(bool p_fin);

    void setNumber(int p_number);

    std::string print();
};


#endif //UNTITLED_STATE_H
