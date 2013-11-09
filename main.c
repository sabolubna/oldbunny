#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#include "main.h"
#include "thingies.h"
#include "shotsenemies.h"

#define LEVELTYPES 2
#define ITEMS 11
#define BASICITEMS 6
#define SHOPITEMS 5
#define PORTALPOSX 370
#define PORTALPOSY 150
//typedef enum {TRUE = 1, FALSE = 0} bool;

//**********RYSOWANIE***************
void drawmenu(Game *game)
{
        al_draw_bitmap(game->menu.picture,0,0,0);
    if (game->menu.screen == 0)
    {
        switch(game->menu.choice)
        {
            case 0: al_draw_scaled_bitmap(game->menu.picture,500,50,204,44,450,40,300,60,0); break;
            case 1: al_draw_scaled_bitmap(game->menu.picture,500,120,204,44,450,110,300,60,0); break;
            case 2: al_draw_scaled_bitmap(game->menu.picture,500,190,204,44,450,180,300,60,0); break;
            case 3: al_draw_scaled_bitmap(game->menu.picture,500,260,204,44,450,250,300,60,0); break;
            case 4: al_draw_scaled_bitmap(game->menu.picture,500,330,204,44,450,320,300,60,0); break;
        }
    }
}

void drawbunny(Game *game)
{
    int a;
    switch (game->bunny.statex)
    {
        case 0: a = 0; break;
        case 1: a = 1; break;
        case 2: a = 2; break;
        case 3: a = 2; break;
        case 4: a = 1; break;
        case 5: a = 0; break;
        case 6: a = 1; break;
        case 7: a = 0; break;
        case 8: a = 1; break;
    }
    if (game->bunny.statex >= 5)
        game->bunny.statex = 0;
    al_draw_filled_ellipse(game->bunny.posx+game->bunny.szer/2, game->bunny.posy-game->bunny.grub/2+5,
                           game->bunny.szer/2, game->bunny.grub/2, al_map_rgba(0,0,0,50));
    if (game->bunny.immortal < al_get_time())
        al_draw_scaled_bitmap(game->bunny.picture,0+a*200,0+game->bunny.statey*392,200,392,game->bunny.posx,game->bunny.posy-90,50,98,0);
    else
        al_draw_tinted_scaled_bitmap(game->bunny.picture,al_map_rgba(200,100,100,100),0+a*200,
                        0+game->bunny.statey*392,200,392,game->bunny.posx,game->bunny.posy-90,50,98,0);
}

void drawhp(Game *game)
{
    int i = 0;
    while(i<2*game->bunny.hearts)
    {
        if (game->bunny.hp-i >= 2)
        {
            al_draw_bitmap_region(game->hearts,39,0,39,35,525+i*45/2,8,0);
        }
        else if (game->bunny.hp-i == 1)
        {
            al_draw_bitmap_region(game->hearts,0,0,39,35,525+i*45/2,8,0);
        }
        else
        {
            al_draw_bitmap_region(game->hearts,78,0,39,35,525+i*45/2,8,0);
        }
        i+=2;
    }
}

void background(Game *game)
{
    al_draw_bitmap(game->level.picture,0,0,0);
    int i;
//**********kafelki pod itemy*********
    i = 0;
    while(i<game->room->itemcnt)
    {
        al_draw_bitmap(game->tile, game->room->items[i].posx-5, game->room->items[i].posy-13,0);
        i++;
    }
    if (game->spaceitem>-1)
    {
        if (game->spaceleft <= 0)
            al_draw_bitmap_region(game->itempics,game->spaceitem%10*45,game->spaceitem/10*45,45,45,5,5,0);
        else
        {
            al_draw_tinted_bitmap_region(game->itempics,al_map_rgba(200,200,200,200),game->spaceitem%10*45,game->spaceitem/10*45,45,45,5,5,0);
            al_draw_bitmap_region(game->cyferki,game->spaceleft*25,0,25,35,60,10,0);
        }
    }
//***********liczniki***********
    if (game->coins>=10)
    {
        al_draw_bitmap_region(game->cyferki,(game->coins/10)*25,0,25,35,160,10,0);
        al_draw_bitmap_region(game->cyferki,(game->coins % 10)*25,0,25,35,185,10,0);
    }
    else al_draw_bitmap_region(game->cyferki,(game->coins % 10)*25,0,25,35,160,10,0);
    if (game->bombs>=10)
    {
        al_draw_bitmap_region(game->cyferki,(game->bombs/10)*25,0,25,35,260,10,0);
        al_draw_bitmap_region(game->cyferki,(game->bombs % 10)*25,0,25,35,285,10,0);
    }
    else al_draw_bitmap_region(game->cyferki,(game->bombs % 10)*25,0,25,35,260,10,0);
    if (game->keys>=10)
    {
        al_draw_bitmap_region(game->cyferki,(game->keys/10)*25,0,25,35,360,10,0);
        al_draw_bitmap_region(game->cyferki,(game->keys % 10)*25,0,25,35,385,10,0);
    }
    else al_draw_bitmap_region(game->cyferki,(game->keys % 10)*25,0,25,35,360,10,0);
//********drzwi*********
    if (game->room->right!=NULL)
    {
        if (game->room->type == 2 || game->room->right->type == 2)
        { //curse/bonus room
            al_draw_bitmap_region(game->doors,0,160,45,80,750,285,1);
        } //secret room
        else if (game->room->type == 5 || game->room->right->type == 5)
        {
            if ( game->room->doorsopen[1] == 1)
                al_draw_bitmap_region(game->doors,0,491,45,80,750,285,1);
        }
        else if(game->room->right->type == 1 || game->room->type == 1)
        { //item room
            if (game->room->right->locked == 1)
                al_draw_bitmap_region(game->doors,0,320,45,80,750,285,1);
            else
                al_draw_bitmap_region(game->doors,0,240,45,80,750,285,1);
        } //shop
        else if (game->room->right->locked == 1)
            al_draw_bitmap_region(game->doors,0,80,45,80,750,285,1);
            //zwykly
        else if (game->room->right->type != 5)
            al_draw_bitmap_region(game->doors,0,0,45,80,750,285,1);
    }
    if (game->room->left!=NULL)
    {
        if (game->room->type == 2 || game->room->left->type == 2)
        { //curse room
            al_draw_bitmap_region(game->doors,0,160,45,80,5,285,0);
        } //secret room
        else if (game->room->type == 5 || game->room->left->type == 5)
        {
            if ( game->room->doorsopen[3] == 1)
                al_draw_bitmap_region(game->doors,0,491,45,80,5,285,0);
        }
        else if(game->room->left->type == 1 || game->room->type == 1)
        { //item room
            if (game->room->left->locked == 1)
                al_draw_bitmap_region(game->doors,0,320,45,80,5,285,0);
            else
                al_draw_bitmap_region(game->doors,0,240,45,80,5,285,0);
        } //shop
        else if(game->room->left->locked == 1)
            al_draw_bitmap_region(game->doors,0,80,45,80,5,285,0);
        else if (game->room->left->type != 5)
            al_draw_bitmap_region(game->doors,0,0,45,80,5,285,0);
    }
    if (game->room->up!=NULL)
    {
        if (game->room->type == 2 || game->room->up->type == 2)
        { //curse room
            al_draw_bitmap_region(game->doors,45,160,80,45,360,55,0);
        } //secret room
        else if (game->room->type == 5 || game->room->up->type == 5)
        {
            if ( game->room->doorsopen[0] == 1)
                al_draw_bitmap_region(game->doors,45,491,80,45,360,55,0);
        }
        else if(game->room->up->type == 1 || game->room->type == 1)
        { //item room
            if (game->room->up->locked == 1)
                al_draw_bitmap_region(game->doors,45,320,80,45,360,55,0);
            else
                al_draw_bitmap_region(game->doors,45,240,80,45,360,55,0);
        } //shop
        else if(game->room->up->locked == 1)
            al_draw_bitmap_region(game->doors,45,80,80,45,360,55,0);
        else if (game->room->up->type != 5)
            al_draw_bitmap_region(game->doors,45,0,80,45,360,55,0);
    }
    if (game->room->down!=NULL)
    {
        if (game->room->type == 2 || game->room->down->type == 2)
        {
            al_draw_bitmap_region(game->doors,45,160,80,45,360,550,2);
        } //secret room
        else if (game->room->type == 5 || game->room->down->type == 5)
        {
            if ( game->room->doorsopen[2] == 1)
                al_draw_bitmap_region(game->doors,45,491,80,45,360,550,2);
        }
        else if(game->room->down->type == 1 || game->room->type == 1)
        { //item room
            if (game->room->down->locked == 1)
                al_draw_bitmap_region(game->doors,45,320,80,45,360,550,2);
            else
                al_draw_bitmap_region(game->doors,45,240,80,45,360,550,2);
        } //shop
        else if(game->room->down->locked == 1)
            al_draw_bitmap_region(game->doors,45,80,80,45,360,550,2);
        else if (game->room->down->type != 5)
            al_draw_bitmap_region(game->doors,45,0,80,45,360,550,2);
    }
//*********bramy*************
    if (game->room->type != 5)
    {
        if (game->room->up!=NULL)
        {
            if (game->room->up->type != 5)
            {
                if (game->room->doorsopen[0] == 0)
                    al_draw_bitmap_region(game->doors,49,400,91,49,353,52,0);
            }
        }
        if (game->room->right!=NULL)
        {
            if (game->room->right->type != 5)
            {
                if (game->room->doorsopen[1] == 0)
                    al_draw_bitmap_region(game->doors,0,400,49,91,750,280,1);
            }
        }
        if (game->room->down!=NULL)
        {
            if (game->room->down->type != 5)
            {
                if (game->room->doorsopen[2] == 0)
                    al_draw_bitmap_region(game->doors,49,400,91,49,353,550,2);
            }
        }
        if (game->room->left!=NULL)
        {
            if (game->room->left->type != 5)
            {
                if (game->room->doorsopen[3] == 0)
                    al_draw_bitmap_region(game->doors,0,400,49,91,1,280,0);
            }
        }

    }
    //al_save_bitmap("ekran.png", al_get_backbuffer(game->display));
    //game->background = al_load_bitmap("ekran.png");
}

