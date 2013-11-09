#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "main.h"

//**************BOMBY****************
void newbomb(Game *game, Room *room, int posx, int posy, int enemy, int exploded, double time)
{
    Bomb bomb;
    bomb.posx = posx;
    bomb.posy = posy;
    bomb.time = al_get_time() + time;
    bomb.state = 0;
    bomb.color = 0;
    bomb.killsenemy = enemy;
    bomb.exploded = exploded;
    room->bombs[room->bombcnt] = bomb;
    room->bombcnt++;
}

void drawbomb(Game *game, int i)
{
    game->room->bombs[i].state++;
    double time;
        time = game->room->bombs[i].time - al_get_time();
        if (time > 2)
        {
            if (game->room->bombs[i].state%32==0) game->room->bombs[i].color = 1 - game->room->bombs[i].color;
        }
        else if (time > 1)
        {
            if (game->room->bombs[i].state%16==0) game->room->bombs[i].color = 1-game->room->bombs[i].color;
        }
        else if (time > 0.3)
        {
            if (game->room->bombs[i].state%4==0) game->room->bombs[i].color = 1 - game->room->bombs[i].color;
        }
        if (game->room->bombs[i].color == 1 && time > 0.3)
            al_draw_tinted_bitmap(game->bombpic,al_map_rgb(255,0,0),game->room->bombs[i].posx,game->room->bombs[i].posy-35,0);
        else if (time > 0.3)
            al_draw_bitmap(game->bombpic,game->room->bombs[i].posx,game->room->bombs[i].posy-35,0);
}

void bomb_sort(Game *game)
{
    int i, j;
    Bomb tmp;
    for (j=1; j<game->room->bombcnt; j++)
    {
        i = j;
        while(game->room->bombs[i].posy < game->room->bombs[i-1].posy && i > 0)
        {
            tmp = game->room->bombs[i];
            game->room->bombs[i] = game->room->bombs[i-1];
            game->room->bombs[i-1] = tmp;
            i--;
        }
    }
}

void deletebomb(Game *game, Room *room, int i)
{
    while(i+1<room->bombcnt)
    {
        room->bombs[i] = room->bombs[i+1];
        i++;
    }
    room->bombcnt--;
}

void nobombs(Room *room)
{
    room->bombcnt = 0;
}

//**************ITEMY*******************
void newitem(Game *game, Room *room, int price, int number, int posx, int posy)
{
    Item item;
    item.price = price;
    item.number = number;
    item.posx = posx;
    item.posy = posy;
    item.taken = 0;
    item.frozen = 0;
    item.spaceleft = 0;
    room->items[room->itemcnt] = item;
    room->itemcnt++;
}

void drawitem(Game *game, int i)
{
    Item item;
    item = game->room->items[i];
    if (item.taken == 0)
    {
        al_draw_bitmap_region(game->itempics, item.number%10*45,item.number/10*45,45,45,item.posx,item.posy-45,0);
        if (item.price>0)
        {
            if (item.price>=10)
            {
                al_draw_bitmap_region(game->cyferki,(item.price/10)*25,0,25,35,item.posx+12,item.posy+10,0);
                al_draw_bitmap_region(game->cyferki,(item.price % 10)*25,0,25,35,item.posx+37,item.posy+10,0);
                al_draw_bitmap_region(game->cyferki,250,0,25,35,item.posx-13,item.posy+10,0);
            }
            else
            {
                al_draw_bitmap_region(game->cyferki,(item.price % 10)*25,0,25,35,item.posx+22,item.posy+10,0);
                al_draw_bitmap_region(game->cyferki,250,0,25,35,item.posx-3,item.posy+10,0);
            }
        }
    }
}

void deleteitem(Room *room, int i)
{
    while(i+1<room->itemcnt)
    {
        room->items[i] = room->items[i+1];
        i++;
    }
    room->itemcnt--;
}

void noitems(Room *room)
{
    room->itemcnt = 0;
}

