#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "main.h"
#include "thingies.h"

//***************STRZALY*******************
void newshot(Game *game, ALLEGRO_BITMAP *picture, int posx, int posy, int posz, int wys, int szer, double speedx, double speedy,
             int range, int killsbunny, int type, int dmg)
{
    Shot shot;
    shot.picture = picture;
    shot.posx = posx;
    shot.posy = posy - 1;
    shot.posz = posz;
    shot.wys = wys;
    shot.szer = szer;
    shot.speedx = speedx;
    shot.speedy = speedy;
    shot.range = range;
    if (type == 1)
    {
        shot.endtime = al_get_time() + game->bunny.shotfreq*0.4;
    }
    shot.killsbunny = killsbunny;
    shot.type = type;
    shot.dmg = dmg;
    game->shots[game->shotcnt] = shot;
    game->shotcnt++;
}

void deleteshot(Game *game, int i)
{
    int j;
    for (j=i; j+1<game->shotcnt; j++)
    {
        game->shots[j] = game->shots[j+1];
    }
    game->shotcnt--;
}

void drawshot(Game *game, int i)
{
    Shot shot;
    shot = game->shots[i];
        if (shot.type==0)
        {
            al_draw_filled_ellipse(shot.posx+shot.szer/2, shot.posy, shot.szer/2, shot.wys/2, al_map_rgba(0,0,0,50));
            if (shot.range<40)
                al_draw_tinted_bitmap(shot.picture,al_map_rgba(255,255,255,200),shot.posx,
                                      shot.posy-shot.posz-shot.wys, 0);
            else
                al_draw_bitmap(shot.picture,shot.posx, shot.posy-shot.posz-shot.wys, 0);
        }
        else if (shot.type==1)
        {
            if (shot.speedx==0)
            {
                if (shot.speedy>0)
                {
                    al_draw_line(shot.posx, shot.posy-shot.posz, shot.posx, game->wys, al_map_rgb(255,0,0),3);
                }
                else
                {
                    al_draw_line(shot.posx, shot.posy-shot.posz, shot.posx, 0, al_map_rgb(255,0,0),3);
                }
            }
            else
            {
                if (shot.speedx>0)
                {
                    al_draw_line(shot.posx, shot.posy, game->szer, shot.posy, al_map_rgba(0,0,0,50),3);
                    al_draw_line(shot.posx, shot.posy-shot.posz, game->szer, shot.posy-shot.posz, al_map_rgb(255,0,0),3);
                }
                else
                {
                    al_draw_line(shot.posx, shot.posy, 0, shot.posy, al_map_rgba(0,0,0,50),3);
                    al_draw_line(shot.posx, shot.posy-shot.posz, 0, shot.posy-shot.posz, al_map_rgb(255,0,0),3);
                }
            }
        }
}