void object_sort(Game *game)
{
    int i, j;
    int tmp, tmp2;
    for (j=1; j<6; j++)
    {
        i = j;
        while(game->objects[i][1] < game->objects[i-1][1] && i > 0)
        {
            tmp = game->objects[i][0];
            tmp2 = game->objects[i][1];
            game->objects[i][0] = game->objects[i-1][0];
            game->objects[i][1] = game->objects[i-1][1];
            game->objects[i-1][0] = tmp;
            game->objects[i-1][1] = tmp2;
            i--;
        }
    }
}

void draw(Game *game)
{
    background(game);
    //al_draw_bitmap(game->background,0,0,0);
    drawhp(game);
    //*********PORTAL*************
    if (game->room->type == 3 && game->room->enemycnt == 0)
    {
        if (game->portalstate >= 6) game->portalstate = 0;
        al_draw_scaled_bitmap(game->portal,242*game->portalstate,0,242,95,PORTALPOSX,PORTALPOSY,60,24,0);
    }
    int i, j, tmp, tmp2;

    enemy_sort(game);
    shot_sort(game);
    bomb_sort(game);
    pickup_sort(game);
    int enemy, shot, bomb, pickup, item;
    enemy = 0;
    shot = 0;
    bomb = 0;
    pickup = 0;
    item = 0;
    game->objects[0][0] = 1;
    game->objects[0][1] = game->bunny.posy;
    if (game->room->enemycnt > 0)
    {
        game->objects[1][0] = 2;
        game->objects[1][1] = game->room->enemies[0].posy;
    }
    else
    {
        game->objectcnt--;
        game->objects[1][0] = 0;
        game->objects[1][1] = 800+game->objectcnt;
    }
    if (game->shotcnt > 0)
    {
        game->objects[2][0] = 3;
        game->objects[2][1] = game->shots[0].posy;
    }
    else
    {
        game->objectcnt--;
        game->objects[2][0] = 0;
        game->objects[2][1] = 800+game->objectcnt;
    }
    if (game->room->bombcnt > 0)
    {
        game->objects[3][0] = 4;
        game->objects[3][1] = game->room->bombs[0].posy;
    }
    else
    {
        game->objectcnt--;
        game->objects[3][0] = 0;
        game->objects[3][1] = 800+game->objectcnt;
    }
    if (game->room->pickupcnt > 0)
    {
        game->objects[4][0] = 5;
        game->objects[4][1] = game->room->pickups[0].posy;
    }
    else
    {
        game->objectcnt--;
        game->objects[4][0] = 0;
        game->objects[4][1] = 800+game->objectcnt;
    }
    if (game->room->itemcnt > 0)
    {
        game->objects[5][0] = 6;
        game->objects[5][1] = game->room->items[0].posy;
    }
    else
    {
        game->objectcnt--;
        game->objects[5][0] = 0;
        game->objects[5][1] = 800+game->objectcnt;
    }

    object_sort(game);
    while(game->objects[0][0] != 0)
    {
        switch (game->objects[0][0])
        {
            case 1:
                drawbunny(game);
                game->objects[0][0] = 0;
                game->objects[0][1] = 800 + game->objectcnt;
                break;
            case 2:
                drawenemy(game, enemy);
                enemy++;
                if (enemy==game->room->enemycnt)
                {
                    game->objects[0][0] = 0;
                    game->objects[0][1] = 800+game->objectcnt;
                }
                else
                    game->objects[0][1] = game->room->enemies[enemy].posy;
                break;
            case 3:
                drawshot(game, shot);
                shot++;
                if (shot==game->shotcnt)
                {
                    game->objects[0][0] = 0;
                    game->objects[0][1] = 800+game->objectcnt;
                }
                else
                    game->objects[0][1] = game->shots[shot].posy;
                break;
            case 4:
                drawbomb(game, bomb);
                bomb++;
                if (bomb==game->room->bombcnt)
                {
                    game->objects[0][0] = 0;
                    game->objects[0][1] = 800+game->objectcnt;
                }
                else
                    game->objects[0][1] = game->room->bombs[bomb].posy;
                break;
            case 5:
                drawpickup(game, pickup);
                pickup++;
                if (pickup==game->room->pickupcnt)
                {
                    game->objects[0][0] = 0;
                    game->objects[0][1] = 800+game->objectcnt;
                }
                else
                    game->objects[0][1] = game->room->pickups[pickup].posy;
                break;
            case 6:
                drawitem(game, item);
                item++;
                if (item==game->room->itemcnt)
                {
                    game->objects[0][0] = 0;
                    game->objects[0][1] = 800+game->objectcnt;
                }
                else
                    game->objects[0][1] = game->room->items[item].posy;
                break;
        }
        object_sort(game);
    }
    //****dymki*******
    Bomb bombb;
    double time;
    i = 0;
    while(i<game->room->bombcnt)
    {
        bombb = game->room->bombs[i];
        time = bombb.time - al_get_time();
        if (time < 0)
        {
            deletebomb(game, game->room, i);
        }
        else if (time < 0.3)
        { // wybuch bomby
            if (game->room->bombs[i].exploded == 0)
            {
                game->room->bombs[i].exploded = 1;
                //otwieranie drzwi
                if (game->room->up!=NULL && game->room->doorsopen[0] == 0 && pow(game->room->bombs[i].posx+17-400,2)+pow(game->room->bombs[i].posy-17-100,2) < pow(85,2) )
                {
                    game->room->doorsopen[0] = 1;
                    if (game->room->up->type == 5 || game->room->type == 5)
                        game->room->up->doorsopen[2] = 1;
                }
                if (game->room->right!=NULL && game->room->doorsopen[1] == 0 && pow(game->room->bombs[i].posx+17-750,2)+pow(game->room->bombs[i].posy-17-325,2) < pow(85,2) )
                {
                    game->room->doorsopen[1] = 1;
                    if (game->room->right->type == 5 || game->room->type == 5)
                        game->room->right->doorsopen[3] = 1;
                }
                if (game->room->down!=NULL && game->room->doorsopen[2] == 0 && pow(game->room->bombs[i].posx+17-400,2)+pow(game->room->bombs[i].posy-17-550,2) < pow(85,2) )
                {
                    game->room->doorsopen[2] = 1;
                    if (game->room->down->type == 5 || game->room->type == 5)
                        game->room->down->doorsopen[0] = 1;
                }
                if (game->room->left!=NULL && game->room->doorsopen[3] == 0 && pow(game->room->bombs[i].posx+17-50,2)+pow(game->room->bombs[i].posy-17-325,2) < pow(85,2) )
                {
                    game->room->doorsopen[3] = 1;
                    if (game->room->left->type == 5 || game->room->type == 5)
                        game->room->left->doorsopen[1] = 1;
                }
                // ranienie krolisia
                if (pow(game->bunny.posx+25-game->room->bombs[i].posx-17,2)+pow(game->bunny.posy-20-game->room->bombs[i].posy+17,2) < pow(85,2))
                {
                    game->bunny.hp-=2;
                    game->bunny.immortal = al_get_time()+1;
                }
                int j = 0;
                while(j<game->room->enemycnt) //ranienie wrogow
                {
                    if ( pow(game->room->enemies[j].posx+game->room->enemies[j].szer/2 - game->room->bombs[i].posx-17,2)
                        + pow(game->room->enemies[j].posy-game->room->enemies[j].grub/2 - game->room->bombs[i].posy+17,2)
                        < pow(85,2))
                    {
                        game->room->enemies[j].hp -= 200;
                        if (game->room->enemies[j].hp <= 0)
                            deleteenemy(game, j);
                        j--;
                    }
                    j++;
                }
            }
            if (time > 0.2)
                al_draw_tinted_bitmap(game->dym,al_map_rgba(0,0,0,100),bombb.posx-69,bombb.posy-95,0);
            else if (time > 0.1)
                al_draw_tinted_bitmap(game->dym,al_map_rgba(0,0,0,50),bombb.posx-69,bombb.posy-95,1);
            else
                al_draw_tinted_bitmap(game->dym,al_map_rgba(0,0,0,20),bombb.posx-69,bombb.posy-95,2);
        }
        i++;
    }
    al_flip_display();
}

