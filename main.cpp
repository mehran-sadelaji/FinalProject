#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include"CppConsoleTable.hpp"

using namespace std;

struct Cinema;
struct Salone;
struct Sans;
struct Film;
struct Actor;
void addActor(Actor &);

// structs

struct Cinema {
    string name;
    vector<Salone*> salones;
};

struct Salone {
    int code;
    vector<Sans*> scheduls;
};

struct Sans {
    time_t start;
    time_t end;
    Film *film;
    int emptyCap;
};

struct Film {
    int length;
    string content;
    string summary;
    vector<Actor*> actors;
};

struct Actor {
    string name;
};


// data entries
vector<Actor> actors;
vector<Sans> sans;
vector<Salone> salones;
vector<Cinema> cinemas;


// functions

void addActor(Actor &actor) {
    bool flg = false;
    for(Actor &act : actors) {
        if(act.name == actor.name) {
            flg = 1;
        }
    }
    if(!flg) {
        actors.push_back(actor);
    }
}

int main() {
    
}