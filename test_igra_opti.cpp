#include "neshta.h"

using namespace std;
using namespace sf;
RenderWindow window;
Player igrach(1*20,28*20);
Imame imam;
vector<RenderedEntity*> Entities, Bombi;
queue<Bullet> Bullets;
int Bomb_map[50][30];
int end_x,end_y;
string jump="ok";
int how_much = 0, cooldown = 0, cooldown2 = 0;
int checkpoint_x = 1 * 20, checkpoint_y=28*20;


bool if_hit()
{
	if (!hit_for(0, Entities.size()))
		return false;
	
    if(igrach.x<end_x+20 && igrach.x>end_x-igrach.sizee && igrach.y<end_y+20 && igrach.y>end_y-igrach.sizee)
    {
        window.close();
    }

    return true;
}
bool hit_for (int start, int end) {
	if (start == end) return true;
	if (!do_hitcheck(start))
		return false;
	return hit_for(start + 1, end);
}
bool do_hitcheck(int i) {
	if (igrach.x<Entities[i]->x + 20 && igrach.x>Entities[i]->x - igrach.sizee && igrach.y<Entities[i]->y + 20 && igrach.y>Entities[i]->y - igrach.sizee && Entities[i]->type == EntityType::Waypoint_type && Entities[i]->visited == 0)
	{
		Entities[i]->visited = 1;
		checkpoint_x = Entities[i]->x;
		checkpoint_y = Entities[i]->y;
	}
	if (igrach.x<Entities[i]->x + 20 && igrach.x>Entities[i]->x - igrach.sizee && igrach.y<Entities[i]->y + 20 && igrach.y>Entities[i]->y - igrach.sizee && Entities[i]->type == EntityType::Bomb_type && Entities[i]->visited == 0)
	{
		if (Entities[i]->fuse == -10)
			Entities[i]->fuse = 200;
		return false;
	}
	if (igrach.x<Entities[i]->x + Entities[i]->sizee && igrach.x>Entities[i]->x - igrach.sizee && igrach.y<Entities[i]->y + Entities[i]->sizee && igrach.y>Entities[i]->y - igrach.sizee && Entities[i]->type != EntityType::Waypoint_type && Entities[i]->type != EntityType::Bomb_type)
	{
		if (Entities[i]->type == EntityType::Hazard_type)
		{
			igrach.x = checkpoint_x;
			igrach.y = checkpoint_y;
		}
		return false;
	}
	return false;
}

bool playerCheck(int x,int y)
{
    if(igrach.x<x+20 && igrach.x>x-igrach.sizee && igrach.y<y+20 && igrach.y>y-igrach.sizee)
    {
        return false;
    }
    return true;
}

bool if_hitfor(int start, int end, int x, int y, int g, EntityType type) {
	if (start == end) return playerCheck(x, y);
	if (!if_hitcheck(start, y, x, g, type)) return false;
	return if_hitfor(start + 1, end, x, y, g, type);
}
bool if_hitcheck(int i,int y, int x,int g,EntityType type) {
	if (Entities[i]->type == EntityType::Waypoint_type) return true;
	if (Entities[i]->type == EntityType::Bomb_type && Entities[i]->visited == 1) return true;
	if (x<Entities[i]->x + 20 && x>Entities[i]->x - 20 && y<Entities[i]->y + 20 && y>Entities[i]->y - 20 && Entities[i]->type != type)
	{
		return false;
	}
	if (x<Entities[i]->x + 19 && x>Entities[i]->x - 19 && y<Entities[i]->y + 19 && y>Entities[i]->y - 19 && Entities[i]->type == type && i != g)
	{
		return false;
	}
}