void drawpause(Game *game)
{
    al_draw_bitmap(game->pause,0,0,0);
}
//*************GRA******************
void newgame(Game* game)
{
    game->bunny.posx = 375;
    game->bunny.posy = 370;
    game->bunny.posz = 0;
    game->bunny.szer = 50;
    game->bunny.wys = 98;
    game->bunny.grub = 25;
    game->bunny.statex = 0;
    game->bunny.statey = 0;
    game->bunny.step = 2.5;

    game->bunny.shotfreq = 0.6;
    game->bunny.range = 300;
    game->bunny.dmg = 20;
    game->bunny.speed = 0.009;

    game->bunny.shotspeed = 4;
    game->bunny.shottype = 0;
    game->bunny.kicspeed = 0.09;
    game->shotcnt = 0;
    game->bunny.lastshot = al_get_time();
    game->levelnum = 0;
    game->maxlevelnum = 2;
    game->bunny.hp = 6;
    game->bunny.hearts = 3;
    game->bunny.immortal = al_get_time();
    game->coins = 5;
    game->bombs = 3;
    game->keys = 1;
    game->spaceitem = -1;
    game->spaceleft = 0;
    game->bunny.battery = 0;
    game->bunny.boots = 0;
    int i;
    for (i=0; i<LEVELTYPES; i++)
        game->usedlevels[i] = 0;
    for (i=0; i<ITEMS; i++)
        game->useditems[i] = 0;
    game->roomeffect[0] = 0;
    game->roomeffect[1] = 0;
    game->roomeffect[2] = 0;
}

void changeroom(Game* game)
{
    noshots(game);
    game->bunny.shotfreq -= game->roomeffect[0];
    game->bunny.dmg -= game->roomeffect[1];
    game->bunny.shottype -= game->roomeffect[2];
    game->roomeffect[0] = 0;
    game->roomeffect[1] = 0;
    game->roomeffect[2] = 0;
    Bomb bomb;
    int i;
    while(i<game->room->bombcnt)
    {
        bomb.time = al_get_time() + 2;
        i++;
    }
    if (game->room->enemycnt > 0)
    {
        if (game->room->up!=NULL)
        {
            if (game->room->up->type != 5)
            {
                game->room->doorsopen[0] = 0;
            }
        }
        if (game->room->right!=NULL)
        {
            if (game->room->right->type != 5)
            {
                game->room->doorsopen[1] = 0;
            }
        }
        if (game->room->down!=NULL)
        {
            if (game->room->down->type != 5)
            {
                game->room->doorsopen[2] = 0;
            }
        }
        if (game->room->left!=NULL)
        {
            if (game->room->left->type != 5)
            {
                game->room->doorsopen[3] = 0;
            }
        }
    }
    i = 0;
    while(i<game->room->enemycnt)
    {
        game->room->enemies[i].lastshot = (rand()%10)/10.0 + al_get_time();
        i++;
    }
}