void shotsmove(Game *game)
{
    Shot shot;
    int i, j;
    i = 0;
    while(i < game->shotcnt)
    {
        shot = game->shots[i];
        if (shot.type == 0)
        {
            if (shot.range < 0 || shot.posx <= 50 || shot.posx+shot.szer >= 750 || shot.posy-shot.wys <= 100
                || shot.posy >= 550)
            {
                deleteshot(game, i);
                i--;
            }
            else
            {
                shot.posx += shot.speedx;
                shot.posy += shot.speedy;
                shot.posz -= shot.posz/shot.range;
                shot.range = shot.range - sqrt(pow(shot.speedx,2)+pow(shot.speedy,2));
                game->shots[i] = shot;
                Enemy enemy;
                j = 0;
                if (shot.killsbunny == 0)
                while(j < game->room->enemycnt)
                {
                    enemy = game->room->enemies[j];
                    if (shot.posy <= enemy.posy + 10 && shot.posy >= enemy.posy-enemy.grub/2+5
                        && shot.posx <= enemy.posx+enemy.szer && shot.posx+shot.szer >=enemy.posx)
                    {
                        game->room->enemies[j].hp -= shot.dmg;
                        if (game->room->enemies[j].hp <= 0)
                        {
                            deleteenemy(game, j);
                            j = game->room->enemycnt;
                        }
                        else
                            game->room->enemies[j].lasthurt = al_get_time();
                        deleteshot(game, i);
                        i--;
                    }
                    j++;
                }
                else
                {
                    if (shot.posy <= game->bunny.posy + 10 && shot.posy >= game->bunny.posy-game->bunny.grub/2+5
                        && shot.posx <= game->bunny.posx+game->bunny.szer && shot.posx+shot.szer >=game->bunny.posx)
                    {
                        if (game->bunny.immortal < al_get_time())
                        {
                            game->bunny.hp -= shot.dmg;
                            game->bunny.immortal = al_get_time() + 1;
                        }
                        deleteshot(game,i);
                        i--;
                    }
                }
            }
        }
        else if (shot.type == 1)
        {
            if (shot.endtime < al_get_time())
            {
                deleteshot(game, i);
                i--;
            }
            else
            {
                shot.posx = game->bunny.posx+25;
                shot.posy = game->bunny.posy;
                game->shots[i] = shot;
                Enemy enemy;
                j = 0;
                while(j < game->room->enemycnt)
                {
                    enemy = game->room->enemies[j];
                    if (( shot.speedy == 0 && shot.speedx > 0 && shot.posx <= enemy.posx && shot.posy <= enemy.posy + 10
                         && shot.posy >= enemy.posy-enemy.grub+10)
                        ||
                        (shot.speedy == 0 && shot.speedx < 0 && shot.posx >= enemy.posx && shot.posy <= enemy.posy + 10
                         && shot.posy >= enemy.posy-enemy.grub+10)
                        ||
                        (shot.speedx == 0 && shot.speedy > 0 && shot.posy <= enemy.posy && shot.posx <= enemy.posx+enemy.szer
                         && shot.posx+shot.szer >=enemy.posx)
                        ||
                        (shot.speedx == 0 && shot.speedy < 0 && shot.posy >= enemy.posy && shot.posx <= enemy.posx+enemy.szer
                         && shot.posx+shot.szer >=enemy.posx))
                    {
                        game->room->enemies[j].hp -= shot.dmg/10;
                        if (game->room->enemies[j].hp <= 0)
                        {
                            deleteenemy(game, j);
                            j = game->room->enemycnt;
                        }
                        else
                            game->room->enemies[j].lasthurt = al_get_time();
                    }
                    j++;
                }
            }
        }
        i++;
    }
}

void shot_sort(Game *game)
{
    int i, j;
    Shot tmp;
    for (j=0; j<game->shotcnt; j++)
    {
        i = j;
        while(game->shots[i].posy < game->shots[i-1].posy && i > 0)
        {
            tmp = game->shots[i];
            game->shots[i] = game->shots[i-1];
            game->shots[i-1] = tmp;
            i--;
        }
    }
}

void noshots(Game *game)
{
    game->shotcnt = 0;
}

