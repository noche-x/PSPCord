#pragma once
#include "AppState.hpp"
#include <vector>

class StateManager{
    public:
        StateManager();

        void changeState(AppState* state);
        void pushState(AppState* state);
        void popState();
        AppState* getState();

        void update();
        void draw();

        AppState* currentState();
        int returnVal();
    private:
        std::vector<AppState*> states;
};

extern StateManager g_StateManager;