void itemeffect(Game *game, int num, int i)
{
    double tmp;
    switch (num)
    {
        case 0: //rabbits heart
        {
            game->bunny.hearts++;
            game->bunny.hp+=2;
            break;
        }
        case 1: // coin bag
        {
            game->coins+=10;
            if (game->coins > 99)
                game->coins = 99;
            break;
        }
        case 2: // plonaca kapucha
        {
            game->bunny.shotpic = al_create_sub_bitmap(game->shotpics,60,0,20,20);
            game->bunny.range += 200;
            break;
        }
        case 3: // worek marchewek
        {
            if (game->spaceitem >= 0)
            {
                game->room->items[i].number = game->spaceitem;
                game->room->items[i].taken = 0;
                tmp = game->spaceleft;
                game->spaceleft = game->room->items[i].spaceleft;
                game->room->items[i].spaceleft = tmp;
            }
            game->spaceitem = 3;
            break;
        }
        case 4: // mystery worek
        {
            if (game->spaceitem >= 0)
            {
                game->room->items[i].number = game->spaceitem;
                game->room->items[i].taken = 0;
                tmp = game->spaceleft;
                game->spaceleft = game->room->items[i].spaceleft;
                game->room->items[i].spaceleft = tmp;
            }
            game->spaceitem = 4;
            break;
        }
        case 5: // candy rush
        {
            game->bunny.speed -= 0.001;
            game->bunny.kicspeed -= 0.02;
            break;
        }
        case 6: // bomb bag
        {
            game->bombs += 10;
            if (game->bombs > 99)
                game->bombs = 99;
            break;
        }
        case 7: // key bag
        {
            game->keys += 10;
            if (game->keys > 99)
                game->keys = 99;
            break;
        }
        case 8: //lasergun
        {
            game->bunny.shottype = 1;
            game->bunny.shotfreq = 0.6;
            game->bunny.range = 700;
            break;
        }
        case 9: //duracell
        {
            game->bunny.battery += 2;
            break;
        }
        case 10: // buciki
        {
            game->bunny.boots += 1;
            break;
        }
        //bonus items
        //pokemon level
        case 15: //lvlup candy
        {
            game->bunny.dmg += 10;
            if (game->bunny.dmg > 60)
                game->bunny.dmg = 60;
            game->bunny.shotfreq -= 0.1;
            if (game->bunny.shotfreq < 0.1)
                game->bunny.shotfreq = 0.1;
            game->bunny.range += 80;
            if (game->bunny.range > 700)
                game->bunny.range = 700;
            game->bunny.speed -= 0.001;
            if (game->bunny.speed < 0.004)
                game->bunny.speed = 0.004;
            game->bunny.hearts++;
            if (game->bunny.hearts > 6)
            {
                game->bunny.hearts = 6;
                game->bunny.hp = game->bunny.hearts*2;
            }
            break;
        }
        case 16: // pokeball
        {
            game->bunny.shotpic = al_create_sub_bitmap(game->shotpics,41,0,20,20);
            break;
        }
        case 17: // squirtle's glasses
        {
            if (game->spaceitem >= 0)
            {
                game->room->items[i].number = game->spaceitem;
                game->room->items[i].taken = 0;
                tmp = game->spaceleft;
                game->spaceleft = game->room->items[i].spaceleft;
                game->room->items[i].spaceleft = tmp;
            }
            game->spaceitem = 17;
            break;
        }
        case 18: // ash cap GOTTA CATCH EM ALL
        {
            newpickup(game, game->room, 0, rand()%665+50, rand()%415+100,0);
            newpickup(game, game->room, 1, rand()%665+50, rand()%415+100,0);
            newpickup(game, game->room, 2, rand()%665+50, rand()%415+100,0);
            newpickup(game, game->room, 3, rand()%665+50, rand()%415+100,0);
            break;
        }
        case 51: // niebieski portalgun
        {
            if (game->spaceitem >= 0)
            {
                game->room->items[i].number = game->spaceitem;
                game->room->items[i].taken = 0;
                tmp = game->spaceleft;
                game->spaceleft = game->room->items[i].spaceleft;
                game->room->items[i].spaceleft = tmp;
            }
            game->spaceitem = 3;
            break;
        }
        case 52: //pomaranczowy portal gun
        {
            if (game->spaceitem >= 0)
            {
                game->room->items[i].number = game->spaceitem;
                game->room->items[i].taken = 0;
                tmp = game->spaceleft;
                game->spaceleft = game->room->items[i].spaceleft;
                game->room->items[i].spaceleft = tmp;
            }
            game->spaceitem = 4;
            break;
        }
    }
}

