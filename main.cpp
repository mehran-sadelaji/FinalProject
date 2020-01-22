#include<iostream>
#include<string>
#include<vector>
#include<time.h>

using namespace std;

struct Cinema;
struct Salone;
struct Sans;
struct Film;
struct Actor;

struct Cinema {
    string name;
    vector<Salone> salones;
};

struct Salone {
    int code;
    vector<Sans> scheduls;
};

struct Sans {
    time_t start;
    time_t end;
    Film film;
    int emptyCap;
};

struct Film {
    int length;
    string content;
    string summary;
    vector<Actor> actors;
};

struct Actor {
    string name;
};