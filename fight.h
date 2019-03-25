#pragma once

#include <gamebase/Gamebase.h>

using namespace gamebase;
using namespace std;


struct enemyType
{
	int chance;
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
		you = design.child<Layer<bool>>("you");
		enemys = design.child<Layer<enemyInfo>>("enemys");
		loadDB();
		updateYou();
	}
	void loadMap(string name, string type)
	{
		nowPos = 3;
		Your = true;
		for (auto& a : enemys.all())
		{
			a.child<Texture>("enem").setImageName(DB[type].enemy[name].file);
			a.child<Label>("col").setText(toString(DB[type].enemy[name].level));
			enemys.data(a).life = DB[type].enemy[name].Maxlife;
			enemys.data(a).name = name;
			enemys.data(a).type = type;
			enemys.data(a).pos = a.id()+1;
		}
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
			input >> b2.chance;
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
			DB[a].enemy[a2] = b2;
		}
	}
	
	void updateYou()
	{
		for (auto& a : you.all())
		{
			if (a.id() != nowPos - 1)
			{
				you.data(a) = false;
				a.hide();
			}
			else
			{
				you.data(a) = true;
				a.show();
				a.child<Texture>("youm").setImageName("emodj0.png");
			}
		}
	}



	bool Your;
	int nowPos=3;
	map<string , enemyDB> DB;
	Layer<bool> you;
	Layer<enemyInfo> enemys;
};