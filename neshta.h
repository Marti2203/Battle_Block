#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

enum EntityType
{
    Entity_type,RenderedEntity_type,Waypoint_type,
    Bullet_type,Turret_type,Turret_x_type,
    Turret_y_type,Platform_type,Up_Down_type,
    Left_Right_type,Mapi_type,Hazard_type,
    Player_type,Bomb_type,Boom_Type
};

struct Entity
{
    int x;
    int y;
    int sizee;
    EntityType type=EntityType::Entity_type;
};

struct RenderedEntity:Entity
{
    int visited = 0;
    int nakude;
    int nakude_x=0;
    int nakude_y=0;
    int cooldown;
    int kolko_da_e_cooldowna=700;
    EntityType type;
    virtual void Update(sf::RenderWindow& window)=0;
    sf::RectangleShape square;
    ~RenderedEntity(){};
};
struct Platform:RenderedEntity
{
    void Update(sf::RenderWindow& window)
    {

    };
    ~Platform(){};
};

struct Waypoint:RenderedEntity
{
    EntityType type=EntityType::Waypoint_type;
    Waypoint(int a,int b)
    {
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color(45,153,23));
        square.setPosition(a,b);
        x=a;
        y=b;
        sizee=20;
    }
    virtual void Update(sf::RenderWindow& window)
    {
        if(!visited)
        {
            window.draw(square);
        }
    }
};

struct Bullet
{
    int x;
    int y;
    sf::RectangleShape square;
    int nakude_x;
    int nakude_y;
    EntityType type=EntityType::Bullet_type;
    int ot_koy;
    int ot_koy2;
    int sizee=3;
    Bullet(int a, int b, int c, int d,int e,int f)
    {
        square.setSize(sf::Vector2f(3,3));
        square.setFillColor(sf::Color::Red);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude_x=c;
        nakude_y=d;
        ot_koy=e;
        ot_koy2=f;
    }
    virtual void Update(sf::RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Turret:RenderedEntity
{
    EntityType type=EntityType::Turret_type;
    void Update(sf::RenderWindow& window){};
    ~Turret(){};
};

struct Turret_x:Turret
{
    EntityType type=EntityType::Turret_x_type;
    Turret_x(int a,int b,int c)
    {
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color::White);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude=1;
        sizee=20;
    }
    void Update(sf::RenderWindow& window)
    {
        if(cooldown>0)
        {
           cooldown-=10;
        }
        window.draw(square);
    }
};

struct Turret_y:Turret
{
    EntityType type=EntityType::Turret_y_type;
    Turret_y(int a,int b,int c)
    {
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color::Green);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude=1;
        sizee=20;
    }
    void Update(sf::RenderWindow& window)
    {
        if(cooldown>0)
        {
           cooldown-=10;
        }
        window.draw(square);
    }
};

struct Up_Down:Platform
{
    EntityType type=EntityType::Up_Down_type;
    Up_Down(int a , int b , int c)
    {
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color::Blue);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude_y=c;
        sizee=20;
    }
    void Update(sf::RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Left_Right:Platform
{
    EntityType type=EntityType::Left_Right_type;
    Left_Right(int a , int b , int c)
    {
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color::Yellow);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude_x=c;
        sizee=20;
    }
    void Update(sf::RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Mapi:RenderedEntity
{
    EntityType type=EntityType::Mapi_type;
    Mapi(int a , int b)
    {
        type=EntityType::Mapi_type;
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color::Red);
        square.setPosition(a,b);
        x=a;
        y=b;
        sizee=20;
    }
    void Update(sf::RenderWindow& window)
    {
        window.draw(square);
    }
};

struct Hazard:RenderedEntity
{
    EntityType type=EntityType::Hazard_type;
    Hazard(int a , int b)
    {
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color(0,128,0));
        square.setPosition(x,y);
        x=a;
        y=b;
        sizee=15;
    }
    void Update(sf::RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Bomb:RenderedEntity
{
    EntityType type=EntityType::Bomb_type;
    Bomb(int a , int b)
    {
        square.setSize(sf::Vector2f(20,20));
        square.setFillColor(sf::Color(102,0,204));
        square.setPosition(x,y);
        x=a;
        y=b;
        sizee=20;
        cooldown=0;
        kolko_da_e_cooldowna=1000;
        visited=0;
    }
    void Update(sf::RenderWindow& window)
    {
        if(visited==0){
        square.setPosition(x,y);
        window.draw(square);
        }
        if(cooldown>0)
        {
            cooldown-=10;
        }
    }
};

struct Player: Entity
{
    EntityType type=EntityType::Player_type;
    int sizee=10;
    int life=300;
    Player(int a,int b)
    {
        x=a;
        y=b;
    }
};

struct Boom:Entity
{
    EntityType type=EntityType::Boom_type;
    int fuse;
    Boom(int a,int b ,int c)
    {
        x=a;
        y=b;
        fuse=c;
    }
};

void get_number(string line , int masiv[])
{
    int space=0;
    for(int i=0;i<(int)line.length();i++)
    {
        if(line.at(i)==' ')
        {
            space++;
            continue;
        }
        masiv[space]=masiv[space]*10+line.at(i)-'0';
    }
}

vector<RenderedEntity*> GetOfType(vector<RenderedEntity*> entities,EntityType type)
{
    vector<RenderedEntity*> result;
    for (int i=0;i<entities.size();i++)
    {
        if(entities[i]->type==type){
            result.push_back(entities[i]);
        }
    }
    return result;
}

int Get_Position(vector<RenderedEntity*> entities,EntityType type,int x,int y)
{
    for (int i=0;i<entities.size();i++)
    {
        if(entities[i]->type==type && entities[i]->x==x && entities[i]->y==y){
            return i;
        }
    }
    return 0;
}
