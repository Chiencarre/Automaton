//
// Created by lenna on 19/04/2020.
//

#include <iostream>
#include <iterator>
#include "State.h"

State::State() {}

State::State(int p_number, bool p_isInitial, bool p_isFinal) :
        number(p_number), isInitial(p_isInitial), isFinal(p_isFinal) {
}

State::State(int p_number, bool p_isInitial, bool p_isFinal,std::vector<Transition> p_transitions) :
        number(p_number), isInitial(p_isInitial), isFinal(p_isFinal), transitions(p_transitions) {
}


std::string State::print() {
    std::string state = std::to_string(isInitial) + " " + std::to_string(isFin()) + "\n" + std::to_string(transitions.size()) + "\n";
    //std::cout << isInitial << " " << isFinal << "\n";
    //std::cout << transitions.size() << "\n";
    for (int i = 0; i < transitions.size(); ++i) {
        state += std::to_string(transitions.at(i).getTo()->getNumber()) + " " + transitions.at(i).getLabel() + "\n";
    }
    return state;
}

bool State::isInit() {
    return isInitial;
}

bool State::isFin() {
    return isFinal;
}

void State::setTransitions(std::vector<Transition> p_transitions){
    transitions = p_transitions;
}

std::vector<Transition>& State::getTransitions() {
    return transitions;
}

int State::getNumber() {
    return number;
}

State::~State() {}

std::vector<Transition> State::getETransition() {
    std::vector<Transition> eTrans;
    for(int i = 0; i < transitions.size(); ++i){
        if(transitions.at(i).getLabel() == '#'){
            eTrans.push_back(transitions.at(i));
        }
    }
    return eTrans;
}

void State::addTransition(Transition p_transition) {
    transitions.push_back(p_transition);
}

void State::deleteTransition(int p_it_transition) {
    //std::cout << transitions.at(p_it_transition).getTo()->getNumber() << " " << transitions.at(p_it_transition).getLabel();
    transitions.erase(transitions.begin() +p_it_transition);
}