void draw_board()
{
	RectangleShape square(Vector2f(20,20));
    square.setFillColor(Color(123,6,9));
    square.setPosition(end_x,end_y);
    window.draw(square);
    square.setSize(Vector2f(10,10));
    square.setFillColor(Color::Blue);
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
    int br=0;
	useline(kray, line, br);
    kray.close();
}
void useline(ifstream& kray, string& line,int& br) {
	if (!getline(kray, line)) return;
	initialiseswitch(line, br);
	br++;
	return useline(kray, line, br);
}
void initialiseswitch(string& line,int& br) {
	int masiv[10] = { 0 };
	masiv[3] = 1;
	get_number(line, masiv);
	switch (masiv[2]) {
	case 1:
		Entities.push_back(new Mapi(masiv[0], masiv[1]));
		break;
	case 2:
		Entities.push_back(new Left_Right(masiv[0], masiv[1], masiv[3]));
		imam.Left_Right_ima = true;
		break;
	case 3:
		Entities.push_back(new Up_Down(masiv[0], masiv[1], masiv[3]));
		imam.Up_Down_ima = true;
		break;
	case 4:
		Entities.push_back(new Turret_x(masiv[0], masiv[1]));
		imam.Turret_x_ima = true;
		break;
	case 5:
		Entities.push_back(new Turret_y(masiv[0], masiv[1]));
		imam.Turret_y_ima = true;
		break;
	case 6:
		Entities.push_back(new Hazard(masiv[0], masiv[1]));
		break;
	case 7:
		igrach.setX(masiv[0]);
		igrach.setY(masiv[1]);
		checkpoint_x = masiv[0];
		checkpoint_y = masiv[1];
		break;
	case 8:
		end_x = masiv[0] * 20;
		end_y = masiv[1] * 20;
		break;
	case 9:
		Entities.push_back(new Waypoint(masiv[0], masiv[1]));
		break;
	case 10:
		Entities.push_back(new Bomb(masiv[0], masiv[1]));
		Bomb_map[masiv[0]][masiv[1]] = br;
		break;
	}
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
    if (Keyboard::isKeyPressed(Keyboard::W))
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
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        igrach.x-=2;
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        igrach.x+=2;
    }
	playercheckhit(init_x, init_y);
}
void playercheckhit(int& init_x,int& init_y) {
	if (if_hit()) return;
	playernothit(init_x, init_y);
	return playercheckhit(init_x, init_y);
}
void playernothit(int& init_x,int& init_y) {
	int k = 0;
	int k2 = 0;
	if (igrach.x - init_x>0) { k = 1; }
	if (igrach.x - init_x<0) { k = -1; }
	if (igrach.y - init_y>0) { k2 = 1; }
	if (igrach.y - init_y<0) { k2 = -1; }
	igrach.x = igrach.x + (-1)*(k);
	igrach.y = igrach.y + (-1)*(k2);
}

void gravity()
{
    int init_x=igrach.x;
    int init_y=igrach.y;
    igrach.y+=3;
	gravitycheckhit(init_x, init_y);
}
void gravitycheckhit(int& init_x, int& init_y) {
	if (if_hit()) return;
	gravitynothit(init_x, init_y);
	return gravitycheckhit(init_x, init_y);
}
void gravitynothit(int& init_x,int& init_y) {
	int k = 0;
	int k2 = 0;
	if (igrach.x - init_x>0) { k = 1; }
	if (igrach.x - init_x<0) { k = -1; }
	if (igrach.y - init_y>0) { k2 = 1; }
	if (igrach.y - init_y<0) { k2 = -2; }
	igrach.x = igrach.x + (-1)*(k);
	igrach.y = igrach.y + (-1)*(k2);
	if (jump == "no" || jump == "no2")
	{
		cooldown = 150;
	}
	jump = "ok";
	how_much = 0;
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
	jumpcheckhit(init_x,init_y);
}
void jumpcheckhit(int& init_x,int& init_y) {
	if (if_hit()) return;
	jumpnothit(init_x, init_y);
	return jumpcheckhit(init_x, init_y);
}
void jumpnothit(int& init_x,int& init_y) {
	int k = 0;
	int k2 = 0;
	if (igrach.x - init_x>0) { k = 1; }
	if (igrach.x - init_x<0) { k = -1; }
	if (igrach.y - init_y>0) { k2 = 1; }
	if (igrach.y - init_y<0) { k2 = -2; }
	igrach.x = igrach.x + (-1)*(k);
	igrach.y = igrach.y + (-1)*(k2);
	if (jump == "no" || jump == "no2")
	{
		cooldown = 150;
	}
	how_much = 0;
}

