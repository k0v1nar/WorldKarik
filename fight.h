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

struct Attac
{
	int dam;
	int TypeD;
};
struct yOu
{
	float Maxlife;
	float life;
	float armorN;
	float armorM;
};
struct enemyInfo
{
	float life;
	int dam;
	string type;
	string name;
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
		you = design.child<Layer<void>>("you");
		enemys = design.child<Layer<enemyInfo>>("enemys");
		dam_ = design.child<Layer<void>>("dam_");
		loadDB();
	}
	void loadMap(string name, string type)
	{
		nowPos = 2;
		Your = true;
		for (auto& a : enemys.all())
		{
			a.show();
			a.child<Texture>("enem").setImageName(DB[type].enemy[name].file);
			a.child<Label>("col").setText(toString(DB[type].enemy[name].level));
			a.child<FilledRect>("hp").setSize(196,16);
			enemys.data(a).life = DB[type].enemy[name].Maxlife;
			enemys.data(a).dam = DB[type].enemy[name].dam;
			enemys.data(a).name = name;
			enemys.data(a).type = type;
		}
		dataYou.life = dataYou.Maxlife;
		for (auto& a : you.all())
		{
			
			a.child<FilledRect>("hp").setSize(196, 16);
		}
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
		for (auto& a : you.all())
		{
			if (a.id() != nowPos)
			{
				a.hide();
			}
			else
			{
				a.show();
				a.child<Texture>("youm").setImageName("emodj0.png");
				a.child<FilledRect>("hp").setSize(196 * (dataYou.life / dataYou.Maxlife), 16);
			}
		}
	}
	void updateEnemy()
	{
		for (auto& a : enemys.all())
		{
			a.show();
			a.child<FilledRect>("hp").setSize(196*(enemys.data(a).life/ DB[enemys.data(a).type].enemy[enemys.data(a).name].Maxlife),16);
			if (enemys.data(a).life <= 0)
			{
				a.hide();
			}
		}
	}
	int enemyPosLocat()
	{
		for (auto& a : enemys.all())
		{
			if (a.isMouseOn() && a.isVisible())
			{
				return a.id();
			}
		}
		return -1;
	}
	void resultAttact(Attac a,int b)
	{
		for (auto& c : enemys.all())
		{
			if (c.id() == b)
			{
				if (a.TypeD == 1)
				{
					int d2 = a.dam - DB[enemys.data(c).type].enemy[enemys.data(c).name].armN;
					if (d2 <= 0)
						d2 = 1;
					enemys.data(c).life -= d2;
					updateEnemy();
					auto& d = dam_.load("dam_anim.json", c.box().center().x+randomInt(-50,50), c.box().center().y + randomInt(-50, 50));
					d.skin<Label>().setText("-" + toString(d2));
					d.anim.play("play");
				}
				if (a.TypeD == 2)
				{
					int d2 = a.dam - DB[enemys.data(c).type].enemy[enemys.data(c).name].armM;
					if (d2 <= 0)
						d2 = 1;
					enemys.data(c).life -= d2;
					updateEnemy();
					auto& d = dam_.load("dam_anim.json",c.box().center());
					d.skin<Label>().setText("-" + toString(d2));
					d.anim.play("play");
				}
				break;
			}
		}
	}
	void attactEnemy(int i)
	{
		auto a = enemys.get(i);
		if (a.isVisible())
		{
			if (positions[nowPos].count(a.id()))
			{
				if (dataYou.armorN < enemys.data(a).dam)
					dataYou.life -= enemys.data(a).dam;
				else
					dataYou.life--;
				auto b = you.get(nowPos);
				auto& c = dam_.load("dam_anim.json", b.box().center().x + randomInt(-50, 50), b.box().center().y + randomInt(-50, 50));
				c.skin<Label>().setText("-" + toString(enemys.data(a).dam));
				c.anim.play("play");
				updateYou();
				a.anim.play("down");
				a.anim.play("up");
			}
		}
		if (i == 4)
		{
			Your = true;
			return;
		}
		callOnce(0.20, attactEnemy, i + 1);
	}
	void enemyAttact()
	{
		Your = false;
		attactEnemy(0);
	}
	yOu dataYou;
	int Kill = 0;
	bool Your;
	int nowPos=2;
	map<string , enemyDB> DB;
	Layer<void> you;
	Layer<void> dam_; 
	Layer<enemyInfo> enemys;
	set<int> positions[5] = {
		{ 0, 1 },
		{ 0, 1, 2 },
		{ 1, 2, 3 },
		{ 2, 3, 4 },
		{ 3, 4 }
	};
};