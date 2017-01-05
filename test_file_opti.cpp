#include"neshta.h"

using namespace std;
using namespace sf;

RenderWindow window(VideoMode(1000, 600), "BattleBlock"), pallet(VideoMode(160, 200), "Pallet");
vector<int> Map[50][30];
int Pallet[8][10];
bool setting = false;
int koy_x, koy_y, block = 1;


void drawmap(RectangleWrapper& square, int i, int j, int maxi, int maxj) {
	if (j == maxj) return drawmap(square, i+1, 0, maxi, maxj);
	if (i == maxi) return;
	if (!Map[i][j].empty())
	{
		square.setPosition(i, j);
		square.setFillColor(DrawColors[Map[i][j][0] - 1]);
	}
	window.draw(square);
	return drawmap(square, i, j + 1, maxi, maxj);
}
void drawpallete(RectangleWrapper square, int i, int j, int maxi, int maxj) {
	if (j == maxj) return drawpallete(square, i + 1, 0, maxi, maxj);
	if (i == maxi) return;
	square.setPosition(i, j);
	square.setFillColor(DrawColors[Pallet[i][j] - 1]);
	pallet.draw(square);
	return drawpallete(square, i, j + 1, maxi, maxj);
}
void draw_board()
{
	RectangleWrapper square(Vector2f(20, 20));
	drawmap(square, 0, 0, 50, 30);

	drawpallete(square, 0, 0, 8, 10);
}

void get_block(int x, int y)
{
	block = Pallet[x / 20][y / 20];
}

void get_outBase(int x, int y, int z, int i, int j, int maxi, int maxj) {
	if (j == maxj) return get_outBase(x, y, z, i + 1, 0, maxi, maxj);
	if (i == maxi) return;
	if (!Map[i][j].empty() && Map[i][j][0] == z)
		if (i != x || j != y)
			Map[i][j].erase(Map[i][j].begin(), Map[i][j].end());
	return get_outBase(x, y, z, i, j + 1, maxi, maxj);
}
void get_outBase(int x, int y, int z)
{
	get_outBase(x, y, z, 0, 0, 50, 30);			
}

void get_out(int x, int y)
{
	get_outBase(x, y, 7);
}
void get_out2(int x, int y)
{
	get_outBase(x, y, 8);
}

void wall(int i, int end) {
	if (i == end) return;
	Map[i][0].push_back(1);
	Map[i][29].push_back(1);
	return wall(i + 1, end);
}
void ceiling(int j, int end) {
	if (j == end) return;
	Map[0][j].push_back(1);
	Map[49][j].push_back(1);
	return wall(j + 1, end);
}
void MakeMap()
{
	wall(0, 50);
	ceiling(0, 30);
}

string choose_file()
{
	cout << "In which file do you want to save it?" << endl << "Enter:";
	string k;
	cin >> k;
	return k;
}

void palettefor(int i, int j, int maxi, int maxj,int& br) {
	if (j == maxj) return palettefor(i + 1, 0, maxi, maxj,br);
	if (i == maxi) return;
	Pallet[i][j] = br;
	br++;
	return palettefor(i, j + 1, maxi, maxj, br);
}


void coutfor(fstream& file, int i, int j, int maxi, int maxj) {
	if (j == maxj) return coutfor(file, i + 1, 0, maxi, maxj);
	if (i == maxi) return;
	if (!Map[i][j].empty()) {
		file << i << " " << j << " " << Map[i][j][0] << endl;
	}
	return coutfor(file, i, j + 1, maxi, maxj);
}

void poll(Event& event,RenderWindow& window) {
	if (window.pollEvent(event)) {
		if (event.type == Event::Closed)
		{
			window.close(); pallet.close();
		}
		return;
	}
	return poll(event,window);
}
void openwindow(Event& event) {
	if (!window.isOpen())return;
	
	poll(event, window);
	poll(event, pallet);

	window.clear();

	if (Mouse::isButtonPressed(Mouse::Left))
	{
		int localPosition_x = Mouse::getPosition(window).x;
		int localPosition_y = Mouse::getPosition(window).y;
		if (localPosition_x > 0 && localPosition_x < 1000 && localPosition_y>0 && localPosition_y < 600)
		{
			if (setting != true)
			{
				Map[localPosition_x / 20][localPosition_y / 20].erase(Map[localPosition_x / 20][localPosition_y / 20].begin(), Map[localPosition_x / 20][localPosition_y / 20].end());
				Map[localPosition_x / 20][localPosition_y / 20].push_back(block);
				if (block == 7) { get_out(localPosition_x / 20, localPosition_y / 20); }
				if (block == 8) { get_out2(localPosition_x / 20, localPosition_y / 20); }
			}
		}
		localPosition_x = Mouse::getPosition(pallet).x;
		localPosition_y = Mouse::getPosition(pallet).y;
		if (localPosition_x > 0 && localPosition_x < 150 && localPosition_y>0 && localPosition_y < 200)
		{
			get_block(localPosition_x, localPosition_y);
		}
	}
	if (Mouse::isButtonPressed(Mouse::Right))
	{
		int localPosition_x = Mouse::getPosition(window).x;
		int localPosition_y = Mouse::getPosition(window).y;
		if (localPosition_x > 0 && localPosition_x < 1000 && localPosition_y>0 && localPosition_y < 600)
		{
			Map[localPosition_x / 20][localPosition_y / 20].erase(Map[localPosition_x / 20][localPosition_y / 20].begin(), Map[localPosition_x / 20][localPosition_y / 20].end());
		}
	}
	draw_board();
	window.display();
	pallet.display();
	return openwindow(event);
}

int main()
{
	int br = 1;
	palettefor(0, 0, 8, 10, br);

	MakeMap();
	pallet.setPosition(Vector2i(window.getPosition().x - 160, window.getPosition().y));

	Event event;
	openwindow(event);
	string line = choose_file();
	fstream kray;
	kray.open(line, ios::trunc | ios::out);
	kray.close();
	fstream fail;
	fail.open(line, ios::in | ios::out);
	coutfor(fail, 0, 0, 50, 30);
	fail.seekp(fail.beg);
	fail.close();
	return 0;
}
