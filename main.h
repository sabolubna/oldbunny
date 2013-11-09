#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define LEVELTYPES 2
#define ITEMS 5
#define BASICITEMS 3
#define PORTALPOSX 370
#define PORTALPOSY 150

typedef struct Shot
{
    ALLEGRO_BITMAP *picture;
    int wys;
    int szer;
    double posx;
    double posy;
    double posz;
    double speedx;
    double speedy;
    double range;
    int killsbunny; // 0 - rani wroga, 1 - rani krolisia
    int type; //0 - shot, 1 - laser, 2 - lawa
    double dmg;
    double endtime; //laser, lawa
} Shot;

typedef struct Bunny
{
    ALLEGRO_BITMAP *picture;
    ALLEGRO_BITMAP *shotpic;
    double posx;
    double posy;
    double posz;
    int szer;
    int wys;
    int grub;
    double kicspeed; // szybkosc kicania
    int statex; // wysokosc kicniecia
    int statey; // skierowanie buzki
    int hp; // liczba polowek serduszek
    int hearts; // max 6

    double shotfreq; // 0.1 .. 0.6
    int dmg; // 10 .. 60
    double speed; //0.009 ..
    int range; // 300 .. 700

    double lastshot;
    double shotspeed;
    double step;
    int shottype;
    double immortal; // do kiedy nie moze byc raniony
    int battery; //bonus do ladowania spacji
    int boots; // bezbolesny bonus room
} Bunny;

typedef struct Enemy
{
    ALLEGRO_BITMAP *picture;
    ALLEGRO_BITMAP *shotpic;
    int picposx;
    int picposy;
    double posx;
    double posy;
    double posz;
    int wys;
    int szer;
    int grub;
    double hp;
    double lasthurt;
    int number;
    int statepos; //obrazek
    int stateaction; // czynnosc
    int statez;
    int direct;
    double nextdir;
    double nextchange;
    double step;
    double lastshot;
    double shotfreq;
    int shottype;
    int movetype;
    double shotspeed;
    int range;
    int dmg;
    double animetime;
    double lastanime;

    int deathtype; //0 - normal, 1 - fistula, 2 - shots, 3 - bomb,
} Enemy;

typedef struct Pickup
{
    int posx;
    int posy;
    int posz;
    int type; //0 - coin; 1-carrot; 2-bomb; 3- key;
    int price;
    struct Pickup *next;
    struct Pickup *prev;
} Pickup;

typedef struct Item
{
    ALLEGRO_BITMAP *picture;
    int price;
    int number;
    int posx;
    int posy;
    int taken;
    int frozen;
    int spaceleft;
    struct Item *prev;
    struct Item *next;
} Item;

typedef struct Bomb
{
    int posx;
    int posy;
    int killsenemy;
    int state;
    int color;
    int exploded;
    double time;
} Bomb;

typedef struct Room
{
    int type; // typ pokoju - 0 normal, 1 item(klucz), 2 bonus(kolce), 3 boss, 4 sklep(klucz), 5 secret(bomba)
    Pickup pickups[10];
    int pickupcnt;
    Item items[3];
    int itemcnt;
    Bomb bombs[8];
    int bombcnt;
    //Pickup *firstpickup;
    struct Room *left;
    struct Room *right;
    struct Room *up;
    struct Room *down;
    int locked;
    int doorsopen[4]; //0 - blocked(brama), 1 - open
    Enemy enemies[16];
    int enemycnt;
} Room;

typedef struct Level
{
    ALLEGRO_BITMAP *picture;
    ALLEGRO_BITMAP *logo;
    char inffile[24];
    int enemyfirst;
    int enemylast;
    int bossfirst;
    int bosslast;
    int itemfirst;
    int itemlast;
} Level;

typedef struct Menu
{
    ALLEGRO_BITMAP *picture;
    int choice; //0 - newgame, 1 - options, 2-stats 3-about 4-quit
    int screen; //0 menu glowne 1- options 2- stats 3- about
} Menu;

typedef struct Game
{
    ALLEGRO_BITMAP *pickups, *hearts, *cyferki, *doors, *portal, *hellportal, *itempics, *background, *bombpic,
    *keypic, *carrotpic, *coinpic, *tile, *dym, *enemies, *shotpics, *pause;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_KEYBOARD_STATE keyboard;
    int state; // 0 - menu, 1 - gra, 2- pauza
    int portalstate;
    int szer;
    int wys;
    Bunny bunny;
    Level level;
    int levelnum;
    int roomcnt;
    Room *firstroom;
    Room *room;
    Menu menu;
    int sounds;
    int music;
    Shot shots[100];
    int objects[6][2];
    int objectcnt;
    int shotcnt;
    int coins;
    int bombs;
    int keys;
    int maxlevelnum;
    int spaceitem;
    int spaceleft;
    int usedlevels[LEVELTYPES];
    int useditems[ITEMS];
    double roomeffect[3]; // tears, dmg, shottype,
} Game;

#endif // MAIN_H_INCLUDED
