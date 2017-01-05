#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <windows.h>
#include <string>
using namespace std;
using namespace sf;

struct RectangleWrapper : public RectangleShape {
	RectangleWrapper(Vector2f& vector) :RectangleShape(vector) {

	}
	RectangleWrapper() :RectangleShape(Vector2f(0, 0)) {

	}
	void setPosition(float x, float y) {
		RectangleShape::setPosition(x * 20, y * 20);
	}
};

enum EntityType
{
    Entity_type,RenderedEntity_type,Waypoint_type,
    Bullet_type,Turret_type,Turret_x_type,
    Turret_y_type,Platform_type,Up_Down_type,
    Left_Right_type,Mapi_type,Hazard_type,
    Player_type,Bomb_type
};

struct Entity
{
    float x;
	float y;
	int sizee;

	int getX() {
		return x / 20;
	}
	int getY() {
		return y / 20;
	}
	void setX(float x) {
		this->x = x * 20;
	}
	void setY(float y) {
		this->y = y * 20;
	}
    EntityType type=EntityType::Entity_type;
};

struct RenderedEntity:Entity
{
    int visited = 0;
    int nakude;
    int nakude_x=0,nakude_y=0;
    int cooldown;
    int fuse=-10;
    int kolko_da_e_cooldowna=700;
    virtual void Update(RenderWindow& window)=0;
    RectangleWrapper square;
	RenderedEntity() {
		type = EntityType::RenderedEntity_type;
	}
    ~RenderedEntity(){};
};
struct Platform:RenderedEntity
{
	Platform() {
		type = EntityType::Platform_type;
	}
    void Update(RenderWindow& window)
    {

    };
    ~Platform(){};
};

struct Waypoint:RenderedEntity
{
    Waypoint(float a,float b)
    {
		type = EntityType::Waypoint_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color(45,153,23));
        square.setPosition(a,b);
        x=a;
        y=b;
        sizee=20;
    }
    virtual void Update(RenderWindow& window)
    {
        if(visited==0)
        {
            window.draw(square);
        }
    }
};

struct Bullet:RenderedEntity
{
    int nakude_x;
    int nakude_y;
    int ot_koy;
    int ot_koy2;
    int sizee=3;
	Bullet(){}
    Bullet(float a, float b, int c, int d,int e,int f)
    {
		type = EntityType::Bullet_type;
        square.setSize(Vector2f(3,3));
        square.setFillColor(Color::Red);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude_x=c;
        nakude_y=d;
        ot_koy=e;
        ot_koy2=f;
    }
    virtual void Update(RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Turret:RenderedEntity
{
	Turret() { type = EntityType::Turret_type; }
    void Update(RenderWindow& window){};
    ~Turret(){};
};

struct Turret_x:Turret
{
    Turret_x(float a,float b)
    {
		type = EntityType::Turret_x_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color::White);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude=1;
        sizee=20;
    }
    void Update(RenderWindow& window)
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
    Turret_y(float a,float b)
	{
		type = EntityType::Turret_y_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color::Green);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude=1;
        sizee=20;
    }
    void Update(RenderWindow& window)
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
    Up_Down(float a , float b , int c)
    {
		type = EntityType::Up_Down_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color::Blue);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude_y=c;
        sizee=20;
    }
    void Update(RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Left_Right:Platform
{
    Left_Right(float a , float b , int c)
    {
		type = EntityType::Left_Right_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color::Yellow);
        square.setPosition(a,b);
        x=a;
        y=b;
        nakude_x=c;
        sizee=20;
    }
    void Update(RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Mapi:RenderedEntity
{
    Mapi(float a , float b)
    {
        type=EntityType::Mapi_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color::Red);
        square.setPosition(a,b);
        x=a;
        y=b;
        sizee=20;
    }
    void Update(RenderWindow& window)
    {
        window.draw(square);
    }
};

struct Hazard:RenderedEntity
{
    Hazard(float a, float b)
    {
		type = EntityType::Hazard_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color(0,128,0));
        square.setPosition(x,y);
        x=a;
        y=b;
        sizee=15;
    }
    void Update(RenderWindow& window)
    {
        square.setPosition(x,y);
        window.draw(square);
    }
};

struct Bomb:RenderedEntity
{
    Bomb(float a, float b)
    {
		type = EntityType::Bomb_type;
        square.setSize(Vector2f(20,20));
        square.setFillColor(Color(102,0,204));
        square.setPosition(x,y);
        x=a;
        y=b;
        sizee=20;
        cooldown=0;
        kolko_da_e_cooldowna=1000;
        visited=0;
    }
    void Update(RenderWindow& window)
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
    int sizee=10;
    int life=300;
    Player(float a,float b)
    {
		type = EntityType::Player_type;
        x=a;
        y=b;
    }
};

struct Imame
{
    bool Turret_x_ima=false;
    bool Turret_y_ima=false;
    bool Left_Right_ima=false;
    bool Up_Down_ima=false;
};


void get_number(string& line , int* masiv)
{
	traverseline(0, line, 0, masiv);
}

void traverseline(int start,string& line,int space,int* masiv) {
	if (start == line.length()) return;
	if (line.at(start) == ' ')
	{
		space++;
	}
	masiv[space] = masiv[space] * 10 + line.at(start) - '0';
	traverseline(start + 1,line, space, masiv);
}

vector<RenderedEntity*> GetOfType(vector<RenderedEntity*> entities,EntityType type)
{
	return getoftype(entities, type, 0, vector<RenderedEntity*>());
}

vector<RenderedEntity*> getoftype(vector<RenderedEntity*>& list,EntityType type,int index,vector<RenderedEntity*>& result) {
	if (index == list.size()) return result;
	if (list[index]->type == type)
	{
		result.push_back(list[index]);
	}
	return getoftype(list, type, index + 1, result);
}

int Get_Position(vector<RenderedEntity*>& entities,EntityType type,int x,int y)
{
	return getposition(entities, type, x, y, 0);
}

int getposition(vector<RenderedEntity*>& entities, EntityType type, int x, int y, int index) {
	if (index == entities.size()) return 0;
	if (entities[index]->type == type && entities[index]->x == x && entities[index]->y == y) {
		return index;
	}
	return (entities, type, x, y, index + 1);
}

Color DrawColors[] = { 
	Color::Red,
	Color::Yellow,
	Color::Blue,
	Color::White,
	Color::Green,
	Color(0u, 128u, 0u),
	Color(33u, 133u, 156u),
	Color(123u, 6u, 9u),
	Color(45u, 153u, 23u),
	Color(102u, 0u, 204u),
	Color(102u, 0u, 204u)
};
