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
	bool active;
	float life;
};
struct enemyInfo
{
	float life;
	int dam;
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
		you = design.child<Layer<yOu>>("you");
		enemys = design.child<Layer<enemyInfo>>("enemys");
		loadDB();
		updateYou();
	}
	void loadMap(string name, string type)
	{
		nowPos = 3;
		Your = true;
		int b = 1;
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
			enemys.data(a).pos = b;
			b++;
		}
		for (auto& a : you.all())
		{
			you.data(a).life = 50;
			a.child<FilledRect>("hp").setSize(196, 16);
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
			if (a.id() != nowPos - 1)
			{
				you.data(a).active = false;
				a.hide();
			}
			else
			{
				you.data(a).active = true;
				a.show();
				a.child<Texture>("youm").setImageName("emodj0.png");
				a.child<FilledRect>("hp").setSize(196 * (you.data(a).life / 50), 16);
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
			if (a.isMouseOn())
			{
				return enemys.data(a).pos;
			}
		}
	}
	void resultAttact(Attac a,int b)
	{
		for (auto& c : enemys.all())
		{
			if (enemys.data(c).pos == b)
			{
				cout << a.dam<<endl;
				if (a.TypeD == 1)
				{
					int d = a.dam - DB[enemys.data(c).type].enemy[enemys.data(c).name].armN;
					if (d <= 0)
						enemys.data(c).life -= 1;
					else
						enemys.data(c).life -= d;
					updateEnemy();
				}
				if (a.TypeD == 2)
				{
					int d = DB[enemys.data(c).type].enemy[enemys.data(c).name].armM - a.dam;
					if (d <= 0)
						enemys.data(c).life -= 1;
					else
						enemys.data(c).life -= d;
					updateEnemy();
				}
				
				break;
			}

		}
		
	}

	void enemyAttact()
	{

		for (auto a : enemys.all())
		{
			if (a.isVisible)
			{
				if (nowPos == 1)
				{
					for (auto b : you.all())
					{
						you.data(b).life -= enemys.data(a).dam;
					}
					if (a.id() == 1)
					{
						break;
					}
				}
				if (nowPos == 2)
				{
					for (auto b : you.all())
					{
						you.data(b).life -= enemys.data(a).dam;
					}
					if (a.id() == 2)
					{
						break;
					}
				}
				if (nowPos == 3)
				{
					if (a.id() > 0)
					{
						for (auto b : you.all())
						{
							you.data(b).life -= enemys.data(a).dam;
							cout << you.data(b).life << endl;
							cout << enemys.data(a).dam << endl;
						}
					}
					if (a.id() == 3)
					{
						break;
					}
				}
				if (nowPos == 4)
				{
					if (a.id() > 1)
					{
						for (auto b : you.all())
						{
							you.data(b).life -= enemys.data(a).dam;
						}
					}
				}
				if (nowPos == 5)
				{
					if (a.id() > 2)
					{
						for (auto b : you.all())
						{
							you.data(b).life -= enemys.data(a).dam;
						}
					}
				}
				updateYou();
			}
		}
		Your = true;
	}

	int Kill = 0;
	bool Your;
	int nowPos=3;
	map<string , enemyDB> DB;
	Layer<yOu> you;
	Layer<enemyInfo> enemys;
};