#pragma once

#include <gamebase/Gamebase.h>

using namespace gamebase;
using namespace std;

float w_f=0;
float h_f=0;

struct enemyType
{
	bool active;
	int chance;
	int armN;
	int armM;
	int level;
	int dam;
	int Maxlife=100;
	int change_miss;
	string file;
	string typeD;
	string type;
	string name;
	int life;
	map<string, int>drop;
};

struct Attac
{
	int dam;
	int TypeD;
};

struct yOu
{
	float Maxlife=100;
	float life=100;
	float armorN;
	float armorM;
	float energy=100;
	float mana=100;
	float Maxmana=100;
	float Maxenergy=100;
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
		this->design = design;
		fight_map = design.child<Layer<enemyType>>("fight_map");
		you = design.child<Layer<void>>("you").get(0);
		dam_ = design.child<Layer<void>>("dam_");
		loadDB();
	}

	void loadMap(string name, string type)
	{
		nowPos.x = 2;
		nowPos.y = 0;
		Your = true;
		int i = randomInt(3, 7);
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				auto a = fight_map.load("slot_fight.json", w_f*x, h_f*y);
				a.setSize(w_f, h_f);
				if (i > 0 && randomInt(1, 20) < 5)
				{
					a.child<Layout>("enemy").show();
					a.child<Texture>("enem").setImageName(DB[type].enemy[name].file);
					a.child<FilledRect>("life").setSize(w_f, 10);
					i--;
					fight_map.data(a).life = DB[type].enemy[name].Maxlife;
					fight_map.data(a).dam = DB[type].enemy[name].dam;
					fight_map.data(a).name = name;
					fight_map.data(a).type = type;
					fight_map.data(a).active = true;
				}
			}
		}
		dataYou.life = dataYou.Maxlife;
		dataYou.mana = dataYou.Maxmana;
		dataYou.energy = dataYou.Maxenergy;
		you.setSize(w_f, h_f);
		updateYou();
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
				int b4;
				input >> b4;
				string a3;
				input >> a3;
				b2.drop[a3] = b4;
			}
			DB[a].enemy[a2] = b2;
		}
	}

	void updateYou()
	{
		you.setPos(nowPos.x*w_f,nowPos.y*h_f);
		you.child<FilledRect>("life_you").setSize(w_f * (dataYou.life / dataYou.Maxlife), 8);
		you.child<FilledRect>("energy_you").setSize(w_f * (dataYou.energy/dataYou.Maxenergy),8);
		you.child<FilledRect>("mana_you").setSize(w_f * (dataYou.mana/dataYou.Maxmana), 8);
		design.update();
	}
	
	Layout design;
	yOu dataYou;
	int Kill = 0;
	bool Your;
	IntVec2 nowPos=IntVec2(3,0);
	map<string , enemyDB> DB;
	GameObj you;
	Layer<enemyType> fight_map;
	Layer<void> dam_; 
	
};
