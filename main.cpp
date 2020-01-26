#include<iostream>
#include<string>
#include<vector>
#include<time.h>
#include<chrono>
#include"CppConsoleTable.hpp"

using namespace std;
using ConsoleTable = samilton::ConsoleTable;

#define delay() for(int i = 0; i < (int) 1e8; ++i)
#define deb() cout << "error///////////////\n";cout.flush()

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
void saloneMenu();
void cinemaMenu();
void buyTicketMenu();

// structs

struct Cinema {
    int id;
    string name;
    vector<Salone*> salones;
};

struct Salone {
    int id;
    int code;
    vector<Sans*> scheduls;
    Cinema *cinema;
};

struct Sans {
    int id;
    time_t start;
    time_t end;
    Film *film;
    int emptyCap;
    Salone *salone;
};

struct Film {
    int id;
    string name;
    int length;
    string content;
    string summary;
    vector<Actor*> actors;
};

struct Actor {
    int id;
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
    return rand() % 10000234 + 37;
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
        sanses.push_back(sans);
        salone->scheduls.push_back(&sanses.back());
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
        salones.push_back(salone);
        cinema->salones.push_back(&salones.back());
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

bool buyTicket(Sans &sans) {
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
    //freopen("data.dat", "r", stdin);
    FILE *fin = fopen("data.dat", "r");
    int cntAct;
    fscanf(fin, "%d", &cntAct);
    for(int i = 0; i < cntAct; ++i) {
        // cin >> name;
        char name[200];
        fscanf(fin, "%s", name);
        Actor actor;
        actor.name = name;
        actor.id = actors.size();
        //cerr << "addActor: " << name << " added\n";
        addActor(actor);
    }
    int cntFilm;
    // cin >> cntFilm;
    fscanf(fin, "%d", &cntFilm);
    for(int i = 0; i < cntFilm; ++i) {
        char name[200];
        int length;
        char content[500];
        char summary[1000];
        int cntActorsOfFilm;
        // cin >> name >> length >> content >> summary >> cntActorsOfFilm;
        fscanf(fin, "%s %d %s %s %d", name, &length, content, summary, &cntActorsOfFilm);
        Film film;
        film.name = name;
        film.length = length;
        film.content = content;
        film.summary = summary;
        for(int j = 0; j < cntActorsOfFilm; ++j) {
            int address;
            fscanf(fin ,"%d", &address);
            film.actors.push_back(&actors[address]);
        }
        film.id = films.size();
        addFilm(film);
    }
    int cntSans;
    // cin >> cntSans;
    vector<int> salonAddrVec;
    fscanf(fin, "%d", &cntSans);
    for(int i = 0; i < cntSans; ++i) {
        tm start;
        tm end;
        int month, day, hour, minute;
        // cin >> month >> day >> hour >> minute;
        fscanf(fin, "%d %d %d %d", &month, &day, &hour, &minute);
        start.tm_mon = month;
        start.tm_mday = day;
        start.tm_hour = hour;
        start.tm_min = minute;

        // cin >> month >> day >> hour >> minute;
        fscanf(fin, "%d %d %d %d", &month, &day, &hour, &minute);
        end.tm_mon = month;
        end.tm_mday = day;
        end.tm_hour = hour;
        end.tm_min = minute;

        int addres;
        fscanf(fin, "%d", &addres);

        int empCap;
        // cin >> empCap;
        fscanf(fin, "%d", &empCap);

        int salonAddr;
        fscanf(fin, "%d", &salonAddr);

        Sans sans;
        sans.id = sanses.size();
        sans.start = mktime(&start);
        sans.end = mktime(&end);
        sans.film = &films[addres];
        sans.emptyCap = empCap;
        // sans.salone = &salones[salonAddr];
        //addSans(sans, &salones[salonAddr]);

        sanses.push_back(sans);
        salonAddrVec.push_back(salonAddr);
    }
    vector<int> cinemaAddrVec;
    int cntSalon;
    // cin >> cntSalon;
    fscanf(fin, "%d", &cntSalon);
    for(int i = 0; i < cntSalon; ++i) {
        Salone salone;
        int code;
        // cin >> code;
        fscanf(fin, "%d", &code);
        salone.code = code;
        int cntScheduls;
        // cin >> cntScheduls;
        fscanf(fin, "%d", &cntScheduls);
        for(int j = 0; j < cntScheduls; ++j) {
            int address;
            fscanf(fin, "%d", &address);
            salone.scheduls.push_back(&sanses[address]);
        }

        int cinemaAddr;
        fscanf(fin, "%d", &cinemaAddr);
        // salone.cinema = &cinemas[cinemaAddr];

        //addSalone(salone, &cinemas[cinemaAddr]);
        salones.push_back(salone);
        cinemaAddrVec.push_back(cinemaAddr);
    }
    int cntCinema;
    // cin >> cntCinema;
    fscanf(fin, "%d", &cntCinema);
    for(int i = 0; i < cntCinema; ++i) {
        char name[200];
        // cin >> name;
        fscanf(fin, "%s", name);
        Cinema cinema;
        cinema.name = name;
        int cntSalone;
        // cin >> cntSalone;
        fscanf(fin, "%d", &cntSalon);
        for(int j = 0; j < cntSalone; ++j) {
            int saloneAddr;
            // cin >> saloneAddr;
            fscanf(fin, "%d", &saloneAddr);
            cinema.salones.push_back(&salones[saloneAddr]);
        }
        addCinema(cinema);
    }


    //cout << sanses.size(); 
    for(int i = 0; i < sanses.size(); ++i) {
        //sanses[i].salone->scheduls.push_back(&sanses[i]);
        // cout << sanses[i].salone->scheduls.size()
        // cout.flush();
        sanses[i].salone = &salones[salonAddrVec[i]];
    }
    for(int i = 0; i < salones.size(); ++i) {
        salones[i].cinema = &cinemas[cinemaAddrVec[i]];
    }
    for(auto &s : sanses) {
        s.salone->scheduls.push_back(&s);
    }
    for(auto &s : salones) {
        s.cinema->salones.push_back(&s);
    }
    // deb();

    // for(auto &s : sanses) {
    //     s.salone->scheduls.push_back(&s);
    // }
    // deb();
    // for(auto &s : salones) {
    //     s.cinema->salones.push_back(&s);
    // }


    fclose(fin);
}

void writeData() {
    freopen("data.dat", "w", stdout);
    cout << actors.size() << endl;
    for(int i = 0; i < actors.size(); ++i) {
        cout << actors[i].name << " ";
    }
    cout << endl << endl;
    cout << films.size() << endl;
    for(int i = 0; i < films.size(); ++i) {
        cout << films[i].name << " " << films[i].length << " " << films[i].content << " " << films[i].summary << endl;
        cout << films[i].actors.size() << endl;
        for(int j = 0; j < films[i].actors.size(); ++j) {
            cout << films[i].actors[j]->id << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << sanses.size() << endl;
    for(int i = 0; i < sanses.size(); ++i) {
        tm *start = localtime(&sanses[i].start);
        cout << start->tm_mon << " " << start->tm_mday << " " << start->tm_hour << " " << start->tm_min << endl;
        tm *end = localtime(&sanses[i].end);
        cout << end->tm_mon << " " << end->tm_mday << " " << end->tm_hour << " " << end->tm_min << endl;
        cout << sanses[i].film->id << endl;
        cout << sanses[i].emptyCap << endl;
        cout << sanses[i].salone->id << endl;
    }
    cout << endl;
    cout << salones.size() << endl;
    for(int i = 0; i < salones.size(); ++i) {
        cout << salones[i].code << endl;
        cout << salones[i].scheduls.size() << endl;
        for(int j = 0; j < salones[i].scheduls.size(); ++j) {
            cout << salones[i].scheduls[j]->id << " ";
        }
        cout << endl << salones[i].cinema->id << endl;
    }
    cout << endl;
    cout << cinemas.size() << endl;
    for(int i = 0; i < cinemas.size(); ++i) {
        cout << cinemas[i].name << endl;
        cout << cinemas[i].salones.size() << endl;
        for(int j = 0; j < cinemas[i].salones.size(); ++j) {
            cout << cinemas[i].salones[i]->id << " ";
        }
        cout << endl;
    }




}

void menu() {
    ConsoleTable table(1, 1, samilton::Alignment::centre);
    ConsoleTable::TableChars chars;
    chars.topDownSimple = '=';
    chars.leftSeparation = '=';
    chars.centreSeparation = '=';
    chars.rightSeparation = '=';
    chars.downSeparation = '=';
    chars.topSeparation = '=';
    chars.topRight = '=';
    chars.downLeft = '=';
    chars.downRight = '=';
    chars.topLeft = '=';
    chars.leftRightSimple = '=';
    table.setTableChars(chars);

    //system("clear");
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
	table[5][1] = "buy ticket";
    table[6][0] = "0";
	table[6][1] = "exit";
    cout << table << endl;
    cout << "Enter your option: " << endl;
    int op;
    cin >> op;
    switch (op)
    {
        case 0:
            cout << "writing changes :)\n";
            writeData();
            delay();
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
        case 4:
            saloneMenu();
            break;
        case 5:
            cinemaMenu();
            break;
        case 6:
            buyTicketMenu();
            break;
    default:
        //system("clear");
        cout << "Your option isn't valid. select another one!";
        cout.flush();
        delay();
    }
}

void actorMenu() {
    string name;
    cout << "\nEnter a name for actor:\n";
    cout.flush();
    cin >> name;
    Actor actor = {actors.size(), name};
    bool flg = addActor(actor);
    if(flg) {
        cout << "Actor created successfully :)";
    } else {
        cout << "Actor is already created :(";
    }
    cout.flush();
    delay();
}

void filmMenu() {
    Film film;
    film.id = films.size();
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
    chars.topDownSimple = '=';
    chars.leftSeparation = '=';
    chars.centreSeparation = '=';
    chars.rightSeparation = '=';
    chars.downSeparation = '=';
    chars.topSeparation = '=';
    chars.topRight = '=';
    chars.downLeft = '=';
    chars.downRight = '=';
    chars.topLeft = '=';
    chars.leftRightSimple = '=';
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
    chars.topDownSimple = '=';
    chars.leftSeparation = '=';
    chars.centreSeparation = '=';
    chars.rightSeparation = '=';
    chars.downSeparation = '=';
    chars.topSeparation = '=';
    chars.topRight = '=';
    chars.downLeft = '=';
    chars.downRight = '=';
    chars.topLeft = '=';
    chars.leftRightSimple = '=';
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
    sans.salone = &salones[op - 1];
    sans.id = sanses.size();
    bool flg = addSans(sans, &salones[op - 1]);
    if(flg) {
        cout << "sans added successfully :)";
    } else {
        cout << "sans is making conflict :(";
    }
    cout.flush();
    delay();
}

void saloneMenu() {
    Salone salone;
    salone.id = salones.size();
    salone.code = generateRandomNum();

    ConsoleTable table(1, 1, samilton::Alignment::centre);
    ConsoleTable::TableChars chars;
    chars.topDownSimple = '=';
    chars.leftSeparation = '=';
    chars.centreSeparation = '=';
    chars.rightSeparation = '=';
    chars.downSeparation = '=';
    chars.topSeparation = '=';
    chars.topRight = '=';
    chars.downLeft = '=';
    chars.downRight = '=';
    chars.topLeft = '=';
    chars.leftRightSimple = '=';
    table.setTableChars(chars);
	for(int i = 0; i < cinemas.size(); ++i) {
        table[i][0] = to_string(i + 1);
        table[i][1] = cinemas[i].name;
    }
    cout << table << endl;
    cout << "Enter cinema number:\n" << endl;
    cout.flush();
    int op;
    cin >> op;
    salone.cinema = &cinemas[op - 1];

    bool flg = addSalone(salone, &cinemas[op - 1]);
    if(flg) {
        cout << "Salone created successfully :)";
    } else {
        cout << "Salone can't be created :(";
    }
    cout.flush();
    delay();
}

void cinemaMenu() {
    Cinema cinema;
    cinema.id = cinemas.size();
    cout << "Enter cinema name: ";
    cout.flush();
    cin >> cinema.name;
    bool flg = addCinema(cinema);
    if(flg) {
        cout << "Cinema created successfully :)";
    } else {
        cout << "Cinema can't be created :(";
    }
    cout.flush();
    delay();
}

void buyTicketMenu() {
    ConsoleTable table(1, 1, samilton::Alignment::centre);
    ConsoleTable::TableChars chars;
    chars.topDownSimple = '=';
    chars.leftSeparation = '=';
    chars.centreSeparation = '=';
    chars.rightSeparation = '=';
    chars.downSeparation = '=';
    chars.topSeparation = '=';
    chars.topRight = '=';
    chars.downLeft = '=';
    chars.downRight = '=';
    chars.topLeft = '=';
    chars.leftRightSimple = '=';
    table.setTableChars(chars);

    table[0][0] = "ٔNumber";
    table[0][1] = "Film";
    table[0][2] = "Start";
    table[0][3] = "End";
    table[0][4] = "Content";
    table[0][5] = "Summary";
    table[0][6] = "Capacity";

    int cnt = 1;
    time_t curr_time;
	curr_time = time(0);
	tm *tm_local = localtime(&curr_time);
    for(int i = 0; i < sanses.size(); ++i) {
        if(sanses[i].start >= curr_time && sanses[i].end > curr_time && sanses[i].emptyCap > 0) {
            table[cnt][0] = cnt;
            table[cnt][1] = sanses[i].film->name;
            tm *sTime = localtime(&sanses[i].start);
            table[cnt][2] = to_string(sTime->tm_mon + 1) + "-" + to_string(sTime->tm_mday) + "-" + to_string(sTime->tm_hour) + "-" + to_string(sTime->tm_min);

            tm *eTime = localtime(&sanses[i].end);
            table[cnt][3] = to_string(eTime->tm_mon + 1) + "-" + to_string(eTime->tm_mday) + "-" + to_string(eTime->tm_hour) + "-" + to_string(eTime->tm_min);

            table[cnt][4] = sanses[i].film->content;
            table[cnt][5] = sanses[i].film->summary;
            table[cnt][6] = sanses[i].emptyCap;
            ++cnt;
        }
    }
    cout << table << endl;
    cout << "Enter your sans number or enter 0 to exit: ";
    cout.flush();
    int op;
    cin >> op;
    if(op == 0) menu();
    cnt = 1;
    for(int i = 0; i < sanses.size(); ++i) {
        if(sanses[i].start >= curr_time && sanses[i].end > curr_time && sanses[i].emptyCap > 0) {
            if(cnt == op) {
                bool flg = buyTicket(sanses[i]);
                if(flg) {
                    cout << "ticket bought successfully :)";
                } else {
                    cout << "can't buy ticket :(";
                }
                cout.flush();
                break;
            }
        }
    }
    delay();
}


int main() {
    srand(time(0));
    readData();
    while(1) {
        menu();
    }
}
