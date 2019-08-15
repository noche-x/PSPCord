#pragma once

class AppState{
    public:
        virtual void init() = 0;
        virtual void enter() = 0;
        virtual void shutdown() = 0;

        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void update() = 0;
        virtual void draw() = 0;

    protected:
		AppState() {}                             
};