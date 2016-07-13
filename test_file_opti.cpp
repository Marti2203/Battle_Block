#include<iostream>
#include<fstream>
#include<cstring>
#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include<windows.h>
#include"neshta.h"
using namespace std;
sf::RenderWindow window(sf::VideoMode(1000,600),"BattleBlock");
sf::RenderWindow pallet(sf::VideoMode(160,200),"Pallet");
int Map[50][30];
int Pallet[8][10];
int block=1;
void draw_board()
{
    sf::RectangleShape square(sf::Vector2f(20,20));
    for(int i=0;i<50;i++)
    {
        for(int j=0;j<30;j++)
        {
            if(Map[i][j]==1)
            {
                square.setFillColor(sf::Color::Red);
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==2)
            {
                square.setFillColor(sf::Color::Yellow);
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==3)
            {
                square.setFillColor(sf::Color::Blue);
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==4)
            {
                square.setFillColor(sf::Color::White);
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==5)
            {
                square.setFillColor(sf::Color::Green);
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==6)
            {
                square.setFillColor(sf::Color(0,128,0));
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==7)
            {
                square.setFillColor(sf::Color(33,133,156));
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==8)
            {
                square.setFillColor(sf::Color(123,6,9));
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==9)
            {
                square.setFillColor(sf::Color(45,153,23));
                square.setPosition(i*20,j*20);
            }
            if(Map[i][j]==10)
            {
                square.setFillColor(sf::Color(102,0,204));
                square.setPosition(i*20,j*20);
            }
            window.draw(square);
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(Pallet[i][j]==1)
            {
                square.setFillColor(sf::Color::Red);
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==2)
            {
                square.setFillColor(sf::Color::Yellow);
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==3)
            {
                square.setFillColor(sf::Color::Blue);
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==4)
            {
                square.setFillColor(sf::Color::White);
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==5)
            {
                square.setFillColor(sf::Color::Green);
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==6)
            {
                square.setFillColor(sf::Color(0,128,0));
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==7)
            {
                square.setFillColor(sf::Color(33,133,156));
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==8)
            {
                square.setFillColor(sf::Color(123,6,9));
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==9)
            {
                square.setFillColor(sf::Color(45,153,23));
                square.setPosition(i*20,j*20);
            }
            if(Pallet[i][j]==10)
            {
                square.setFillColor(sf::Color(102,0,204));
                square.setPosition(i*20,j*20);
            }
            pallet.draw(square);
        }
    }
}
void get_block(int x , int y)
{
    block=Pallet[x/20][y/20];
}
void get_outBase(int x,int y,int z)
{
    for(int i=0;i<50;i++)
    {
        for(int j=0;j<30;j++)
        {
            if(Map[i][j]==z && i!=x && y!=j)
            {
                Map[i][j]=0;
            }
        }
    }
}
void get_out(int x, int y)
{
    get_outBase(x,y,7);
}
void get_out2(int x, int y)
{
    get_outBase(x,y,8);
}
void MakeMap()
{
    for(int i=0;i<50;i++)
    {
        Map[i][0]=1;
        Map[i][29]=1;
    }
    for(int j=0;j<30;j++)
    {
        Map[0][j]=1;
        Map[49][j]=1;
    }
}
string choose_file()
{
    cout<<"In which file do you want to save it?"<<endl;
    cout<<"Enter:";
    string k;
    cin>>k;
    return k;
}
int main()
{
    int br=1;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<10;j++)
        {
            Pallet[i][j]=br;
            br++;
        }
    }
    MakeMap();
    pallet.setPosition(sf::Vector2i(window.getPosition().x-160,window.getPosition().y));
    while (window.isOpen())
    {
        sf::Event event;
            while (window.pollEvent(event))
           {
               if (event.type == sf::Event::Closed){
                   window.close();pallet.close();}
           }
        sf::Event event2;
        while (pallet.pollEvent(event2))
        {
               if (event2.type == sf::Event::Closed){
                   window.close();pallet.close();}
        }
        window.clear();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int localPosition_x = sf::Mouse::getPosition(window).x;
            int localPosition_y = sf::Mouse::getPosition(window).y;
            if(localPosition_x>0 && localPosition_x<1000 && localPosition_y>0 && localPosition_y<600)
            {
                Map[localPosition_x/20][localPosition_y/20]=block;
                if(block==7){get_out(localPosition_x/20,localPosition_y/20);}
                if(block==8){get_out2(localPosition_x/20,localPosition_y/20);}
            }
            localPosition_x = sf::Mouse::getPosition(pallet).x;
            localPosition_y = sf::Mouse::getPosition(pallet).y;
            if(localPosition_x>0 && localPosition_x<150 && localPosition_y>0 && localPosition_y<200)
            {
                get_block(localPosition_x,localPosition_y);
            }
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            int localPosition_x = sf::Mouse::getPosition(window).x;
            int localPosition_y = sf::Mouse::getPosition(window).y;
            if(localPosition_x>0 && localPosition_x<1000 && localPosition_y>0 && localPosition_y<600)
            {
                Map[localPosition_x/20][localPosition_y/20]=0;
            }
        }
        draw_board();
        window.display();
        pallet.display();
    }
    string line=choose_file();
    fstream kray;
    kray.open(line,ios::trunc | ios::out);
    kray.close();
    fstream fail;
    fail.open(line,ios::in | ios::out);
    for(int i=0;i<50;i++)
    {
        for(int j=0;j<30;j++)
        {
            if(Map[i][j]!=0){
            fail<<i<<" "<<j<<" "<<Map[i][j]<<endl;
            }
        }
    }
    fail.seekp(fail.beg);
    fail.close();
    return 0;
}

