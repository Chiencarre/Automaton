//
// Created by lenna on 19/04/2020.
//
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "utils.h"
#include "Automaton.h"

Automaton::Automaton() {}

Automaton::Automaton(std::string p_file_name) {
    std::ifstream automaton_file(p_file_name);
    std::string line;
    int number_states;
    int number_transitions;
    std::vector<State> list_states;
    std::vector<Transition> list_transitions;
    initial_states = std::vector<int>();
    final_states = std::vector<int>();

    //Matrix transition : rows are the states, columns are the transitions
    std::vector<std::vector<std::string>> matrix_transition;
    if (automaton_file) {

        std::getline(automaton_file, line);
        //Get the alphabet
        alphabet = line;

        std::getline(automaton_file, line);
        //line represents now the number of state
        number_states = std::stoi(line);

        for (int i = 0; i < number_states; ++i) {
            //State state = State(i);
            std::getline(automaton_file, line);

            //line represents now a state
            //Initial and final
            auto it = line.cbegin();
            //Transform iterator into int
            bool isInitial = *it - '0';
            //Go to final information
            it = std::next(it, 2);
            bool isFinal = *it - '0';
            if (isInitial == 1) {
                initial_states.push_back(i);
            }
            if (isFinal == 1) {
                final_states.push_back(i);
            }

            std::getline(automaton_file, line);
            //Line is now the number of transition
            number_transitions = std::stoi(line);

            //Vector represents the list of transition of the current state
            std::vector<std::string> list_transitions_state;
            for (int j = 0; j < number_transitions; ++j) {
                std::getline(automaton_file, line);
                //Line is now a transition
                list_transitions_state.push_back(line);
            }
            matrix_transition.push_back(list_transitions_state);
            list_transitions_state.clear();
            State state = State(i, isInitial, isFinal);
            list_states.push_back(state);
            //list_transitions.clear();
        }
        states = list_states;

        //Now that all the states are created, create the transitions
        for (int i = 0; i < number_states; ++i) {
            for (int j = 0; j < matrix_transition.at(i).size(); ++j) {
                int to_index = matrix_transition.at(i).at(j)[0] - '0';
                char label = matrix_transition.at(i).at(j)[2];
                Transition transition = Transition(&states.at(to_index), label);
                list_transitions.push_back(transition);
            }
            states.at(i).setTransitions(list_transitions);
            list_transitions.clear();
        }

    } else {
        std::cout << " Creating automaton : opening failed";
    }
}

Automaton::~Automaton() {}

void Automaton::setAlphabet(std::string p_alphabet) {
    alphabet = p_alphabet;
}

void Automaton::setStates(std::vector<State> p_states) {
    states = p_states;
    for (int i = 0; i < states.size(); ++i) {
        if (states.at(i).isInit()) {
            initial_states.push_back(i);
        }
        if (states.at(i).isFin()) {
            final_states.push_back(i);
        }
    }
}

std::string Automaton::getAlphabet() {
    return alphabet;
}


std::string Automaton::print() {
    std::string automaton = alphabet + "\n" + std::to_string(states.size()) + "\n";

    for (int i = 0; i < states.size(); ++i) {
        automaton += states.at(i).print();
    }
    return automaton;
}

bool Automaton::isWordAccepted(std::string p_word) {
    auto label = p_word.cbegin();
    bool stuck = false;
    bool wordAccepted = false;
    std::vector<int> current_states = initial_states;
    std::vector<int> current_states_2;

    while (label != p_word.cend() && stuck == false) {
        //For each state in the current_states
        for (int i = 0; i < current_states.size(); ++i) {
            State current_state = states.at(current_states.at(i));

            //for each transition in the current state
            for (int j = 0; j < current_state.getTransitions().size(); ++j) {
                Transition current_transition = current_state.getTransitions().at(j);
                if (*label == current_transition.getLabel()) {
                    current_states_2.push_back(current_transition.getTo()->getNumber());
                }
            }
        }
        //If no transitions at all, then we're stuck
        if (current_states_2.size() == 0) {
            stuck = true;
        } else {
            label = std::next(label, 1);
            current_states = current_states_2;
            current_states_2.clear();
        }
    }
    int index = 0;
    while (index < current_states.size() && wordAccepted == false) {
        wordAccepted = states.at(current_states.at(index)).isFin();
        index++;
    }
    return wordAccepted;
}

