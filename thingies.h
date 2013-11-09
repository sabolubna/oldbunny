#ifndef THINGIES_H_INCLUDED
#define THINGIES_H_INCLUDED

#include "main.h"

//**************BOMBY*******************
void newbomb(Game *game, Room *room, int posx, int posy, int enemy, int exploded, double time);
void drawbomb(Game *game, int i);
void bomb_sort(Game *game);
void bomb_sort(Game *game);
void deletebomb(Game *game, Room *room, int i);
void nobombs(Room *room);
//**************ITEMY*******************
void newitem(Game *game, Room *room, int price, int number, int posx, int posy);
void drawitem(Game *game, int i);
void deleteitem(Room *room, int i);
void noitems(Room *room);
void itemeffect(Game *game, int num, int i);
void spaceeffect(Game *game, int num);
//***************PICKUPY*****************
void newpickup(Game *game, Room *room, int type, int posx, int posy, int price);
void drawpickup(Game *game, int i);
void pickup_sort(Game *game);
void deletepickup(Room *room, int i);
void nopickups(Room *room);

#endif // THINGIES_H_INCLUDED
