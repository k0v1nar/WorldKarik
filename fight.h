#pragma once

#include <gamebase/Gamebase.h>

using namespace gamebase;
using namespace std;


struct enemyType
{
	int armN;
	int armM;
	int level;
	int dam;
	int Maxlife;
	int change_miss;
	string file;
	string typeD;
	map<string, int>drop;
};

struct enemyInfo
{
	int life;
	string type;
	string name;
	int pos;
};

struct enemyDB
{
	map<string, enemyType> enemy;
};

class Fight
{
	public:
	void load(Layout design,float h2)
	{
		h = h2;
		you = design.child<Layer<void>>("you");
		enemys = design.child<Layer<enemyInfo>>("enemys");
		loadDB();
	}
	void loadMap()
	{
		nowPos = 3;
		Your = true;
	}
	void loadDB()
	{
		ifstream input("data/load/fight/db.txt");
		string a;
		input >> a;
		int b;
		input >> b;
		for (int i = 0; i < b; i++)
		{
			string a2;
			input >> a2;
			enemyType b2;
			input >> b2.level;
			input >> b2.armN;
			input >> b2.armM;
			input >> b2.dam;
			input >> b2.typeD;
			input >> b2.Maxlife;
			input >> b2.change_miss;
			input >> b2.file;
			int b3;
			input >> b3;
			for (auto i2 = 0; i2 < b3; i2++)
			{
				string a3;
				input >> a3;
				int b4;
				input >> b4;
				b2.drop[a3] = b4;
			}
		}
	}
	bool Your;
	int nowPos;
	map<string , enemyDB> DB;
	Layer<void> you;
	Layer<enemyInfo> enemys;
};