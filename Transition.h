//
// Created by lenna on 19/04/2020.
//

#ifndef UNTITLED_TRANSITION_H
#define UNTITLED_TRANSITION_H


#include <string>
class State;
class Transition {
private:
    State *to;
    char label;

    Transition();

public:

    Transition(State *p_to, char p_label);

    void setTo(State *p_to);

    char getLabel();

    State* getTo();

    ~Transition();

};


#endif //UNTITLED_TRANSITION_H