void newlevel(Game *game)
{
    game->levelnum++;
    printf("levelnum %d\n", game->levelnum);
    game->roomcnt = game->levelnum+1;
    int lvlcnt, lvl, random, item, j;
    char bitmapname[24];
    char logoname[24];
    int type;
    Level level;
    FILE* fp;
    fp = fopen("files/levels.txt","r");
    fscanf(fp, "%d", &lvlcnt);
    lvl = rand()%lvlcnt;
    while(game->usedlevels[lvl] == 1)
    {
        lvl = rand()%lvlcnt;
    }
    game->usedlevels[lvl] = 1;
    int i = 0;
    while(i<=lvl)
    {
        fscanf(fp,"%s", &bitmapname);
        fscanf(fp, "%s", &logoname);
        fscanf(fp, "%d %d %d %d %d %d", &level.bossfirst, &level.bosslast, &level.enemyfirst, &level.enemylast,
            &level.itemfirst, &level.itemlast);
        i++;
    }
    fclose(fp);
    printf("lvl %d", lvl);
    level.picture = al_load_bitmap(bitmapname);
    level.logo = al_load_bitmap(logoname);
    al_draw_bitmap(level.logo,0,0,0);
    al_flip_display();
    game->level = level;
    Room *room;
    Room *tmp;
    room = (Room*)malloc(sizeof(Room));
    room->type = 0;
    room->left = NULL;
    room->right = NULL;
    room->up = NULL;
    room->down = NULL;
    room->enemycnt = 0;
    room->itemcnt = 0;
    room->pickupcnt = 0;
    room->bombcnt = 0;
    room->locked = 0;
    for (i=0; i<4; i++)
    {
        room->doorsopen[i] = 1;
    }
    game->room = room;
    game->firstroom = room;
    tmp = room;
    for (i=0; i<game->roomcnt; i++)
    {
        room = (Room*)malloc(sizeof(Room));
        room->left = tmp;
        tmp->right = room;
        room->type = 0;
        room->enemycnt = 0;
        for (j=0; j<1; j++)
        {
            newenemy(game, room, rand()%4, rand()%700+50, rand()%450+100, 0);
        }
        room->itemcnt = 0;
        room->pickupcnt = 0;
        room->bombcnt = 0;
        room->right = NULL;
        room->up = NULL;
        room->down = NULL;
        room->locked = 0;
        for (j=0; j<4; j++)
        {
            room->doorsopen[j] = 1;
        }
        tmp = room;
    }//*********BOSS ROOM*****************
        room = (Room*)malloc(sizeof(Room));
        room->left = tmp;
        tmp->right = room;
        room->type = 3;
        room->enemycnt = 0;
        for (j=0; j<5; j++)
        {
            newenemy(game, room, rand()%4, rand()%700+50, rand()%450+100, 0);
        }
        room->itemcnt = 0;
        room->pickupcnt = 0;
        room->bombcnt = 0;
        room->right = NULL;
        room->up = NULL;
        room->down = NULL;
        room->locked = 0;
        for (i=0; i<4; i++)
        {
            room->doorsopen[i] = 1;
        }
       //************ITEM ROOM**************
       tmp = game->room;
       random = rand()%(game->roomcnt);
       for (i=0; i<=random; i++)
       {
           tmp = tmp->right;
       }
        room = (Room*)malloc(sizeof(Room));
        room->type = 1;
        room->enemycnt = 0;
        room->itemcnt = 0;
        room->pickupcnt = 0;
        room->bombcnt = 0;
        random = rand()%2;
        if (random == 0)
        {
            random = rand()%BASICITEMS;
            while(game->useditems[random]==1)
            {
                random = rand()%BASICITEMS;
            }
        }
        else
        {
            random = rand()%(game->level.itemlast - game->level.itemfirst+1)+game->level.itemfirst;
            while(game->useditems[random]==1)
            {
                random = rand()%(game->level.itemlast - game->level.itemfirst+1)+game->level.itemfirst;
            }
        }
        item = random;
        newitem(game, room, 0, random, 378, 318);
        room->left = NULL;
        room->right = NULL;
        room->down = NULL;
        room->up = NULL;
        room->locked = 1;
        for (i=0; i<4; i++)
        {
            room->doorsopen[i] = 1;
        }
        random = rand()%2;
        if (random==1)
        {
            tmp->down = room;
            room->up = tmp;
        }
        else
        {
            tmp->up = room;
            room->down = tmp;
        }
        //**********BONUS ROOM**************
        room = (Room*)malloc(sizeof(Room));
        room->type = 2;
        room->enemycnt = 0;
        room->itemcnt = 0;
        room->pickupcnt = 0;
        room->bombcnt = 0;
        newpickup(game, room, rand()%4, rand()%665+50, rand()%415+100,0);
        room->down = NULL;
        room->up = NULL;
        room->left = NULL;
        room->right = NULL;
        room->locked = 0;
        for (i=0; i<4; i++)
        {
            room->doorsopen[i] = 1;
        }
        tmp = game->room;
        random = rand()%(game->roomcnt);
        for (i=0; i<=random; i++)
        {
            tmp = tmp->right;
        }
        random = rand()%2;
        while ((random==0 && tmp->up!=NULL) || (random==1 && tmp->down!=NULL))
        {
            tmp = game->room;
            random = rand()%(game->roomcnt);
            for (i=0; i<=random; i++)
            {
                tmp = tmp->right;
            }
            random = rand()%2;
        }
        if (random==1)
        {
            tmp->down = room;
            room->up = tmp;
            if (tmp->left->down!=NULL)
            {

                tmp = tmp->left->down;
                tmp->right = room;
                room->left = tmp;
            }
            if (tmp->right->down!=NULL)
            {
                tmp = tmp->right->down;
                tmp->left = room;
                room->right = tmp;
            }
        }
        else
        {
            tmp->up = room;
            room->down = tmp;
            if (tmp->left->up!=NULL)
            {
                tmp = tmp->left->up;
                tmp->right = room;
                room->left = tmp;
            }
            if (tmp->right->up!=NULL)
            {
                tmp = tmp->right->up;
                tmp->left = room;
                room->right = tmp;
            }
        }
        //***********SHOP***************
        room = (Room*)malloc(sizeof(Room));
        room->type = 4;
        room->enemycnt = 0;
        room->itemcnt = 0;
        room->pickupcnt = 0;
        room->bombcnt = 0;
        room->down = NULL;
        room->up = NULL;
        room->left = NULL;
        room->right = NULL;
        room->locked = 1;
        for (i=0; i<4; i++)
        {
            room->doorsopen[i] = 1;
        }
        tmp = game->room;
        random = rand()%(game->roomcnt);
        for (i=0; i<=random; i++)
        {
            tmp = tmp->right;
        }

        random = rand()%2;
        while ((random==0 && tmp->up!=NULL) || (random==1 && tmp->down!=NULL))
        {
            random = rand()%(game->roomcnt);
            tmp = game->room;
            for (i=0; i<=random; i++)
            {
                tmp = tmp->right;
            }
            random = rand()%2;
        }
        if (random==1)
        {
            tmp->down = room;
            room->up = tmp;
            if (tmp->left->down!=NULL)
            {

                tmp = tmp->left->down;
                tmp->right = room;
                room->left = tmp;
            }
            if (tmp->right->down!=NULL)
            {
                tmp = tmp->right->down;
                tmp->left = room;
                room->right = tmp;
            }
        }
        else
        {
            Room *temp;
            tmp->up = room;
            room->down = tmp;
            if (tmp->left->up!=NULL)
            {
                temp = tmp->left->up;
                temp->right = room;
                room->left = temp;
            }
            if (tmp->right->up!=NULL)
            {
                temp = tmp->right->up;
                temp->left = room;
                room->right = temp;
            }
        }
        random = rand()%SHOPITEMS+BASICITEMS;
        while(game->useditems[random]==1 || random==item)
        {
            random = rand()%SHOPITEMS+BASICITEMS;
        }
        newitem(game, room, (rand()%3)*5+5, random, 308, 323);
        newpickup(game, room, rand()%3+1, 448, 323, rand()%2+1);

        //*************SECRET ROOM****************
        room = (Room*)malloc(sizeof(Room));
        room->type = 5;
        room->enemycnt = 0;
        room->itemcnt = 0;
        room->pickupcnt = 0;
        room->bombcnt = 0;
        room->down = NULL;
        room->up = NULL;
        room->left = NULL;
        room->right = NULL;
        room->locked = 0;
        for (i=0; i<4; i++)
        {
            room->doorsopen[i] = 0;
        }
        tmp = game->room;
        random = rand()%(game->roomcnt);
        for (i=0; i<=random; i++)
        {
            tmp = tmp->right;
        }

        random = rand()%2;
        while ((random==0 && tmp->up!=NULL) || (random==1 && tmp->down!=NULL))
        {
            random = rand()%(game->roomcnt);
            tmp = game->room;
            for (i=0; i<=random; i++)
            {
                tmp = tmp->right;
            }
            random = rand()%2;
        }
        if (random==1)
        {
            Room *temp;
            tmp->down = room;
            room->up = tmp;
            tmp->doorsopen[2] = 0;
            if (tmp->left->down!=NULL)
            {
                temp = tmp->left->down;
                temp->right = room;
                room->left = temp;
                if (temp->type != 2)
                    temp->doorsopen[1] = 0;
                else
                    room->doorsopen[3] = 1;
            }
            if (tmp->right->down!=NULL)
            {
                temp = tmp->right->down;
                temp->left = room;
                room->right = temp;
                if (temp->type != 2)
                    temp->doorsopen[3] = 0;
                else
                    room->doorsopen[1] = 1;
            }
        }
        else
        {
            Room *temp;
            tmp->up = room;
            room->down = tmp;
            tmp->doorsopen[0] = 0;
            if (tmp->left->up!=NULL)
            {
                temp = tmp->left->up;
                temp->right = room;
                room->left = temp;
                if (temp->type != 2)
                {
                    temp->doorsopen[1] = 0;
                }
                else
                    room->doorsopen[3] = 1;
            }
            if (tmp->right->up!=NULL)
            {
                temp = tmp->right->up;
                temp->left = room;
                room->right = temp;
                if (temp->type != 2)
                    temp->doorsopen[3] = 0;
                else
                    room->doorsopen[1] = 1;
            }
        }
        newitem(game, room, 0, 2, 378, 318);
        //*************ANIMACJA***********
        ALLEGRO_BITMAP *bunny;
        bunny = al_create_sub_bitmap(game->pickups,247,0,277,186);
        int posy = -139;
        al_rest(0.5);
        al_get_keyboard_state(&game->keyboard);
        while(posy<600 && !al_key_down(&game->keyboard, ALLEGRO_KEY_ENTER))
        {
            al_get_keyboard_state(&game->keyboard);
            al_draw_bitmap(level.logo,0,0,0);
            if ((posy+139)%200 < 100)
                al_draw_bitmap_region(bunny,0,0,139,186,320,posy,0);
            else
                al_draw_bitmap_region(bunny,139,0,138,186,309,posy,0);
            posy += 2;
            al_rest(0.007);
            al_flip_display();
        }
}

