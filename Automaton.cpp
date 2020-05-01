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
        std::cout << "Creating automaton : opening failed";
    }
}

Automaton::~Automaton() {}

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
            //Go on to the next label of the word
            label = std::next(label, 1);
            current_states = current_states_2;
            current_states_2.clear();
        }
    }
    int index = 0;
    //If the word is read entirely but we are not on a final state
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

    //Fill the table with each destination state for each letter of the alphabet
    for (auto c = alphabet.cbegin(); c != alphabet.cend(); ++c) {
        for (int j = 0; j < number_states; ++j) {
            State current_state = states.at(j);
            std::vector<Transition> transitions = current_state.getTransitions();
            for (int k = 0; k < transitions.size(); ++k) {
                //If there is a transition from this state with this letter, push back the to_state.
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

    int current_group_index;
    int previous_group_index;
    std::vector<int> column;
    std::vector<int> current_group;
    std::vector<int> previous_group;
    do { //Fill the table with the new group value
        current_group.clear();
        previous_group.clear();
        previous_group_index = table.size() - 1;

        //For each char of the alphabet, put the to_state new group
        for (int c = 0; c < alphabet_size; ++c) {
            for (int i = 0; i < number_states; ++i) {
                int to_state = table.at(c).at(i);
                column.push_back(table.at(previous_group_index).at(to_state));
            }
            table.push_back(column);
            column.clear();
        }
        current_group_index = table.size();


        int classe_name = 0; //Used in the map to later create the new states
        std::map<std::string, int> groups;

        //Create the new class
        for (int j = 0; j < number_states; ++j) {
            std::string classe = "";
            for (int i = previous_group_index; i < current_group_index; ++i) {
                classe += std::to_string(table.at(i).at(j));
            }
            std::pair<std::map<std::string, int>::iterator, bool> ptr;
            ptr = groups.insert({classe, classe_name});
            //If classe has been added, ie, if classe was not already present in the map
            if (ptr.second) { //Push the class to the current group
                current_group.push_back(classe_name);
                classe_name++;
            } else { //Push the already exisiting class
                current_group.push_back(groups.at(classe));
            }
            classe = "";
        }
        table.push_back(current_group);
        previous_group = table.at(previous_group_index);
    } while (current_group != previous_group);

    //Now modify the automaton !!
    std::set<int> state_set;
    std::vector<State> new_states;
    //Create the new states
    for (int i = 0; i < current_group.size(); ++i) {
        std::pair<std::set<int>::iterator, bool> ptr;
        //Group is now a new state
        ptr = state_set.insert(current_group.at(i));
        //If it has been added as a state
        if (ptr.second) { //Check its initial and final characteristics
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

    //Create the new transitions
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
}

void Automaton::makeDeterministic() {
    std::vector<State> new_states;
    std::vector<int> new_finals;

    std::vector<std::vector<std::string>> table;
    //Key is the state and value is the index of the future state
    std::map<std::string, int> map_next_states;
    std::map<std::string, int> map_states;
    std::map<std::string, int> map_current_states;

    std::string next_state = "";
    std::vector<std::string> string_next_states;

    //Next_states are all the initial states
    for (int i = 0; i < initial_states.size(); ++i) {
        next_state += std::to_string(states.at(initial_states.at(i)).getNumber()) + "-";
    }
    map_current_states.insert({next_state, map_states.size()});
    map_states = map_current_states;

    do {
        map_next_states.clear();
        //For each state in the current states
        for (auto current_state = map_current_states.cbegin();
             current_state != map_current_states.cend(); ++current_state) {

            //For each letter of the alphabet
            for (auto c = alphabet.cbegin(); c != alphabet.cend(); ++c) {
                //Parse every state the current state has been made off
                std::vector<std::string> every_states;
                utils::splitString(current_state->first, every_states, '-');
                //For each of theses states
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
        //Insert all the next states in the map_states
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
        //State is onitial only if all the states it has been made off are initials
        for (int i = 0; i < results.size(); ++i) {
            isInit = isInit && states.at(std::stoi(results.at(i))).isInit();
        }
        int index = 0;
        //State is final if one of the states it has been made off is final
        while (index < results.size() && isFin == false) {
            isFin = states.at(std::stoi(results.at(index))).isFin();
            if (isFin) {
                new_finals.push_back(std::stoi(results.at(index)));
            }
            index++;
        }
        new_states.push_back(State(it->second, isInit, isFin));
        //Add the index of the state to the initial states if initial
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

void Automaton::removeELoop() {

    bool found = false;
    std::vector<State> current_states;
    int last_state;
    std::vector<State> next_states;
    std::set<State *> all_states;
    std::vector<State *> states_with_Etrans;

    //Get all the states with epsilon transitions
    for (int i = 0; i < states.size(); ++i) {
        if (!states.at(i).getETransition().empty()) {
            states_with_Etrans.push_back(&states.at(i));
        }
    }

    do {
        State *begin_state = states_with_Etrans.at(0); //State from where the loop starts, if there is one
        int begin_state_index = begin_state->getNumber();
        current_states.push_back(*begin_state);

        do {
            next_states.clear();
            for (int l = 0; l < current_states.size(); ++l) {
                State current_state = current_states.at(l);
                std::vector<Transition> eTrans = current_states.at(l).getETransition();
                //If the current state has epsilon transitions
                if (eTrans.size() != 0) {
                    for (int j = 0; j < eTrans.size(); ++j) {
                        Transition current_trans = eTrans.at(j);
                        //If the destination state of this epsilon transitions has epsilon transitions and the destination state is not the beginning of the loop
                        if (current_trans.getTo()->getNumber() != begin_state->getNumber() &&
                            current_trans.getTo()->getETransition().size() !=
                            0) { //Add it to the all_states set (added only if it hasn't been treated to avoid looping)
                            std::pair<std::set<State *>::iterator, bool> ptr = all_states.insert(current_trans.getTo());
                            //If it has been added, add it to the next states to treat
                            if (ptr.second) {
                                next_states.push_back(*current_trans.getTo());
                            }
                        } else if (current_trans.getTo()->getNumber() ==
                                   begin_state->getNumber()) { //If the destination is the beginning of the loop,
                            //the current state is the last state of the loop
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

        //If a loop has been found
        if (found) {
            found = false;
            std::vector<Transition> transitions_to_add;

            //For each state of the automaton
            for (int i = 0; i < states.size(); ++i) {
                State *current_state = &states.at(i);

                //For each transition of each state
                for (int j = 0; j < current_state->getTransitions().size(); ++j) {
                    Transition *current_trans = &current_state->getTransitions().at(j);
                    //If the destination is the beginning of the loop and it will not lead to a epsilon loop, add the transition with the destination being the end of the loop
                    if (current_trans->getTo()->getNumber() == begin_state->getNumber() &&
                        current_state->getNumber() != begin_state->getNumber() &&
                        (current_state->getNumber() != last_state || current_trans->getLabel() != '#')) {
                        transitions_to_add.push_back(Transition(&states.at(last_state), current_trans->getLabel()));
                        current_trans->setTo(nullptr);

                    } else //Copy the transition from the beginning of the loop to the end of the loop (if it is not a epsilon transition on the end)
                        if (current_state->getNumber() == begin_state->getNumber() &&
                               (current_trans->getTo()->getNumber() != last_state ||
                                current_trans->getLabel() != '#')) {
                        states.at(last_state).addTransition(
                                Transition(current_trans->getTo(), current_trans->getLabel()));
                        current_trans->setTo(nullptr);

                    } else //Mark the remaining transitions looping on the end to nullptr
                        if (current_trans->getTo()->getNumber() == begin_state->getNumber() ||
                               current_state->getNumber() == begin_state->getNumber()) {
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
            states.at(last_state).setInit(begin_state->isInit() || states.at(last_state).isInit());
            states.at(last_state).setFin(begin_state->isFin() || states.at(last_state).isFin());

            //Redo transition because a state is going to be removed and change the vector
            for (int j = 0; j < states.size(); ++j) {
                std::vector<Transition> new_trans;
                for (int k = 0; k < states.at(j).getTransitions().size(); ++k) {
                    Transition *current_transition = &states.at(j).getTransitions().at(k);
                    if (current_transition->getTo()->getNumber() <= begin_state_index) {
                        new_trans.push_back(Transition(current_transition->getTo(), current_transition->getLabel()));
                    } else {
                        new_trans.push_back(Transition(&states.at(current_transition->getTo()->getNumber() - 1),
                                                       current_transition->getLabel()));
                    }
                }
                states.at(j).setTransitions(new_trans);
                new_trans.clear();
            }

            //Remove the state representing the beginning of the loop
            states.erase(states.begin() + begin_state_index);
            for (int i = 0; i < states.size(); ++i) {
                if (states.at(i).getNumber() >= begin_state_index) {
                    states.at(i).setNumber(states.at(i).getNumber() - 1);
                }
            }
            states_with_Etrans.clear();
            //Redo the states with epsilon transition.
            for (int i = 0; i < states.size(); ++i) {
                if (!states.at(i).getETransition().empty()) {
                    states_with_Etrans.push_back(&states.at(i));
                }
            }
        }

        //If it is not empty, reinitialize everything. else, end of while
        if (states_with_Etrans.size() != 0) {
            begin_state = states_with_Etrans.front();
            states_with_Etrans.erase(states_with_Etrans.begin());
            begin_state_index = begin_state->getNumber();
            current_states.clear();
            next_states.clear();
            all_states.clear();
        }

    } while (!states_with_Etrans.empty());
}

void Automaton::deleteETransition() {
    std::vector<State *> states_with_Etrans;
    std::vector<Transition> transitions_to_add;

    //Get all states with ETrans
    for (int i = 0; i < states.size(); ++i) {
        if (states.at(i).getETransition().size() != 0) {
            states_with_Etrans.push_back(&states.at(i));
        }
    }

    while (states_with_Etrans.size() != 0) {
        State *current_state = *states_with_Etrans.begin();
        //For every epsilon transition
        for (int i = 0; i < current_state->getETransition().size(); ++i) {
            Transition current_Etrans = current_state->getETransition().at(i);

            current_Etrans.getTo()->setInit(current_state->isInit());
            if (current_Etrans.getTo()->isFin()) {
                current_state->setFin(true);
            }
            //For each state
            for (int j = 0; j < states.size(); ++j) {
                //For each transition
                for (int k = 0; k < states.at(j).getTransitions().size(); ++k) {
                    Transition current_trans = states.at(j).getTransitions().at(k);
                    //If the destination is the beginning of the epsilon transition, make it point to the destination as well
                    if (current_trans.getTo()->getNumber() == current_state->getNumber()) {
                        transitions_to_add.push_back(Transition(current_Etrans.getTo(), current_trans.getLabel()));
                    }
                }
                for (int l = 0; l < transitions_to_add.size(); ++l) {
                    states.at(j).addTransition(transitions_to_add.at(l));
                }
                transitions_to_add.clear();
            }
        }
        states_with_Etrans.erase(states_with_Etrans.begin());
    }

    //Delete all epsilon transitions
    for (int i = 0; i < states.size(); ++i) {
        std::vector<Transition> transition_state = states.at(i).getTransitions();
        transition_state.erase(std::remove_if(transition_state.begin(),
                                              transition_state.end(), [](Transition &transition) {
                    return transition.getLabel() == '#';
                }), transition_state.end());
        states.at(i).setTransitions(transition_state);
    }
}

void Automaton::makeEFree() {
    removeELoop();
    deleteETransition();
}