void mrudni()
{
    vector<RenderedEntity*>Platforms;
    if(imam.Up_Down_ima==true) {Platforms=GetOfType(Entities,EntityType::Up_Down_type);}
    vector<RenderedEntity*>k;
    if(imam.Left_Right_ima==true) {k=GetOfType(Entities,EntityType::Left_Right_type);}
	insertplatforms(k, Platforms, 0);
	platformsloopcheck(0, Platforms);
}
void platformsloopcheck(int i,vector<RenderedEntity*>& Platforms) {
	if (i == Platforms.size()) return;
	if (igrach.x<Platforms[i]->x + 20 && igrach.x>Platforms[i]->x - igrach.sizee 
		&& igrach.y<Platforms[i]->y && igrach.y>Platforms[i]->y - igrach.sizee - 1)
	{
		int init_x = igrach.x;
		int init_y = igrach.y;
		igrach.y += Platforms[i]->nakude_y;
		igrach.x += Platforms[i]->nakude_x;
		platformcheckhit(init_x, init_y);
	}
	int g = Get_Position(Entities, Platforms[i]->type, Platforms[i]->x, Platforms[i]->y);
	Platforms[i]->y += Platforms[i]->nakude_y;
	Platforms[i]->x += Platforms[i]->nakude_x;
	platformhit(Platforms, i,g);
	return platformsloopcheck(i + 1, Platforms);
}
void platformhit(vector<RenderedEntity*>& Platforms,int& i,int& g) {
	if (if_hitfor(0, Entities.size(), Platforms[i]->x, Platforms[i]->y, g, Platforms[i]->type))return;
	Platforms[i]->y -= Platforms[i]->nakude_y;
	Platforms[i]->nakude_y *= -1;
	Platforms[i]->x -= Platforms[i]->nakude_x;
	Platforms[i]->nakude_x *= -1;
	return platformhit(Platforms, i,g);
}
void platformnothit(int& init_x, int& init_y) {
	int k = 0;
	int k2 = 0;
	if (igrach.x - init_x>0) { k = 1; }
	if (igrach.x - init_x<0) { k = -1; }
	if (igrach.y - init_y>0) { k2 = 1; }
	if (igrach.y - init_y<0) { k2 = -2; }
	igrach.x = igrach.x + (-1)*(k);
	igrach.y = igrach.y + (-1)*(k2);
}
void platformcheckhit(int& init_x, int& init_y) {
	if (if_hit()) return;
	platformnothit(init_x, init_y);
	return platformcheckhit(init_x, init_y);
}
void insertplatforms(vector<RenderedEntity*>& source, vector<RenderedEntity*> Platforms,int index) {
	if (index == source.size()) return;
	Platforms.push_back(source[index]);
	return insertplatforms(source, Platforms, index+1);
}

bool if_hit_bullets(int x,int y,int k,int sizee)
{
	if (!bulletfor(0, Entities.size(), sizee, x, y, k)) return false;
    if(igrach.x<x+sizee && igrach.x>x-igrach.sizee && igrach.y<y+sizee && igrach.y>y-igrach.sizee)
    {
        igrach.life-=50;
        return false;
    }
    return true;
}
bool bulletfor(int i,int end,int& sizee,int& x,int& y,int& k) {
	if (i == end) return true;
	if (bulletcheck(i, sizee, x, y, k)) return true;
	return bulletfor(i + 1, end, sizee, x, y, k);
}
bool bulletcheck(int i,int sizee,int x,int y,int k) {
	if (Entities[i]->type == EntityType::Waypoint_type) return true;
	if (x<Entities[i]->x + 20 && x>Entities[i]->x - sizee && y<Entities[i]->y + 20 && y>Entities[i]->y - sizee)
	{
		if (Entities[i]->type != EntityType::Turret_x_type && Entities[i]->type != EntityType::Turret_y_type)  return false; 
		if (Entities[i]->type == EntityType::Turret_x_type && k != i)  return false; 
		if (Entities[i]->type == EntityType::Turret_y_type && k != i)  return false; 
	}
}

void shooting()
{
    vector<RenderedEntity*> Turrets_x;
    if(imam.Turret_x_ima==true) {Turrets_x=GetOfType(Entities,EntityType::Turret_x_type);}
	xturretsfor(0, Turrets_x);
    vector<RenderedEntity*> Turrets_y;
    if(imam.Turret_y_ima==true) {Turrets_y=GetOfType(Entities,EntityType::Turret_y_type);}
	yturretsfor(0, Turrets_y);
    int kolko=Bullets.size();
	Bullet k;
	bulletfor(0, kolko, k);
}
void bulletfor(int i, int end,Bullet& k) {
	if (i == end) return;
	k = Bullets.front();
	Bullets.pop();
	k.x += k.nakude_x;
	k.y += k.nakude_y;
	if (if_hit_bullets(k.x, k.y, k.ot_koy, k.sizee))
	{
		Bullets.push(k);
	}
	return bulletfor(i + 1, end,k);
}
void yturretsfor(int i, vector<RenderedEntity*>& Turrets_y) {
	if (i == Turrets_y.size()) return;
	if (Turrets_y[i]->cooldown == 0)
	{
		Bullet k(Turrets_y[i]->x + 8, Turrets_y[i]->y + 8 + Turrets_y[i]->nakude * 10, 0, Turrets_y[i]->nakude * 2, -1, Get_Position(Entities, Turret_y_type, Turrets_y[i]->x, Turrets_y[i]->y));
		Bullets.push(k);
		Turrets_y[i]->cooldown = Turrets_y[i]->kolko_da_e_cooldowna;
		Turrets_y[i]->nakude *= -1;
	}
	return xturretsfor(i + 1, Turrets_y);
}
void xturretsfor(int i,vector<RenderedEntity*>& Turrets_x) {
	if (i == Turrets_x.size()) return;
	if (Turrets_x[i]->cooldown == 0)
	{
		Bullet k(Turrets_x[i]->x + 8 + Turrets_x[i]->nakude * 10, Turrets_x[i]->y + 10, Turrets_x[i]->nakude * 2, 0, Get_Position(Entities, Turret_x_type, Turrets_x[i]->x, Turrets_x[i]->y), -1);
		Bullets.push(k);
		Turrets_x[i]->cooldown = Turrets_x[i]->kolko_da_e_cooldowna;
		Turrets_x[i]->nakude *= -1;
	}
	return xturretsfor(i + 1, Turrets_x);
}