void cleanlevel(Game *game)
{
    Room *room;
    Room *roomdel;
    room = game->firstroom;
    while (room!=NULL)
    {
        if (room->up!=NULL)
        {
            roomdel = room->up;
            //noitems(room->up);
            //nopickups(room->up);
            //nobombs(room->up);
            free(roomdel);
        }
        if (room->down!=NULL)
        {
            roomdel = room->down;
            //noitems(room->down);
            //nopickups(room->down);
            //nobombs(room->down);
            free(roomdel);
        }
        roomdel = room;
        //noitems(room);
        //nopickups(room);
        //nobombs(room);
        room = room->right;
        free(roomdel);
    }
    game->firstroom = NULL;
    game->room = NULL;
}

void start(Game *game)
{
    newlevel(game);
    int i;
    double time = al_get_time();
    double czas = al_get_time();
    double statetime = al_get_time();
    ALLEGRO_FONT * font8 = al_create_builtin_font();
    while( !(game->bunny.hp <= 0) && !al_key_down( &game->keyboard, ALLEGRO_KEY_ESCAPE) && game->levelnum<=game->maxlevelnum)
    {
        al_get_keyboard_state(&game->keyboard);
        //****************PAUZA*******************
        if (al_key_down(&game->keyboard, ALLEGRO_KEY_P))
        {
            //drawpause(game);
            al_rest(1);
            al_get_keyboard_state(&game->keyboard);
            while(!al_key_down(&game->keyboard, ALLEGRO_KEY_ENTER))
            {
                al_get_keyboard_state(&game->keyboard);
                //drawpause(game);
            }
            al_get_keyboard_state(&game->keyboard);
        }
//**********************KICANIE**********************
        if (al_get_time() > statetime+game->bunny.kicspeed && (al_key_down(&game->keyboard, ALLEGRO_KEY_D) || al_key_down(&game->keyboard, ALLEGRO_KEY_A) ||
            al_key_down(&game->keyboard, ALLEGRO_KEY_S) || al_key_down(&game->keyboard, ALLEGRO_KEY_W)))
        {
            game->bunny.statex++;
            statetime = al_get_time();
        }
        else if(al_get_time() > statetime+game->bunny.kicspeed && game->bunny.statex>0)
        {
            game->bunny.statex++;
            statetime = al_get_time();
        }
        else if(al_get_time() > statetime+game->bunny.kicspeed && game->bunny.statey>0)
        {
            game->bunny.statey = 0;
            statetime = al_get_time();
        }//********************BIEGANIE*******************
        if (al_get_time() > czas + game->bunny.speed)
        {
            if ( al_key_down(&game->keyboard, ALLEGRO_KEY_W))
            {
                if (game->bunny.posy-game->bunny.grub > 100)
                {
                    if (!al_key_down(&game->keyboard, ALLEGRO_KEY_A) && !al_key_down(&game->keyboard, ALLEGRO_KEY_D))
                        game->bunny.posy-=game->bunny.step;
                    else
                        game->bunny.posy-=game->bunny.step/sqrt(2);

                }
                game->bunny.statey = 2;
                if (game->room->up!=NULL && (game->room->up->locked == 0 || game->keys > 0 ||
                    game->room->type == 2 || game->room->type == 5) && game->room->doorsopen[0] > 0 &&
                    game->bunny.posx>370 && game->bunny.posx<380 && game->bunny.posy <= 150)
                {
                    if (game->room->up->type == 2 || game->room->type == 2)
                    {
                        if (game->bunny.boots == 0)
                        {
                        game->bunny.hp--;
                        game->bunny.immortal = al_get_time()+1;
                        }
                        if (game->room->up->locked == 1)
                            game->room->up->locked = 0;
                    }
                    else if (game->room->type == 5 && game->room->up->locked == 1)
                    {
                        game->room->up->locked = 0;
                    }
                    else if (game->room->up->locked == 1)
                    {
                        game->keys--;
                        game->room->up->locked = 0;
                    }
                    for(i=0; i<20/game->bunny.step; i++)
                    {
                        game->bunny.posy -= game->bunny.step;
                        draw(game);
                        al_rest(game->bunny.speed);
                    }
                    Item item;
                    i = 0;
                    while(i<game->room->itemcnt)
                    {
                        item = game->room->items[i];
                        if (item.taken == 1)
                        {
                            deleteitem(game->room,i);
                            i--;
                        }
                        i++;
                    }
                    game->room = game->room->up;
                    game->bunny.posy = 550;
                    changeroom(game);
                }
            }
            if ( al_key_down(&game->keyboard, ALLEGRO_KEY_S))
            {
                if (game->bunny.posy < 540)
                {
                    if (!al_key_down(&game->keyboard, ALLEGRO_KEY_A) && !al_key_down(&game->keyboard, ALLEGRO_KEY_D))
                        game->bunny.posy+=game->bunny.step;
                    else
                        game->bunny.posy+=game->bunny.step/sqrt(2);
                }
                game->bunny.statey = 0;
                if (game->room->down!=NULL && (game->room->down->locked == 0 || game->keys > 0 ||
                    game->room->type == 2 || game->room->type == 5) && game->room->doorsopen[2] > 0 &&
                    game->bunny.posx>370 && game->bunny.posx<380 && game->bunny.posy >= 540)
                {
                    if (game->room->down->type == 2 || game->room->type == 2)
                    {
                        if (game->bunny.boots == 0)
                        {
                            game->bunny.hp--;
                            game->bunny.immortal = al_get_time()+1;
                        }
                        if (game->room->down->locked == 1)
                            game->room->down->locked = 0;
                    }
                    else if (game->room->type == 5 && game->room->down->locked == 1)
                    {
                        game->room->down->locked = 0;
                    }
                    else if (game->room->down->locked == 1)
                    {
                        game->keys--;
                        game->room->down->locked = 0;
                    }
                    for(i=0; i<20/game->bunny.step; i++)
                    {
                        game->bunny.posy += game->bunny.step;
                        draw(game);
                        al_rest(game->bunny.speed);
                    }
                    Item item;
                    i = 0;
                    while(i<game->room->itemcnt)
                    {
                        item = game->room->items[i];
                        if (item.taken == 1)
                        {
                            deleteitem(game->room,i);
                            i--;
                        }
                        i++;
                    }
                    game->room = game->room->down;
                    game->bunny.posy = 100+game->bunny.grub;
                    changeroom(game);
                }
            }
            if ( al_key_down(&game->keyboard, ALLEGRO_KEY_D))
            {
                if (game->bunny.posx < 705)
                {
                    if (!al_key_down(&game->keyboard, ALLEGRO_KEY_W) && !al_key_down(&game->keyboard, ALLEGRO_KEY_S))
                        game->bunny.posx += game->bunny.step;
                    else
                        game->bunny.posx += game->bunny.step/sqrt(2);
                }
                game->bunny.statey = 1;
                if (game->room->right!=NULL && (game->room->right->locked == 0 || game->keys > 0 ||
                    game->room->type == 2 || game->room->type == 5) && game->room->doorsopen[1] > 0 &&
                    game->bunny.posy>340 && game->bunny.posy<355 && game->bunny.posx >= 705)
                {
                    if (game->room->right->type == 2 || game->room->type == 2)
                    {
                        if (game->bunny.boots == 0)
                        {
                            game->bunny.hp--;
                            game->bunny.immortal = al_get_time()+1;
                        }
                        if (game->room->right->locked == 1)
                            game->room->right->locked = 0;
                    }
                    else if (game->room->type == 5 && game->room->right->locked == 1)
                    {
                        game->room->right->locked = 0;
                    }
                    else if (game->room->right->locked == 1)
                    {
                        game->keys--;
                        game->room->right->locked = 0;
                    }
                    for(i=0; i<20/game->bunny.step; i++)
                    {
                        game->bunny.posx += game->bunny.step;
                        draw(game);
                        al_rest(game->bunny.speed);
                    }
                    Item item;
                    i = 0;
                    while(i<game->room->itemcnt)
                    {
                        item = game->room->items[i];
                        if (item.taken == 1)
                        {
                            deleteitem(game->room,i);
                            i--;
                        }
                        i++;
                    }
                    game->room = game->room->right;
                    game->bunny.posx = 45;
                    changeroom(game);
                }
            }
            if ( al_key_down(&game->keyboard, ALLEGRO_KEY_A))
            {
                if (game->bunny.posx > 45)
                {
                    if (!al_key_down(&game->keyboard, ALLEGRO_KEY_W) && !al_key_down(&game->keyboard, ALLEGRO_KEY_S))
                        game->bunny.posx-=game->bunny.step;
                    else
                        game->bunny.posx-=game->bunny.step/sqrt(2);
                }
                game->bunny.statey = 3;
                if (game->bunny.posy>340 && game->bunny.posy<355 && game->bunny.posx <= 45 &&
                    game->room->left!=NULL && (game->room->left->locked == 0 || game->keys > 0 ||
                    game->room->type == 2 || game->room->type == 5) && game->room->doorsopen[3] > 0)
                {
                    if (game->room->left->type == 2 || game->room->type == 2)
                    {
                        if (game->bunny.boots == 0)
                        {
                            game->bunny.hp--;
                            game->bunny.immortal = al_get_time()+1;
                        }
                        if (game->room->left->locked == 1)
                            game->room->left->locked = 0;
                    }
                    else if (game->room->type == 5 && game->room->left->locked == 1)
                    {
                        game->room->left->locked = 0;
                    }
                    else if (game->room->left->locked == 1)
                    {
                        game->keys--;
                        game->room->left->locked = 0;
                    }
                    for(i=0; i<20/game->bunny.step; i++)
                    {
                        game->bunny.posx -= game->bunny.step;
                        draw(game);
                        al_rest(game->bunny.speed);
                    }
                    Item item;
                    i = 0;
                    while(i<game->room->itemcnt)
                    {
                        item = game->room->items[i];
                        if (item.taken == 1)
                        {
                            deleteitem(game->room,i);
                            i--;
                        }
                        i++;
                    }
                    game->room = game->room->left;
                    game->bunny.posx = game->szer - game->bunny.szer - 45;
                    changeroom(game);
                }
            }
            //*************BOMBY******************
            if (al_key_down(&game->keyboard, ALLEGRO_KEY_LSHIFT) && game->room->bombcnt == 0 && game->bombs>0)
            {
                newbomb(game, game->room, game->bunny.posx+7, game->bunny.posy, 1, 0, 3);
                game->bombs--;
            }
            czas = al_get_time();
        }//**************STRZELANIE******************
        if (al_get_time() > game->bunny.lastshot + game->bunny.shotfreq)
        {
            //printf("szczela");
            if ( al_key_down(&game->keyboard, ALLEGRO_KEY_LEFT))
            {
                newshot(game, game->bunny.shotpic, game->bunny.posx + 15, game->bunny.posy, 38,
                        20,20,-game->bunny.shotspeed, 0, game->bunny.range, 0, game->bunny.shottype, game->bunny.dmg);

                game->bunny.lastshot = al_get_time();
            }
            else if ( al_key_down(&game->keyboard, ALLEGRO_KEY_RIGHT))
            {
                newshot(game, game->bunny.shotpic, game->bunny.posx + 15, game->bunny.posy, 38,
                        20,20,game->bunny.shotspeed, 0, game->bunny.range, 0, game->bunny.shottype, game->bunny.dmg);
                game->bunny.lastshot = al_get_time();
            }
            else if ( al_key_down(&game->keyboard, ALLEGRO_KEY_UP))
            {
                newshot(game, game->bunny.shotpic, game->bunny.posx + 15, game->bunny.posy, 38,
                        20,20,0, -game->bunny.shotspeed, game->bunny.range, 0, game->bunny.shottype, game->bunny.dmg);
                game->bunny.lastshot = al_get_time();
            }
            else if ( al_key_down(&game->keyboard, ALLEGRO_KEY_DOWN))
            {
                newshot(game, game->bunny.shotpic, game->bunny.posx + 15, game->bunny.posy, 38,
                        20,20,0, game->bunny.shotspeed, game->bunny.range, 0, game->bunny.shottype, game->bunny.dmg);
                game->bunny.lastshot = al_get_time();
            }
        }
        //***********SPACJA*****************
        if (al_key_down(&game->keyboard, ALLEGRO_KEY_SPACE) && game->spaceitem>=0 && game->spaceleft <= 0)
        {
            spaceeffect(game, game->spaceitem);
        }
        if (al_get_time() > time + 0.01)
        {//***********WPADANIE DO PORTALU**********
            time = al_get_time();
            if (game->room->type == 3 && game->room->enemycnt == 0)
            {
                game->portalstate++;
                if (game->bunny.posy<=PORTALPOSY+24 && game->bunny.posy>=PORTALPOSY &&
                        game->bunny.posx+40>=PORTALPOSX && game->bunny.posx+10<=PORTALPOSX+60
                        && (game->room->itemcnt == 0 || game->room->items[0].taken == 1))
                        {
                            //*************ANIMACJA****************
                            noshots(game);
                            game->bunny.statey = 0;
                            game->bunny.statex = 0;
                            game->bunny.posx = PORTALPOSX+5;
                            game->bunny.posy = PORTALPOSY-6;
                            for (i=0; i<3; i++)
                            {
                                game->portalstate++;
                                draw(game);
                                al_rest(0.1);
                                game->bunny.statex++;
                            }
                            while(game->bunny.posy<PORTALPOSY+26)
                            {
                                game->portalstate++;
                                game->bunny.posy++;
                                al_rest(0.005);
                                draw(game);
                            }
                            if (game->levelnum<game->maxlevelnum)
                            {
                                cleanlevel(game);
                                newlevel(game);
                                game->bunny.posy=370;
                                while(game->bunny.posy<400)
                                {
                                    game->bunny.posy++;
                                    al_rest(0.001);
                                    draw(game);
                                }
                            }
                            else
                            {
                                game->levelnum++;
                                printf("levelnum %d\n", game->levelnum);
                            }
                        }
            }
            //*************PODNOSZENIE**********
            Pickup pickup;
            i = 0;
            while(i < game->room->pickupcnt)
            {
                pickup = game->room->pickups[i];
                if(game->bunny.posx-25<=pickup.posx && game->bunny.posx+40>=pickup.posx &&
                    game->bunny.posy>=pickup.posy-35 && game->bunny.posy-70<=pickup.posy-35 &&
                    pickup.price<=game->coins && pickup.posz == 0)
                    {
                        if(!(pickup.type == 1 && game->bunny.hp == 2*game->bunny.hearts))
                        {
                            game->coins -= pickup.price;
                            switch(pickup.type)
                            {
                                case 1: if (game->bunny.hp<2*game->bunny.hearts) game->bunny.hp++; break;
                                case 0: if (game->coins<99) game->coins++; break;
                                case 2: if (game->bombs<99) game->bombs++; break;
                                case 3: if (game->keys<99) game->keys++; break;
                            }
                            deletepickup(game->room,i);
                            i--;
                        }
                    }
                    i++;
            }
            Item item;
            i = 0;
            while(i<game->room->itemcnt)
            {
                if(game->room->items[i].taken == 0 && game->bunny.posx-25<=game->room->items[i].posx
                    && game->bunny.posx+40>=game->room->items[i].posx &&
                    game->bunny.posy>=game->room->items[i].posy-45 && game->bunny.posy-70<=game->room->items[i].posy-45
                    && game->room->items[i].price<=game->coins && game->room->items[i].frozen == 0)
                {
                    game->coins -= game->room->items[i].price;
                    game->room->items[i].price = 0;
                    game->room->items[i].taken = 1;
                    game->room->items[i].frozen = 1;
                    itemeffect(game, game->room->items[i].number, i);
                    game->useditems[game->room->items[i].number] = 1;
                }
                else if (!(game->bunny.posx-25<=game->room->items[i].posx && game->bunny.posx+40>=game->room->items[i].posx
                           && game->bunny.posy>=game->room->items[i].posy-45 && game->bunny.posy-70<=game->room->items[i].posy-45))
                    game->room->items[i].frozen = 0;
                i++;
            }
            shotsmove(game);
            i = 0;
            while (i < game->room->enemycnt)
            {
                enemyaction(game,i);
                enemymove(game,i);
                enemyshot(game,i);
                i++;
            }
            draw(game);
        }
        //else printf("o");
    }
    if (game->levelnum > game->maxlevelnum)
    {
        //**********WYGRANA - ANIMACJA***********
    }
}

