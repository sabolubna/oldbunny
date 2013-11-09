#ifndef SHOTSENEMIES_H_INCLUDED
#define SHOTSENEMIES_H_INCLUDED

#include "main.h"

//*************STRZALY*************
void newshot(Game *game, ALLEGRO_BITMAP *picture, int posx, int posy, int posz, int wys, int szer, double speedx, double speedy,
             int range, int killsbunny, int type, int dmg);
void deleteshot(Game *game, int i);
void drawshot(Game *game, int i);
void shotsmove(Game *game);
void shot_sort(Game *game);
void noshots(Game *game);
//*************PRZECIWNICY*************
void newenemy(Game *game, Room *room, int number, double posx, double posy, double posz);
void drawenemy(Game *game, int i);
void enemyaction(Game *game, int i);
void enemymove(Game *game, int i);
void enemyshot(Game *game, int i);
void enemy_sort(Game *game);
void deleteenemy(Game *game, int i);

#endif // SHOTSENEMIES_H_INCLUDED