//*************PRZECIWNICY*************
void newenemy(Game *game, Room *room, int number, double posx, double posy, double posz)
{
    Enemy enemy;
    enemy.number = number;
    enemy.posx = posx;
    enemy.posy = posy;
    enemy.posz = posz;
    switch (number)
    {
        case 0: // bulbasaur
        {
            enemy.picture = al_create_sub_bitmap(game->enemies,0,0,228,50);
            enemy.picposx = 0;
            enemy.picposy = 0;
            enemy.wys = 50;
            enemy.szer = 57;
            enemy.grub = 31;
            enemy.hp = 60;
            enemy.step = 1;
            enemy.movetype = 0;
            enemy.shotfreq = 2;
            enemy.shottype = 0;
            enemy.range = 400;
            enemy.shotspeed = 3.5;
            enemy.dmg = 1;
            enemy.shotpic = al_create_sub_bitmap(game->shotpics, 0,0,20,20);
            enemy.animetime = 0.2;
            enemy.statepos = 0;
            break;
        }
        case 1: //charmander
        {
            enemy.picture = al_create_sub_bitmap(game->enemies,0,50,432,73);
            enemy.picposx = 0;
            enemy.picposy = 0;
            enemy.wys = 73;
            enemy.szer = 54;
            enemy.grub = 35;
            enemy.hp = 70;
            enemy.step = 1;
            enemy.movetype = 0;
            enemy.shotfreq = 3;
            enemy.shottype = 1;
            enemy.range = 400;
            enemy.shotspeed = 3.5;
            enemy.dmg = 1;
            enemy.shotpic = al_create_sub_bitmap(game->shotpics, 20,0,20,20);
            enemy.animetime = 0.05;
            enemy.statepos = 0;
            break;
        }
        case 2: // farfetch'd
        {
            enemy.picture = al_create_sub_bitmap(game->enemies,0,125,480,61);
            enemy.picposx = 0;
            enemy.picposy = 0;
            enemy.wys = 61;
            enemy.szer = 60;
            enemy.grub = 33;
            enemy.hp = 70;
            enemy.step = 1;
            enemy.movetype = 1;
            enemy.shotfreq = -1;
            enemy.shottype = -1;
            enemy.range = -1;
            enemy.shotspeed = -1;
            enemy.dmg = 1;
            enemy.shotpic = NULL;
            enemy.animetime = 0.1;
            enemy.statepos = 0;
            break;
        }
        case 3: // squirtle
        {
            enemy.picture = al_create_sub_bitmap(game->enemies,0,187,480,75);
            enemy.picposx = 0;
            enemy.picposy = 0;
            enemy.wys = 75;
            enemy.szer = 57;
            enemy.grub = 33;
            enemy.hp = 60;
            enemy.step = 1;
            enemy.movetype = 0;
            enemy.shotfreq = -1;
            enemy.shottype = 2;
            enemy.range = 700;
            enemy.shotspeed = -1;
            enemy.dmg = 1;
            enemy.shotpic = NULL;
            enemy.animetime = 0.2;
            enemy.statepos = 0;
            break;
        }
    }
    enemy.lastanime = al_get_time();
    enemy.lastshot = al_get_time();
    enemy.nextdir = al_get_time() - 2;
    enemy.nextchange = al_get_time();
    enemy.lasthurt = al_get_time() - 2;
    enemy.stateaction = 1;
    room->enemies[room->enemycnt] = enemy;
    room->enemycnt++;
}

void drawenemy(Game *game, int i)
{
    Enemy enemy;
    enemy = game->room->enemies[i];
    al_draw_filled_ellipse(enemy.posx+enemy.szer/2,enemy.posy-enemy.grub/2 + 5,enemy.szer/2, enemy.grub/2,
                            al_map_rgba(0,0,0,50));
    if (enemy.lasthurt > al_get_time() - 0.3)
        al_draw_tinted_bitmap_region(enemy.picture, al_map_rgba(100,100,100,100),enemy.picposx,enemy.picposy,
                                     enemy.szer, enemy.wys, enemy.posx, enemy.posy-enemy.wys, 0);
    else
        al_draw_bitmap_region(enemy.picture,enemy.picposx,enemy.picposy,
                            enemy.szer, enemy.wys, enemy.posx, enemy.posy-enemy.wys, 0);
}

void enemyaction(Game *game, int i)
{
    Enemy enemy;
    enemy = game->room->enemies[i];
    switch (enemy.number)
    {
        case 0: // bulbasaur
        {
            if (enemy.direct == 1 && enemy.statepos > 1)
                enemy.statepos = 0;
            if (enemy.direct == 3 && enemy.statepos < 2)
                enemy.statepos = 2;
            if (enemy.lastanime + enemy.animetime < al_get_time())
            {
                enemy.statepos++;
                if (enemy.statepos == 2)
                    enemy.statepos = 0;
                if (enemy.statepos == 4)
                    enemy.statepos = 2;
                enemy.lastanime = al_get_time();
            }
            enemy.picposx = enemy.statepos*enemy.szer;
            break;
        }
        case 1: // charmander
        {
            if (enemy.direct == 1 && enemy.statepos > 3)
                enemy.statepos = 0;
            if (enemy.direct == 3 && enemy.statepos < 4)
                enemy.statepos = 4;
            if (enemy.lastanime + enemy.animetime < al_get_time())
            {
                enemy.statepos++;
                if (enemy.statepos == 4)
                    enemy.statepos = 0;
                if (enemy.statepos == 8)
                    enemy.statepos = 4;
                enemy.lastanime = al_get_time();
            }
            enemy.picposx = enemy.statepos*enemy.szer;
            break;
        }
        case 2: // farfetch'd
        {
            if (enemy.direct == 1 && enemy.statepos > 3)
                enemy.statepos = 0;
            if (enemy.direct == 3 && enemy.statepos < 4)
                enemy.statepos = 4;
            if (enemy.lastanime + enemy.animetime < al_get_time())
            {
                enemy.statepos++;
                if (enemy.statepos == 4)
                    enemy.statepos = 0;
                if (enemy.statepos == 8)
                    enemy.statepos = 4;
                enemy.lastanime = al_get_time();
            }
            enemy.picposx = enemy.statepos*enemy.szer;
            break;
        }
        case 3: // squirtle
        {

            if (enemy.direct == 1 && enemy.statepos > 3)
                enemy.statepos = 0;
            if (enemy.direct == 3 && enemy.statepos < 4)
                enemy.statepos = 4;
            if (enemy.lastanime + enemy.animetime < al_get_time())
            {
                enemy.statepos++;
                if (enemy.statepos == 4)
                    enemy.statepos = 0;
                if (enemy.statepos == 8)
                    enemy.statepos = 4;
                enemy.lastanime = al_get_time();
            }
            enemy.picposx = enemy.statepos*enemy.szer;
            break;
        }
    }
    game->room->enemies[i] = enemy;
}

