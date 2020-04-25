//
// Created by lenna on 19/04/2020.
//

#ifndef UNTITLED_TRANSITION_H
#define UNTITLED_TRANSITION_H


#include <string>
class State;
class Transition {
private:
    //TODO CHANGER INT EN POINTEUR
    State *to; //Index of state in the state vector
    char label;

    Transition();

public:

    Transition(State *p_to, char p_label);

    void setTo(State *p_to);

    void setLabel(char p_label);

    char getLabel();

    State* getTo();

    ~Transition();

};


#endif //UNTITLED_TRANSITION_H
