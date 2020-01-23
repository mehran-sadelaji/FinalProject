#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include<chrono>
#include"CppConsoleTable.hpp"

using namespace std;
using ConsoleTable = samilton::ConsoleTable;

#define delay() for(int i = 0; i < (int) 2e9; ++i)

struct Cinema;
struct Salone;
struct Sans;
struct Film;
struct Actor;

int generateRandomNum();
bool addActor(Actor &);
bool addFilm(Film &);
bool addSans(Sans &, Salone *);
bool addSalone(Salone &, Cinema *);
bool addCinema(Cinema &);
bool butTicket(Sans &);
void readData();
void menu();
void actorMenu();
void filmMenu();
void sansMenu();



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
    string name;
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
vector<Film> films;
vector<Sans> sanses;
vector<Salone> salones;
vector<Cinema> cinemas;


// functions

int generateRandomNum() {
    return rand() % 1393 + 37;
}

bool addActor(Actor &actor) {
    bool flg = false;
    for(Actor &act : actors) {
        if(act.name == actor.name) {
            flg = true;
        }
    }
    if(!flg) {
        actors.push_back(actor);
    }
    return !flg;
}

bool addFilm(Film &film) {
    bool flg = false;
    for(Film &flm : films) {
        if(flm.name == film.name) {
            flg = true;
        }
    }
    if(!flg) {
        films.push_back(film);
    }
    return !flg;
}

bool addSans(Sans &sans, Salone *salone) {
    bool flg = false;
    for(int i = 0; i < salone->scheduls.size(); ++i) {
        if((salone->scheduls[i]->start <= sans.start && salone->scheduls[i]->end >= sans.start) || 
           (salone->scheduls[i]->start <= sans.end && salone->scheduls[i]->end >= sans.end)) {
               flg = true;
           }
    }
    if(!flg) {
        salone->scheduls.push_back(&sans);
        sanses.push_back(sans);
    }
    return !flg;
}

bool addSalone(Salone &salone, Cinema *cinema) {
    bool flg = false;
    for(int i = 0; i < cinema->salones.size(); ++i) {
        if(cinema->salones[i]->code == salone.code) {
            flg = true;
        }
    }
    if(!flg) {
        cinema->salones.push_back(&salone);
        salones.push_back(salone);
    }
    return !flg;
}

bool addCinema(Cinema &cinema) {
    bool flg = false;
    for(Cinema &cnm : cinemas) {
        if(cnm.name == cinema.name) {
            flg = true;
        }
    }
    if(!flg) {
        cinemas.push_back(cinema);
    }
    return !flg;
}

bool butTicket(Sans &sans) {
    bool flg = false;
    if(sans.emptyCap == 0) {
        flg = true;
    }
    if(!flg) {
        sans.emptyCap -= 1;
    }
    return !flg;
}