void bombing(int i)
{
        if(Entities[i]->cooldown==0)
        {
           if(if_hitfor(0,Entities.size(),Entities[i]->x,Entities[i]->y,Get_Position(Entities,EntityType::Bomb_type,Entities[i]->x,Entities[i]->y), EntityType::Bomb_type))
            {
                Entities[i]->visited=0;
            }
        }
        if(Entities[i]->fuse==0)
        {
            Entities[i]->visited=1;
            Entities[i]->cooldown=Entities[i]->kolko_da_e_cooldowna;
            if(Bomb_map[Entities[i]->getX()][Entities[i]->getY()+1]!=0 && Entities[Bomb_map[Entities[i]->getX()][Entities[i]->getY()+1]]->visited==0)
            {
                Entities[Bomb_map[Entities[i]->getX()][Entities[i]->getY()+1]]->fuse=200;
            }
            if(Bomb_map[Entities[i]->getX()][Entities[i]->getY()-1]!=0 && Entities[Bomb_map[Entities[i]->getX()][Entities[i]->getY()-1]]->visited==0)
            {
                Entities[Bomb_map[Entities[i]->getX()][Entities[i]->getY()-1]]->fuse=200;
            }
            if(Bomb_map[Entities[i]->getX()+1][Entities[i]->getY()]!=0 && Entities[Bomb_map[Entities[i]->getX()+1][Entities[i]->getY()]]->visited==0)
            {
                Entities[Bomb_map[Entities[i]->getX()+1][Entities[i]->getY()]]->fuse=200;
            }
            if(Bomb_map[Entities[i]->getX()-1][Entities[i]->getY()]!=0 && Entities[Bomb_map[Entities[i]->getX()-1][Entities[i]->getY()]]->visited==0)
            {
                Entities[Bomb_map[Entities[i]->getX()-1][Entities[i]->getY()]]->fuse=200;
            }
        }
        if(Entities[i]->fuse>-10)
        {
            Entities[i]->fuse-=10;
        }
}
void updates()
{
	updateentities(0, Entities.size());
	Bullet k;
	updatebullets(0, k);
}
void updatebullets(int start,Bullet& k) {
	if (start == Bullets.size()) return;
	k = Bullets.front();
	Bullets.pop();
	k.Update(window);
	Bullets.push(k);
	return updatebullets(start, k);
}
void updateentities(int start,int end){
	if (start == end) return;
	Entities[start]->Update(window);
	if (Entities[start]->type == EntityType::Bomb_type) bombing(start); 
	return updateentities(start + 1, end);
}

void poll(Event& event) {
	if (window.pollEvent(event)) {
		if (event.type == Event::Closed)
			window.close();
		return;
	}
	return poll(event);
}
void windowCheck(View& view, Clock& clock, Time& elapsed1) {
	if (!window.isOpen()) return;
	view.setCenter(Vector2f(igrach.x, igrach.y));
	window.setView(view);
	Event event;
	poll(event);
	window.clear();
	updates();
	draw_board();
	mrudni();
	shooting();
	gravity();
	player_command();
	jumping();
	window.display();
	elapsed1 = clock.getElapsedTime();
	Sleep(10);
	clock.restart();
	if (cooldown>0)
	{
		cooldown -= 10;
	}
	if (cooldown2>0)
	{
		cooldown2 -= 10;
	}
	return windowCheck(view, clock, elapsed1);
}

int main()
{
    Clock clock;
    Time elapsed1 = clock.getElapsedTime();
    Left_Right k(1,1,1);
    cout<<sizeof(k);
    initialize();
    window.close();
    window.create(VideoMode(1000,600),"BattleBlock");
    View view;
    view.setCenter(Vector2f(350, 300));
    view.setSize(Vector2f(250, 150));
    Bombi=GetOfType(Entities,Bomb_type);
	windowCheck(view, clock, elapsed1);
    return 0;
}