void spaceeffect(Game *game, int num)
{
    int i;
    int portposx, portposy;
    switch(num)
    {
        case 3: // worek marchewek
        {
            if (game->bunny.hp < game->bunny.hearts*2)
            {
                game->bunny.hp += 2;
                if (game->bunny.hp > game->bunny.hearts*2)
                    game->bunny.hp = game->bunny.hearts*2;
                game->spaceleft = 4;
            }
            break;
        }
        case 4: //mystery worek
        {
            int random;
            random = rand()%3+1;
            if (random == 1) random = 0;
            newpickup(game, game->room, random, rand()%665+50, rand()%415+100,0);
            game->spaceleft = 2;
            break;
        }
        case 10: //portalgun niebieski do boss roomu
        {
            portposx = game->bunny.posx-5;
            portposy = game->bunny.posy+25;
            noshots(game);
            game->bunny.statey = 0;
            game->bunny.statex = 0;
            for (i=0; i<3; i++)
            {
                draw(game);
                al_draw_scaled_bitmap(game->portal,0,0,10,10,portposx,portposy,60,24,0);
                drawbunny(game);
                al_rest(0.1);
                game->bunny.statex++;
            }
            for (i=0; i<30; i++)
            {
                game->bunny.posy++;
                al_rest(0.005);
                draw(game);
                al_draw_scaled_bitmap(game->portal,0,95,242,95,portposx,portposy,60,24,0);
                drawbunny(game);
            }
            Room *tmp;
            tmp = game->firstroom;
            while(tmp->type != 3)
            {
                tmp = tmp->right;
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
            game->room = tmp;
            changeroom(game);
            game->bunny.posy=370;
            game->bunny.posx = 375;
            while(game->bunny.posy<400)
            {
                game->bunny.posy++;
                al_rest(0.001);
                draw(game);
            }
            game->spaceleft = 3;
            break;
        }
        case 17: // squirtle's glasses DEAL WITH IT
        {
            //al_draw_bitmap_region(game->pickups,);
            game->roomeffect[0] = 0.1 - game->bunny.shotfreq;
            game->bunny.shotfreq = 0.1;
            game->spaceleft = 3;
            break;
        }
    }
    if (game->bunny.battery > 0)
    {
        game->spaceleft -= game->bunny.battery;
        if (game->spaceleft < 1) game->spaceleft = 1;
    }
}

//***************PICKUPY*****************
void newpickup(Game *game, Room *room, int type, int posx, int posy, int price)
{
    Pickup pickup;
    pickup.type = type;
    pickup.posx = posx;
    pickup.posy = posy;
    pickup.posz = 40;
    pickup.price = price;
    room->pickups[room->pickupcnt] = pickup;
    room->pickupcnt++;
}

void drawpickup(Game *game, int i)
{
    Pickup pickup;
        pickup = game->room->pickups[i];
        al_draw_filled_ellipse(pickup.posx+17, pickup.posy, 17,5, al_map_rgba(0,0,0,50));
        al_draw_bitmap_region(game->pickups,pickup.type*35,0,35,35,pickup.posx,pickup.posy-35-pickup.posz,0);
        if (pickup.posz > 0) game->room->pickups[i].posz--;
        if (pickup.price>0)
        {
            al_draw_bitmap_region(game->cyferki,pickup.price*25,0,25,35,pickup.posx+17,pickup.posy+10,0);
            al_draw_bitmap_region(game->cyferki,250,0,25,35,pickup.posx-8,pickup.posy+10,0);
        }
}

void pickup_sort(Game *game)
{
    int i, j;
    Pickup tmp;
    for (j=1; j<game->room->pickupcnt; j++)
    {
        i = j;
        while(game->room->pickups[i].posy < game->room->pickups[i-1].posy && i > 0)
        {
            tmp = game->room->pickups[i];
            game->room->pickups[i] = game->room->pickups[i-1];
            game->room->pickups[i-1] = tmp;
            i--;
        }
    }
}

void deletepickup(Room *room, int i)
{
    while(i+1<room->pickupcnt)
    {
        room->pickups[i] = room->pickups[i+1];
        i++;
    }
    room->pickupcnt--;
}

void nopickups(Room *room)
{
    room->pickupcnt = 0;
}