void readData() {
    freopen("data.dat", "r", stdin);
    int cntAct;
    cin >> cntAct;
    for(int i = 0; i < cntAct; ++i) {
        string name;
        cin >> name;
        Actor actor = {name};
        addActor(actor);
    }
    int cntFilm;
    cin >> cntFilm;
    for(int i = 0; i < cntFilm; ++i) {
        string name;
        int length;
        string content;
        string summary;
        int cntActorsOfFilm;
        cin >> name >> length >> content >> summary >> cntActorsOfFilm;
        Film film;
        film.name = name;
        film.length = length;
        film.content = content;
        film.summary = summary;
        for(int j = 0; j < cntActorsOfFilm; ++j) {
            int addres;
            scanf("%x ", &addres);
            film.actors.push_back((Actor *) addres);
        }
        addFilm(film);
    }
    int cntSans;
    cin >> cntSans;
    for(int i = 0; i < cntSans; ++i) {
        tm start;
        tm end;
        int month, day, hour, minute;
        cin >> month >> day >> hour >> minute;
        start.tm_mon = month;
        start.tm_mday = day;
        start.tm_hour = hour;
        start.tm_min = minute;

        cin >> month >> day >> hour >> minute;
        end.tm_mon = month;
        end.tm_mday = day;
        end.tm_hour = hour;
        end.tm_min = minute;

        int addres;
        scanf("%x", &addres);

        int empCap;
        cin >> empCap;

        int salonAddr;
        scanf("%x", &salonAddr);

        Sans sans = {mktime(&start), mktime(&end), (Film *) addres, empCap};
        addSans(sans, (Salone *) salonAddr);
    }
    int cntSalon;
    cin >> cntSalon;
    for(int i = 0; i < cntSalon; ++i) {
        Salone salone;
        int code = generateRandomNum();
        salone.code = code;
        int cntScheduls;
        cin >> cntScheduls;
        for(int j = 0; j < cntScheduls; ++j) {
            int address;
            scanf("%x ", &address);
            salone.scheduls.push_back((Sans *) address);
        }

        int cinemaAddr;
        scanf("%x", &cinemaAddr);

        addSalone(salone, (Cinema *) cinemaAddr);
    }
    int cntCinema;
    cin >> cntCinema;
    for(int i = 0; i < cntCinema; ++i) {
        string name;
        cin >> name;
        Cinema cinema;
        cinema.name = name;
        int cntSalone;
        cin >> cntSalone;
        for(int j = 0; j < cntSalone; ++j) {
            int saloneAddr;
            cin >> saloneAddr;
            cinema.salones.push_back((Salone *) saloneAddr);
        }
        addCinema(cinema);
    }
}

void menu() {
    ConsoleTable table(1, 1, samilton::Alignment::centre);
    ConsoleTable::TableChars chars;
    chars.topDownSimple = '_';
    chars.leftSeparation = '|';
    chars.centreSeparation = '|';
    chars.rightSeparation = '|';
    chars.downSeparation = '|';
    chars.topSeparation = '|';
    chars.topRight = '|';
    chars.downLeft = '|';
    chars.downRight = '|';
    chars.topLeft = '|';
    chars.leftRightSimple = '|';
    table.setTableChars(chars);

    system("clear");
	table[0][0] = "1";
	table[0][1] = "add actor";
	table[1][0] = "2";
	table[1][1] = "add film";
	table[2][0] = "3";
	table[2][1] = "add sans";
	table[3][0] = "4";
	table[3][1] = "add salone";
	table[4][0] = "5";
	table[4][1] = "add cinema";
	table[5][0] = "6";
	table[5][1] = "add by ticket";
    table[5][0] = "0";
	table[5][1] = "exit";
    cout << table << endl;
    cout << "Enter your option: " << endl;
    int op;
    cin >> op;
    switch (op)
    {
        case 0:
            exit(1); 
        case 1:
            actorMenu();
            break;
        case 2:
            filmMenu();
            break;
        case 3:
            sansMenu();
            break;

    default:
        system("clear");
        cout << "Your option isn't valid. select another one!";
        cout.flush();
        delay();
        menu();
    }

}

void actorMenu() {
    string name;
    cout << "\nEnter a name for actor:\n";
    cout.flush();
    cin >> name;
    Actor actor = {name};
    bool flg = addActor(actor);
    if(flg) {
        cout << "Actor created successfully :)";
    } else {
        cout << "Actor is already created :(";
    }
    cout.flush();
    delay();
    menu();
}

