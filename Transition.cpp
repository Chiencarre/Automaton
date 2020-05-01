//
// Created by lenna on 19/04/2020.
//

#include <iostream>
#include "Transition.h"

Transition::Transition() {}

Transition::Transition(State* p_to, char p_label) :
        to(p_to),label(p_label)  {
}

void Transition::setTo(State* p_to) {
    to = p_to;
}

char Transition::getLabel() {
    return label;
}

State* Transition::getTo() {
    return to;
}

Transition::~Transition() {}