void intro(Game* game)
{
    FILE* fp;
    fp = fopen("files/intro.txt","r");
    double restlength = 0;
    int sx,sy,dx,dy,px,py;
    double time = al_get_time();
    int i = 0;
    ALLEGRO_BITMAP *obraz;
    al_get_keyboard_state(&game->keyboard);
    while(!al_key_down(&game->keyboard,ALLEGRO_KEY_ENTER))
    {
        al_get_keyboard_state(&game->keyboard);
        if (al_get_time() > time + restlength)
        {
            if (feof(fp)) break;
            char bitmapname[25];
            fscanf(fp, "%s %lf", &bitmapname, &restlength);
            printf("%d %s %lf\n", i, bitmapname,restlength);
            //fscanf(fp,"%d %d %d %d %d %d",&sx,&sy,&dx,&dy,&px,&py);
            //if (obraz) al_destroy_bitmap(obraz);
            //al_draw_to_color
            obraz = al_load_bitmap(bitmapname);
            al_draw_bitmap(obraz,0,0,0);
            //al_draw_bitmap_region(obraz,sx,sy,dx,dy,px,py,0);
            time = al_get_time();
            al_flip_display();
            i++;
        }
    }
    al_destroy_bitmap(obraz);
    fclose(fp);
}

int main()
{
    srand(time(NULL));
    al_init();
    al_install_keyboard();
    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    Game game;
    game.szer = 800;
    game.wys = 600;
    game.display = al_create_display(game.szer, game.wys);
    al_set_window_title(game.display,"Where's the cake?!");
    game.state = 0;
    game.menu.choice = 0;
    game.menu.picture = al_load_bitmap("backgrounds/menu01.bmp");
    game.menu.screen = 0;
    game.sounds = 1;
    game.music = 1;
    game.bunny.picture = al_load_bitmap("pics/bunny.png");
    game.bunny.shotpic = al_load_bitmap("pics/bunnyshot.png");
    game.hearts = al_load_bitmap("pics/hearts.bmp");
    game.pickups = al_load_bitmap("pics/pickups.png");
    al_convert_mask_to_alpha(game.pickups, al_map_rgb(255,255,255));
    game.cyferki = al_load_bitmap("pics/cyferki.bmp");
    al_convert_mask_to_alpha(game.cyferki, al_map_rgb(0,0,0));
    game.doors = al_load_bitmap("pics/doors.png");
    al_convert_mask_to_alpha(game.doors, al_map_rgb(255,255,255));
    game.portal = al_load_bitmap("pics/portal.png");
    al_convert_mask_to_alpha(game.portal, al_map_rgb(0,0,0));
    game.enemies = al_load_bitmap("pics/enemies01.png");
    al_convert_mask_to_alpha(game.enemies, al_map_rgb(255,255,255));
    game.hellportal = al_load_bitmap("pics/hellportal.png");
    game.itempics = al_load_bitmap("pics/items.png");
    al_convert_mask_to_alpha(game.itempics, al_map_rgb(255,255,255));
    game.shotpics = al_load_bitmap("pics/shotpics.png");
    al_convert_mask_to_alpha(game.shotpics, al_map_rgb(255,255,255));
    //game.coinpic = al_create_sub_bitmap(game.pickups, 0,0,35,35);
    //game.carrotpic = al_create_sub_bitmap(game.pickups, 35,0,35,35);
    game.bombpic = al_create_sub_bitmap(game.pickups, 70,0,35,35);
    //game.keypic = al_create_sub_bitmap(game.pickups, 105,0,35,35);
    game.tile = al_create_sub_bitmap(game.pickups,140,0,56,20);
    game.dym = al_create_sub_bitmap(game.pickups,0,35,172,156);
    game.pause = al_load_bitmap("backgrounds/pause.png");
    game.portalstate = 0;
    //newgame(&game);
    intro(&game);
    double time = al_get_time();
    while(1)
    { //************MENU GLOWNE*********************
        if (game.state == 0 && game.menu.screen == 0 && al_get_time() > time + 0.1)
        {
            al_get_keyboard_state(&game.keyboard);
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_UP) && game.menu.choice > 0)
            {
                time = al_get_time();
                game.menu.choice--;
            }
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_DOWN) && game.menu.choice < 4)
            {
                time = al_get_time();
                game.menu.choice++;
            }
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_ENTER) && game.menu.choice == 4)
            {
                break;
            }
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_ENTER))
            {
                switch(game.menu.choice)
                {
                    case 0: game.state = 1; break;
                    case 1: game.menu.screen = 1;
                            al_destroy_bitmap(game.menu.picture);
                            game.menu.picture = al_load_bitmap("backgrounds/menu02.bmp"); break;
                    case 2: game.menu.screen = 2;
                            al_destroy_bitmap(game.menu.picture);
                            game.menu.picture = al_load_bitmap("backgrounds/menu03.bmp"); break;
                    case 3: game.menu.screen = 3;
                            al_destroy_bitmap(game.menu.picture);
                            game.menu.picture = al_load_bitmap("backgrounds/menu04.bmp"); break;
                }
                game.menu.choice = 0;
                time = al_get_time();
            }
            drawmenu(&game);
            al_flip_display();
            al_rest(0.001);
        }//**************OPTIONS*****************
        if (game.state == 0 && game.menu.screen == 1 && al_get_time() > time + 0.1)
        {
            al_get_keyboard_state(&game.keyboard);
            time = al_get_time();
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_LEFT) && game.menu.choice > 0) game.menu.choice--;
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_RIGHT) && game.menu.choice < 1) game.menu.choice++;
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_ESCAPE))
            {
                game.menu.screen = 0;
                game.menu.choice = 1;
                game.menu.picture = al_load_bitmap("backgrounds/menu01.bmp");
            }
        }//*************STATS******************
        if(game.state == 0 && game.menu.screen == 2 && al_get_time() > time + 0.1)
        {
            al_get_keyboard_state(&game.keyboard);
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_ESCAPE))
            {
                time = al_get_time();
                game.menu.screen = 0;
                game.menu.choice = 2;
                al_destroy_bitmap(game.menu.picture);
                game.menu.picture = al_load_bitmap("backgrounds/menu01.bmp");
            }
        }//*************ABOUT******************
        if(game.state == 0 && game.menu.screen == 3 && al_get_time() > time + 0.1)
        {
            al_get_keyboard_state(&game.keyboard);
            if (al_key_down(&game.keyboard, ALLEGRO_KEY_ESCAPE))
            {
                time = al_get_time();
                game.menu.screen = 0;
                game.menu.choice = 3;
                al_destroy_bitmap(game.menu.picture);
                game.menu.picture = al_load_bitmap("backgrounds/menu01.bmp");
            }
        }//************GRA******************
        if (game.state == 1)
        {
            newgame(&game);
            start(&game);
            al_rest(1);
            game.state = 0;
        }
    }//**************PORZADKI*************
    al_destroy_bitmap(game.menu.picture);
    al_destroy_bitmap(game.bunny.picture);
    al_destroy_display(game.display);
    return 0;
}
/* do poprawienia
1) wskakiwanie do portalu (spacjowy item)
3) gdzie pojawia sie secret
*/