void filmMenu() {
    Film film;
    cout << "\nEnter film name: ";
    cout.flush();
    cin >> film.name;
    cout << "Enter film length as an integer: ";
    cout.flush();
    cin >> film.length;
    cout << "Enter content of film: ";
    cout.flush();
    cin >> film.content;
    cout << "Enter summary of a film in single line:\n";
    cout.flush();
    cin >> film.summary;

    // actors of film

    ConsoleTable table(1, 1, samilton::Alignment::centre);
    ConsoleTable::TableChars chars;
    chars.topDownSimple = '_';
    chars.leftSeparation = '|';
    chars.centreSeparation = '|';
    chars.rightSeparation = '|';
    chars.downSeparation = '|';
    chars.topSeparation = '|';
    chars.topRight = '|';
    chars.downLeft = '|';
    chars.downRight = '|';
    chars.topLeft = '|';
    chars.leftRightSimple = '|';
    table.setTableChars(chars);
	for(int i = 0; i < actors.size(); ++i) {
        table[i][0] = to_string(i + 1);
        table[i][1] = actors[i].name;
    }
    cout << table << endl;
    cout << "Enter a sequense of actors splited by ,. for example(1,3,5,2):\n" << endl;
    cout.flush();
    string pat, act = "";
    cin >> pat;
    for(char c : pat) {
        if(c == ',') {
            film.actors.push_back(&actors[stoi(act) - 1]);
            act = "";
        }
        else act += c;
    }
    film.actors.push_back(&actors[stoi(act) - 1]);
    bool flg = addFilm(film);
    if(flg) {
        cout << "Film created successfully :)";
    } else {
        cout << "Film is already created :(";
    }
    cout.flush();
    delay();
    menu();
}

void sansMenu() {
    Sans sans;
    cout << "\nEnter sans start time as this form month-day-hour-minute for example(4-13-18-30): ";
    cout.flush();
    string startTime;
    cin >> startTime;
    string tim = "";
    tm stTime;
    int cnt = 0;
    for(char c : startTime) {
        if(c == '-') {
            if(cnt == 0) {
                stTime.tm_mon = stoi(tim);
            } else if(cnt == 1) {
                stTime.tm_mday = stoi(tim);
            }   else if(cnt == 2) {
                stTime.tm_hour = stoi(tim);
            }
            cnt++;
            tim = "";
        } else {
            tim += c;
        }
    }
    stTime.tm_min = stoi(tim);
    sans.start = mktime(&stTime);

    cout << "\nEnter sans end time as this form month-day-hour-minute for example(4-13-18-30): ";
    cout.flush();
    string endTime;
    cin >> endTime;
    tim = "";
    tm edTime;
    cnt = 0;
    for(char c : endTime) {
        if(c == '-') {
            if(cnt == 0) {
                edTime.tm_mon = stoi(tim);
            } else if(cnt == 1) {
                edTime.tm_mday = stoi(tim);
            }   else if(cnt == 2) {
                edTime.tm_hour = stoi(tim);
            }
            cnt++;
            tim = "";
        } else {
            tim += c;
        }
    }
    edTime.tm_min = stoi(tim);
    sans.end = mktime(&edTime);

    // film of sans

    ConsoleTable table(1, 1, samilton::Alignment::centre);
    ConsoleTable::TableChars chars;
    chars.topDownSimple = '_';
    chars.leftSeparation = '|';
    chars.centreSeparation = '|';
    chars.rightSeparation = '|';
    chars.downSeparation = '|';
    chars.topSeparation = '|';
    chars.topRight = '|';
    chars.downLeft = '|';
    chars.downRight = '|';
    chars.topLeft = '|';
    chars.leftRightSimple = '|';
    table.setTableChars(chars);
	for(int i = 0; i < films.size(); ++i) {
        table[i][0] = to_string(i + 1);
        table[i][1] = films[i].name;
    }
    cout << table << endl;
    cout << "Enter film number: ";
    cout.flush();
    int op;
    cin >> op;
    sans.film = &films[op - 1];

    cout << "Enter salone capacity: ";
    cout.flush();
    cin >> op;
    sans.emptyCap = op;

    ConsoleTable tableSalone(1, 1, samilton::Alignment::centre);
    tableSalone.setTableChars(chars);
    for(int i = 0; i < salones.size(); ++i) {
        tableSalone[i][0] = to_string(i + 1);
        tableSalone[i][1] = salones[i].code;
    }
    cout << tableSalone << endl;
    cout << "Enter salone number: ";
    cout.flush();
    cin >> op;
    

    bool flg = addSans(sans, &salones[op - 1]);
    if(flg) {
        cout << "sans added successfully :)";
    } else {
        cout << "sans is making conflict :(";
    }
    cout.flush();
    delay();
    menu();
}


int main() {
    srand(time(0));    
    menu();
}