void Automaton::minimize() {
    std::vector<std::vector<int>> table;
    std::vector<int> letter_info;
    int alphabet_size = alphabet.size();
    int number_states = states.size();

    //Fill the first part of the table
    for (auto c = alphabet.cbegin(); c != alphabet.cend(); ++c) {
        for (int j = 0; j < number_states; ++j) {
            State current_state = states.at(j);
            std::vector<Transition> transitions = current_state.getTransitions();
            for (int k = 0; k < transitions.size(); ++k) {
                //If there is a transition from this state with this letter, push back the to state.
                if (transitions.at(k).getLabel() == *c) {
                    letter_info.push_back(transitions.at(k).getTo()->getNumber());
                }
            }
        }
        //Once one state has been done, push letter_info into table and clear letter_info
        table.push_back(letter_info);
        letter_info.clear();
    }

    //Good until here
    std::vector<int> group;
    //If the state is final, group 0 in the next column. If not, group 1
    for (int i = 0; i < number_states; ++i) {
        if (states.at(i).isFin()) {
            group.push_back(0);
        } else {
            group.push_back(1);
        }
    }
    table.push_back(group);
    //First three columns done
    //Good until here

    int current_group_index;
    int previous_group_index;
    std::vector<int> column;
    std::vector<int> current_group;
    std::vector<int> previous_group;
    do { //Fill the table with the new group value
        current_group.clear();
        previous_group.clear();
        previous_group_index = table.size() - 1;

        for (int c = 0; c < alphabet_size; ++c) {
            for (int i = 0; i < number_states; ++i) {
                int to_state = table.at(c).at(i);
                column.push_back(table.at(previous_group_index).at(to_state));
            }
            table.push_back(column);
            column.clear();
        }
        current_group_index = table.size();
        int classe_name = 0;

        std::map<std::string, int> groups;
        for (int j = 0; j < number_states; ++j) {
            std::string classe = "";
            for (int i = previous_group_index; i < current_group_index; ++i) {
                classe += std::to_string(table.at(i).at(j));
            }
            std::pair<std::map<std::string, int>::iterator, bool> ptr;
            ptr = groups.insert({classe, classe_name});
            //If classe has been added, ie, if classe was not already present
            if (ptr.second) {
                current_group.push_back(classe_name);
                classe_name++;
            } else {
                current_group.push_back(groups.at(classe));
            }
            classe = "";
        }
        table.push_back(current_group);
        previous_group = table.at(previous_group_index);
    } while (current_group != previous_group);
    //Good until here

    //Now modify the automaton !!
    std::set<int> state_set;
    std::vector<State> new_states;
    //Create the new states
    for (int i = 0; i < current_group.size(); ++i) {
        std::pair<std::set<int>::iterator, bool> ptr;
        ptr = state_set.insert(current_group.at(i));
        if (ptr.second) {
            int index = 0;
            int isInit = 0;
            int isFin = 0;
            while (index < initial_states.size() && isInit == 0) {
                isInit = states.at(i).isInit();
                index++;
            }
            index = 0;
            while (index < final_states.size() && isFin == 0) {
                isFin = states.at(i).isFin();
                index++;
            }
            new_states.push_back(State(current_group.at(i), isInit, isFin));
        }
    }

    //Good until here
    //Create the new transitions
    std::set<int> copy_state_set = state_set;
    initial_states.clear();
    final_states.clear();
    std::vector<Transition> transitions;
    int index = 0;
    while (state_set.size() != 0) {
        int current_state = table.at(table.size() - 1).at(index);
        //If state is present in the set, it means it has not been treated yet
        if (state_set.find(current_state) != state_set.cend()) {
            for (int i = 0; i < alphabet_size; ++i) {
                int current_position = table.size() - alphabet_size - 1 + i;
                transitions.push_back(Transition(&states.at(table.at(current_position).at(index)), alphabet[i]));
            }
            new_states.at(index).setTransitions(transitions);
            transitions.clear();
            state_set.erase(current_state);
            //Redo initial_states and final_states
            if (new_states.at(index).isInit()) {
                initial_states.push_back(index);
            }
            if (new_states.at(index).isFin()) {
                final_states.push_back(index);
            }
        }
        index++;
    }
    states = new_states;
    //Good until here
}

