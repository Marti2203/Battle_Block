#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <windows.h>
#include "neshta.h"
#include <vector>
#include <queue>
#include <fstream>
using namespace std;
sf::RenderWindow window;
Player igrach(1*20,28*20);
vector<RenderedEntity*> Entities;
queue<Bullet> Bullets;
queue<Boom> Gurmene;
int end_x;
int end_y;
string jump="ok";
int how_much=0;
int cooldown=0;
int cooldown2=0;
int checkpoint_x=1*20;
int checkpoint_y=28*20;
bool if_hit()
{
    for(int i=0;i<Entities.size();i++)
    {
        if(igrach.x<Entities[i]->x+20 && igrach.x>Entities[i]->x-igrach.sizee && igrach.y<Entities[i]->y+20 && igrach.y>Entities[i]->y-igrach.sizee && Entities[i]->visited == 0)
        {
        	if(Entities[i]->type==EntityType::Waypoint_type)
        	{
        	Entities[i]->visited=1;
            	checkpoint_x=Entities[i]->x;
            	checkpoint_y=Entities[i]->y;	
        	}
		if(Entities[i]->type==EntityType::Bomb_type )
		{
		Boom k(Entities[i]->x,Entities[i]->y,300);
            	Gurmene.push(k);
            	return false;
		}
        }
        if(igrach.x<Entities[i]->x+Entities[i]->sizee && igrach.x>Entities[i]->x-igrach.sizee && igrach.y<Entities[i]->y+Entities[i]->sizee && igrach.y>Entities[i]->y-igrach.sizee && Entities[i]->type!=EntityType::Waypoint_type && Entities[i]->type!=EntityType::Bomb_type)
        {
            if(Entities[i]->type==EntityType::Hazard_type)
            {
                igrach.x=checkpoint_x;
                igrach.y=checkpoint_y;
            }
            return false;
        }

    }
    if(igrach.x<end_x+20 && igrach.x>end_x-igrach.sizee && igrach.y<end_y+20 && igrach.y>end_y-igrach.sizee)
    {
        window.close();
    }

    return true;
}
bool playerCheck(int x,int y)
{
    if(igrach.x<x+20 && igrach.x>x-igrach.sizee && igrach.y<y+20 && igrach.y>y-igrach.sizee)
    {
        return false;
    }
    return true;
}
bool if_hit(int x,int y,EntityType type,int g)
{
    for(int i=0;i<Entities.size();i++)
    {
        if(Entities[i]->type==EntityType::Waypoint_type) continue;
        if(Entities[i]->type==EntityType::Bomb_type && Entities[i]->visited==1){continue;}
        if(x<Entities[i]->x+20 && x>Entities[i]->x-20 && y<Entities[i]->y+20 && y>Entities[i]->y-20 && Entities[i]->type!=type)
        {
            return false;
        }
        if(x<Entities[i]->x+19 && x>Entities[i]->x-19 && y<Entities[i]->y+19 && y>Entities[i]->y-19 && Entities[i]->type==type && i!=g)
        {
            return false;
        }
    }
    return playerCheck(x,y);
}
void draw_board()
{
	sf::RectangleShape square(sf::Vector2f(20,20));
    square.setFillColor(sf::Color(123,6,9));
    square.setPosition(end_x,end_y);
    window.draw(square);
    square.setSize(sf::Vector2f(10,10));
    square.setFillColor(sf::Color::Blue);
    square.setPosition(igrach.x,igrach.y);
    window.draw(square);
}
string choose_file()
{
    cout<<"Which saved file you want to play?"<<endl;
    cout<<"Enter:";
    string k;
    cin>>k;
    return k;
}
void initialize()
{
	string line=choose_file();
    ifstream kray;
    kray.open(line,ios::in);
    while(getline(kray,line))
    {
        int masiv[10]={0};
        masiv[3]=1;
        get_number(line,masiv);
        if(masiv[2]==4)
        {
            Entities.push_back(new Turret_x(masiv[0]*20,masiv[1]*20,masiv[3]));
            Entities.back()->type=Turret_x_type;
        }
        if(masiv[2]==3)
        {
            Entities.push_back(new Up_Down(masiv[0]*20,masiv[1]*20,masiv[3]));
            Entities.back()->type=Up_Down_type;
        }
        if(masiv[2]==2)
        {
            Entities.push_back(new Left_Right(masiv[0]*20,masiv[1]*20,masiv[3]));
            Entities.back()->type=Left_Right_type;
        }
        if(masiv[2]==1)
        {
            Entities.push_back(new Mapi(masiv[0]*20,masiv[1]*20));
            Entities.back()->type=Mapi_type;
        }
        if(masiv[2]==6)
        {
            Entities.push_back(new Hazard(masiv[0]*20,masiv[1]*20));
            Entities.back()->type=Hazard_type;
        }
        if(masiv[2]==5)
        {
            Entities.push_back(new Turret_y(masiv[0]*20,masiv[1]*20,masiv[3]));
            Entities.back()->type=Turret_y_type;
        }
        if(masiv[2]==7)
        {
            igrach.x=masiv[0]*20;
            igrach.y=masiv[1]*20;
            checkpoint_x=igrach.x;
            checkpoint_y=igrach.y;
        }
        if(masiv[2]==8)
        {
            end_x=masiv[0]*20;
            end_y=masiv[1]*20;
        }
        if(masiv[2]==9)
        {
            Entities.push_back(new Waypoint(masiv[0]*20,masiv[1]*20));
            Entities.back()->type=Waypoint_type;
        }
        if(masiv[2]==10)
        {
            Entities.push_back(new Bomb(masiv[0]*20,masiv[1]*20));
            Entities.back()->type=Bomb_type;
        }
    }
    kray.close();
}
void player_command()
{
    if(igrach.life<=0)
    {
        igrach.x=checkpoint_x;
        igrach.y=checkpoint_y;
        igrach.life=300;
    }
    int init_x=igrach.x;
    int init_y=igrach.y;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if(jump=="no" && cooldown==0 && cooldown2==0)
        {
            how_much+=70;
            jump="no2";
        }
        if(jump=="ok" && cooldown==0)
        {
            how_much+=60;
            cooldown2=130;
            jump="no";
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        igrach.x-=2;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        igrach.x+=2;
    }
    while(!if_hit())
    {
        int k=0;
        int k2=0;
        if(igrach.x-init_x>0){k=1;}
        if(igrach.x-init_x<0){k=-1;}
        if(igrach.y-init_y>0){k2=1;}
        if(igrach.y-init_y<0){k2=-1;}
        igrach.x=igrach.x+(-1)*(k);
        igrach.y=igrach.y+(-1)*(k2);
    }
}
void gravity()
{
    int init_x=igrach.x;
    int init_y=igrach.y;
    igrach.y+=3;
    while(!if_hit())
    {
        int k=0;
        int k2=0;
        if(igrach.x-init_x>0){k=1;}
        if(igrach.x-init_x<0){k=-1;}
        if(igrach.y-init_y>0){k2=1;}
        if(igrach.y-init_y<0){k2=-2;}
        igrach.x=igrach.x+(-1)*(k);
        igrach.y=igrach.y+(-1)*(k2);
        if(jump=="no" || jump=="no2")
        {
            cooldown=150;
        }
        jump="ok";
        how_much=0;
    }
}
void jumping()
{
    int init_x=igrach.x;
    int init_y=igrach.y;
    if(jump=="no" && how_much>0)
    {
        how_much-=5;
        igrach.y-=6;
    }
    if(jump=="no2" && how_much>0)
    {
        how_much-=5;
        igrach.y-=5;
    }
    while(!if_hit())
    {
        int k=0;
        int k2=0;
        if(igrach.x-init_x>0){k=1;}
        if(igrach.x-init_x<0){k=-1;}
        if(igrach.y-init_y>0){k2=1;}
        if(igrach.y-init_y<0){k2=-2;}
        igrach.x=igrach.x+(-1)*(k);
        igrach.y=igrach.y+(-1)*(k2);
        if(jump=="no" || jump=="no2")
        {
            cooldown=150;
        }
        how_much=0;
    }
}
void mrudni()
{
    vector<RenderedEntity*>Platforms=GetOfType(Entities,EntityType::Up_Down_type);
    vector<RenderedEntity*>k=GetOfType(Entities,EntityType::Left_Right_type);
    for(int i=0;i<k.size();i++)
    {
        Platforms.push_back(k[i]);
    }
    for(int i=0;i<Platforms.size();i++)
    {
        if(igrach.x<Platforms[i]->x+20 && igrach.x>Platforms[i]->x-igrach.sizee && igrach.y<Platforms[i]->y && igrach.y>Platforms[i]->y-igrach.sizee-1)
         {
            int init_x=igrach.x;
            int init_y=igrach.y;
            igrach.y+=Platforms[i]->nakude_y;
            igrach.x+=Platforms[i]->nakude_x;
            while(!if_hit())
            {
                int k=0;
                int k2=0;
                if(igrach.x-init_x>0){k=1;}
                if(igrach.x-init_x<0){k=-1;}
                if(igrach.y-init_y>0){k2=1;}
                if(igrach.y-init_y<0){k2=-2;}
                igrach.x=igrach.x+(-1)*(k);
                igrach.y=igrach.y+(-1)*(k2);
            }
        }
        int g=Get_Position(Entities,Platforms[i]->type,Platforms[i]->x,Platforms[i]->y);
        Platforms[i]->y+=Platforms[i]->nakude_y;
        Platforms[i]->x+=Platforms[i]->nakude_x;
        while(!if_hit(Platforms[i]->x,Platforms[i]->y,Platforms[i]->type,g))
        {
            Platforms[i]->y-=Platforms[i]->nakude_y;
            Platforms[i]->nakude_y*=-1;
            Platforms[i]->x-=Platforms[i]->nakude_x;
            Platforms[i]->nakude_x*=-1;
        }
    }
}
bool if_hit_bullets(int x,int y,int k,int k2,int sizee)
{
    for(int i=0;i<(int)Entities.size();i++)
    {
        if(Entities[i]->type==EntityType::Waypoint_type) continue;
        if(x<Entities[i]->x+20 && x>Entities[i]->x-sizee && y<Entities[i]->y+20 && y>Entities[i]->y-sizee)
        {
            if(Entities[i]->type!=EntityType::Turret_x_type && Entities[i]->type!=EntityType::Turret_y_type){return false;}
            if(Entities[i]->type==EntityType::Turret_x_type && k!=i){return false;}
            if(Entities[i]->type==EntityType::Turret_y_type && k!=i){return false;}
        }
    }
    if(igrach.x<x+sizee && igrach.x>x-igrach.sizee && igrach.y<y+sizee && igrach.y>y-igrach.sizee)
    {
        igrach.life-=50;
        return false;
    }
    return true;
}
void shooting()
{
    vector<RenderedEntity*> Turrets_x=GetOfType(Entities,EntityType::Turret_x_type);
    for(int i=0;i<(int)Turrets_x.size();i++)
    {
        if(Turrets_x[i]->cooldown==0)
        {
            Bullet k(Turrets_x[i]->x+8+Turrets_x[i]->nakude*10,Turrets_x[i]->y+10,Turrets_x[i]->nakude*2,0,Get_Position(Entities,Turret_x_type,Turrets_x[i]->x,Turrets_x[i]->y),-1);
            Bullets.push(k);
            Turrets_x[i]->cooldown=Turrets_x[i]->kolko_da_e_cooldowna;
            Turrets_x[i]->nakude*=-1;
        }
    }
    vector<RenderedEntity*> Turrets_y=GetOfType(Entities,EntityType::Turret_y_type);
    for(int i=0;i<(int)Turrets_y.size();i++)
    {
        if(Turrets_y[i]->cooldown==0)
        {
            Bullet k(Turrets_y[i]->x+8,Turrets_y[i]->y+8+Turrets_y[i]->nakude*10,0,Turrets_y[i]->nakude*2,-1,Get_Position(Entities,Turret_y_type,Turrets_y[i]->x,Turrets_y[i]->y));
            Bullets.push(k);
            Turrets_y[i]->cooldown=Turrets_y[i]->kolko_da_e_cooldowna;
            Turrets_y[i]->nakude*=-1;
        }
    }
    int kolko=Bullets.size();
    for(int i=0;i<kolko;i++)
    {
        Bullet k=Bullets.front();
        Bullets.pop();
        k.x+=k.nakude_x;
        k.y+=k.nakude_y;
        if(if_hit_bullets(k.x,k.y,k.ot_koy,k.ot_koy2,k.sizee))
        {
            Bullets.push(k);
        }
    }
}
void updates()
{
    for(int i=0;i<Entities.size();i++)
    {
        Entities[i]->Update(window);
    }
    for(int i=0;i<Bullets.size();i++)
    {
        Bullet k=Bullets.front();
        Bullets.pop();
        k.Update(window);
        Bullets.push(k);
    }
}
void bombing()
{
    int kolko=Gurmene.size();
    vector<RenderedEntity*> Bombi=GetOfType(Entities,Bomb_type);
    for(int i=0;i<kolko;i++)
    {
        Boom k=Gurmene.front();
        Gurmene.pop();
        if(k.fuse==0)
        {
            for(int i=0;i<Bombi.size();i++)
            {
                if(Bombi[i]->x==k.x && Bombi[i]->y==k.y && Bombi[i]->visited==0)
                {
                    Bombi[i]->visited=1;
                    Bombi[i]->cooldown=Bombi[i]->kolko_da_e_cooldowna;
                    Boom k(Bombi[i]->x,Bombi[i]->y+20,300);
                    Gurmene.push(k);
                    Boom k2(Bombi[i]->x-20,Bombi[i]->y,300);
                    Gurmene.push(k2);
                    Boom k3(Bombi[i]->x,Bombi[i]->y-20,300);
                    Gurmene.push(k3);
                    Boom k4(Bombi[i]->x+20,Bombi[i]->y,300);
                    Gurmene.push(k4);
                }
            }
        }
        else
        {
            k.fuse-=10;
            Gurmene.push(k);
        }
    }
    for(int i=0;i<Bombi.size();i++)
    {
        if(Bombi[i]->cooldown==0)
        {
            if(if_hit(Bombi[i]->x,Bombi[i]->y,EntityType::Bomb_type,Get_Position(Entities,EntityType::Bomb_type,Bombi[i]->x,Bombi[i]->y)))
            {
                Bombi[i]->visited=0;
            }
        }
    }
}
int main()
{
    initialize();
    window.close();
    window.create(sf::VideoMode(1000,600),"BattleBlock");
    sf::View view;
    view.setCenter(sf::Vector2f(350, 300));
    view.setSize(sf::Vector2f(250, 150));
    while (window.isOpen())
    {
        view.setCenter(sf::Vector2f(igrach.x, igrach.y));
        window.setView(view);
        sf::Event event;
            while (window.pollEvent(event))
           {
               if (event.type == sf::Event::Closed)
                   window.close();
           }
        window.clear();
        updates();
        draw_board();
        mrudni();
        bombing();
        shooting();
        gravity();
        player_command();
        jumping();
        window.display();
        Sleep(10);
        if(cooldown>0)
        {
            cooldown-=10;
        }
        if(cooldown2>0)
        {
            cooldown2-=10;
        }
    }
    return 0;
}