void enemymove(Game *game, int i)
{
    Enemy enemy;
    enemy = game->room->enemies[i];
    if (enemy.stateaction == 1)
    switch (game->room->enemies[i].movetype)
    {
        case 0: //losowo zmienia kierunek
            switch (enemy.direct)
            {
                case 0:
                    if (enemy.posy-enemy.grub+10 > 100)
                        enemy.posy -= enemy.step;
                    else
                        enemy.direct = rand()%4;
                    break;
                case 1:
                    if (enemy.posx+enemy.szer < 740)
                        enemy.posx += enemy.step;
                    else
                        enemy.direct = rand()%4;
                    break;
                case 2:
                    if (enemy.posy < 540)
                        enemy.posy += enemy.step;
                    else
                        enemy.direct = rand()%4;
                    break;
                case 3:
                    if (enemy.posx > 50)
                        enemy.posx -= enemy.step;
                    else
                        enemy.direct = rand()%4;
                    break;

            }
            if (enemy.nextdir < al_get_time())
            {
                enemy.direct = rand()%4;
                enemy.nextdir = rand()%3+1 + al_get_time();
            }
            break;
        case 1: // goni krolisia
            if (!(enemy.posx+enemy.szer > game->bunny.posx+3 && enemy.posx < game->bunny.posx+game->bunny.szer-3
                  && enemy.posy-enemy.grub < game->bunny.posy && enemy.posy > game->bunny.posy-game->bunny.grub))
            {
                double ydiff = game->bunny.posy-game->bunny.grub/2 - enemy.posy+enemy.grub/2;
                double xdiff = game->bunny.posx+game->bunny.szer/2 - enemy.posx-enemy.szer/2;
                double dist = sqrt(pow(ydiff,2)+pow(xdiff,2));
                double yspeed = ydiff*enemy.step/dist;
                double xspeed = xdiff*enemy.step/dist;
                enemy.posy += yspeed;
                enemy.posx += xspeed;
                if (xspeed > 0 && enemy.direct != 1)
                    enemy.direct = 1;
                if (xspeed < 0 && enemy.direct != 3)
                    enemy.direct = 3;
            }
            break;
    }
    //ranienie krolisia
    if (enemy.posx+enemy.szer > game->bunny.posx+3 && enemy.posx < game->bunny.posx+game->bunny.szer-3
        && enemy.posy-enemy.grub < game->bunny.posy && enemy.posy > game->bunny.posy-game->bunny.grub
        && game->bunny.immortal < al_get_time())
        {
            game->bunny.hp--;
            game->bunny.immortal = al_get_time() + 1;
        }
    game->room->enemies[i] = enemy;
}