void Automaton::makeDeterministic() {
    std::vector<State> new_states;
    std::vector<int> new_finals;

    std::vector<std::vector<std::string>> table;
    std::map<std::string, int> map_next_states;
    std::map<std::string, int> map_states;
    std::map<std::string, int> map_current_states;

    std::string next_state = "";
    std::vector<std::string> string_next_states;

    //initialisation
    for (int i = 0; i < initial_states.size(); ++i) {
        next_state += std::to_string(states.at(initial_states.at(i)).getNumber()) + "-";
    }
    map_current_states.insert({next_state, map_states.size()});
    map_states = map_current_states;

    do {
        map_next_states.clear();
        for (auto current_state = map_current_states.cbegin();
             current_state != map_current_states.cend(); ++current_state) {

            for (auto c = alphabet.cbegin(); c != alphabet.cend(); ++c) {
                std::vector<std::string> every_states;
                utils::splitString(current_state->first, every_states, '-');
                for (int i = 0; i < every_states.size(); ++i) {
                    State current_state = states.at(std::stoi(every_states.at(i)));
                    //For each transition of the current state
                    for (int j = 0; j < current_state.getTransitions().size(); ++j) {
                        Transition current_transition = current_state.getTransitions().at(j);
                        //Add the state only if label is same and not already present
                        if (current_transition.getLabel() == *c &&
                            next_state.find(std::to_string(current_transition.getTo()->getNumber())) ==
                            std::string::npos) {
                            next_state += std::to_string(current_transition.getTo()->getNumber()) + "-";
                        }
                    }
                }
                string_next_states.push_back(next_state);
                //If next state is not present in the current states, add it to next states
                if (map_states.find(next_state) == map_states.cend()) {
                    map_next_states.insert({next_state, map_states.size()});
                }
                next_state = "";
            }
            table.push_back(string_next_states);
            string_next_states.clear();
        }
        for (auto it = map_next_states.cbegin(); it != map_next_states.cend(); ++it) {
            map_states.insert(*it);
        }
        map_current_states = map_next_states;
    } while (map_next_states.size() != 0);

    //Create states
    for (auto it = map_states.cbegin(); it != map_states.cend(); ++it) {
        bool isFin = false;
        bool isInit = true;
        std::vector<std::string> results;
        utils::splitString(it->first, results, '-');
        for (int i = 0; i < results.size(); ++i) {
            isInit = isInit && states.at(std::stoi(results.at(i))).isInit();
        }
        int index = 0;
        while (index < results.size() && isFin == false) {
            isFin = states.at(std::stoi(results.at(index))).isFin();
            if (isFin) {
                new_finals.push_back(std::stoi(results.at(index)));
            }
            index++;
        }
        new_states.push_back(State(it->second, isInit, isFin));
        if (isInit) {
            initial_states = std::vector<int>(it->second);
        }
    }
    states = new_states;
    final_states = new_finals;

    //Now the transitions
    std::vector<Transition> transitions;
    for (int i = 0; i < table.size(); ++i) {
        for (int c = 0; c < alphabet.size(); ++c) {
            auto it = map_states.find(table.at(i).at(c));
            transitions.push_back(Transition(&states.at(it->second), alphabet[c]));
        }
        states.at(i).setTransitions(transitions);
        transitions.clear();
    }

}

void Automaton::makeEFree() {
    //TODO
    bool found = false;
    std::vector<State> current_states;
    int last_state;
    std::vector<State> next_states;
    std::set<State *> all_states;
    std::vector<State> new_states;
    //for (int i = 0; i < states.size(); ++i) {
    State begin_state = states.at(0);
    current_states.push_back(begin_state);
    do {
        next_states.clear();
        for (int l = 0; l < current_states.size(); ++l) {
            State current_state = current_states.at(l);
            std::vector<Transition> eTrans = current_states.at(l).getETransition();
            if (eTrans.size() != 0) {
                for (int j = 0; j < eTrans.size(); ++j) {
                    Transition current_trans = eTrans.at(j);
                    if (current_trans.getTo()->getNumber() != begin_state.getNumber() &&
                        current_trans.getTo()->getETransition().size() != 0) {
                        std::pair<std::set<State *>::iterator, bool> ptr = all_states.insert(current_trans.getTo());
                        if (ptr.second) {
                            next_states.push_back(*current_trans.getTo());
                        }
                    } else if (current_trans.getTo()->getNumber() == begin_state.getNumber()) {
                        last_state = current_state.getNumber();
                        found = true;
                    }
                }
            }
        }
        if (next_states.size() != 0) {
            current_states = next_states;
        }
    } while (next_states.size() != 0 && found == false);
    if (found) {

        //TODO !! intial state and final state for fusion
        //Last state has been found and a loop has been found
        std::vector<int> transitions_to_delete;
        std::vector<Transition> transitions_to_add;
        for (int i = 0; i < states.size(); ++i) {
            State *current_state = &states.at(i);

            for (int j = 0; j < current_state->getTransitions().size(); ++j) {
                Transition *current_trans = &current_state->getTransitions().at(j);

                if(current_trans->getTo()->getNumber() == begin_state.getNumber() && (current_state->getNumber() != last_state || current_trans->getLabel() != '#')){
                    transitions_to_add.push_back(Transition(&states.at(last_state), current_trans->getLabel()));
                    current_trans->setTo(nullptr);
                } else if (current_state->getNumber() == begin_state.getNumber() && (current_trans->getTo()->getNumber() != last_state || current_trans->getLabel() != '#')) {
                    states.at(last_state).addTransition(
                            Transition(current_trans->getTo(), current_trans->getLabel()));
                    current_trans->setTo(nullptr);
                } else if (current_trans->getTo()->getNumber() == begin_state.getNumber() || current_state->getNumber() == begin_state.getNumber()) {
                    current_trans->setTo(nullptr);
                }

            }
            std::vector<Transition> transition_state = current_state->getTransitions();
            //Remove transitions marked with nullptr
            transition_state.erase(std::remove_if(transition_state.begin(),
                                                  transition_state.end(), [](Transition &transition) {
                        return transition.getTo() == nullptr;
                    }), transition_state.end());
            current_state->setTransitions(transition_state);

            //Add the new transitions
            for (int k = 0; k < transitions_to_add.size(); ++k) {
                current_state->addTransition(transitions_to_add.at(k));
            }
            transitions_to_add.clear();
        }

    }

//}


}

