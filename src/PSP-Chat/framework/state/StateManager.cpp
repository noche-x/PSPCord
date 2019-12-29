#include "StateManager.hpp"

StateManager::StateManager(){

}

void StateManager::changeState(AppState* state){
    //Destroy the state chain and switch
    while (!states.empty()) {
		states.back()->shutdown();
		states.pop_back();
	}

    states.push_back(state);
    states.back()->enter(); //Any other special setup
}

void StateManager::pushState(AppState* state){
    if(!states.empty()){
        states.back()->pause();
    }

    states.push_back(state);
    states.back()->enter();
}

void StateManager::popState(){
    if(!states.empty()){
        states.back()->shutdown();
        states.pop_back();
    }

    if(!states.empty()){
        states.back()->resume();
    }
}

AppState* StateManager::getState(){
    if(!states.empty()){
        return states.back();
    }

    return 0;
}

void StateManager::update(){
    states.back()->update();
}

void StateManager::draw(){
    states.back()->draw();
}

int StateManager::returnVal() {
    return states.back()->returnVal();
}

AppState* StateManager::currentState(){
    return states.back();
}

StateManager g_StateManager = StateManager();