void enemyshot(Game *game, int i)
{
    Enemy enemy;
    enemy = game->room->enemies[i];
    switch (enemy.shottype)
    {
        case 0: //4 prostopadle
        {
            if (enemy.lastshot + enemy.shotfreq < al_get_time())
            {
                newshot(game, enemy.shotpic, enemy.posx+enemy.szer/2, enemy.posy,20,20,20,enemy.shotspeed,
                        0,enemy.range,1,0,enemy.dmg);
                newshot(game, enemy.shotpic, enemy.posx+enemy.szer/2, enemy.posy,20,20,20,-enemy.shotspeed,
                        0,enemy.range,1,0,enemy.dmg);
                newshot(game, enemy.shotpic, enemy.posx+enemy.szer/2, enemy.posy,20,20,20,0,enemy.shotspeed,
                        enemy.range,1,0,enemy.dmg);
                newshot(game, enemy.shotpic, enemy.posx+enemy.szer/2, enemy.posy,20,20,20,0,-enemy.shotspeed,
                        enemy.range,1,0,enemy.dmg);
                game->room->enemies[i].lastshot = al_get_time();
            }
            break;
        }
        case 1: // 1 skierowany
        {
            if (enemy.lastshot + enemy.shotfreq < al_get_time())
            {
                if (pow(enemy.posy-enemy.grub/2+5 - (game->bunny.posy-game->bunny.grub/2+5),2)
                + pow(enemy.posx+enemy.szer/2 - (game->bunny.posx+game->bunny.szer/2),2) < pow(enemy.range,2) )
                {
                    double speedx, speedy;
                    speedx = enemy.shotspeed * -(enemy.posx+enemy.szer/2 - (game->bunny.posx+game->bunny.szer/2))
                    / sqrt(pow(enemy.posy-enemy.grub/2+5 - (game->bunny.posy-game->bunny.grub/2+5),2)
                    + pow(enemy.posx+enemy.szer/2 - (game->bunny.posx+game->bunny.szer/2),2));

                    speedy = enemy.shotspeed * -(enemy.posy-enemy.grub/2+5 - (game->bunny.posy-game->bunny.grub/2+5))
                    / sqrt(pow(enemy.posy-enemy.grub/2+5 - (game->bunny.posy-game->bunny.grub/2+5),2)
                    + pow(enemy.posx+enemy.szer/2 - (game->bunny.posx+game->bunny.szer/2),2));

                    newshot(game,enemy.shotpic, enemy.posx+enemy.szer/2, enemy.posy,20,20,20,speedx,
                            speedy,enemy.range,1,0,enemy.dmg);
                }
                game->room->enemies[i].lastshot = al_get_time();
            }
            break;
        }
    }
}

void enemy_sort(Game *game)
{
    int i, j;
    Enemy tmp;
    for (j=1; j<game->room->enemycnt; j++)
    {
        i = j;
        while(game->room->enemies[i].posy < game->room->enemies[i-1].posy && i > 0)
        {
            tmp = game->room->enemies[i];
            game->room->enemies[i] = game->room->enemies[i-1];
            game->room->enemies[i-1] = tmp;
            i--;
        }
    }
}

void deleteenemy(Game *game, int i)
{
    while(i+1<game->room->enemycnt)
    {
        game->room->enemies[i] = game->room->enemies[i+1];
        i++;
    }
    game->room->enemycnt--;
    //otwieranie bram
    if (game->room->enemycnt == 0)
    {
        if (game->room->up!= NULL && game->room->up->type!=5)
        {
            game->room->doorsopen[0] = 1;
            newbomb(game, game->room, 382, 95, 0, 1, 0.3);
        }
        if (game->room->right!= NULL && game->room->right->type!=5)
        {
            game->room->doorsopen[1] = 1;
            newbomb(game, game->room, 755, 342, 0, 1, 0.3);
        }
        if (game->room->down!= NULL && game->room->down->type!=5)
        {
            game->room->doorsopen[2] = 1;
            newbomb(game, game->room, 382, 590, 0, 1, 0.3);
        }
        if (game->room->left!= NULL && game->room->left->type!=5)
        {
            game->room->doorsopen[3] = 1;
            newbomb(game, game->room, 10, 342, 0, 1, 0.3);
            }
        //item lub pickup
        if (game->room->type == 3)
        {
            int random = rand()%(game->level.itemlast-game->level.itemfirst+1) + game->level.itemfirst;
            while(game->useditems[random]==1)
            {
                random = rand()%(game->level.itemlast-game->level.itemfirst+1) + game->level.itemfirst;
            }
            newitem(game, game->room, 0, random, 378, 428);
        }
        else
        {
            newpickup(game, game->room, rand()%3+1, 383, 323, 0);
        }
        //spacja
        game->spaceleft--;
    }
}
