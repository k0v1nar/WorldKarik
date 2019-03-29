#pragma once

#include <gamebase/Gamebase.h>

using namespace gamebase;
using namespace std;

enum TypeIt
{
	armors,
	weapons,
	resources,
	potions
};
enum Rarity
{
	Common,
	Rare,
	Epic,
	Legendary
};
struct Potion
{
	enum Type
	{
		Heal,
		NArmUp,
		MArmUp,
		SpeedUp
	};
	int PowEffects;
	Type type;
	bool isLeft;
	bool active;
	int num;
};
struct Armor
{
	enum Type
	{
		helmet,
		chesplat,
		leggs,
		boots
	};
	Type type;
	int def, level;
	Rarity rarity;
	bool active;
};
struct Weapon
{
	enum TypeD
	{
		Near_natural,
		Distant_natural,
		Near_magic,
		Distant_magic,
	};
	enum Type
	{
		sword,
		axe,
		pickaxe
	};
	int dam, level;
	TypeD typeD;
	Type typeW;
	Rarity rarity;
	bool active;
	bool isLeft;

};

struct Resource
{
	int number;
	bool full() { return number == 20; };
};
struct Slot
{
	bool empty = true;
	TypeIt type;
	string name;
	union
	{
		Armor armor;
		Weapon weapon;
		Resource resource;
		Potion potion;
	} data;
	int x, y, num;
};

struct Item
{
	string name;
	string file;
	TypeIt type;
	union
	{
		Armor armor;
		Weapon weapon;
		Resource resource;
		Potion potion;
	} data;
};

struct Recipe
{
	int col;
	int from;
	map<string, int> recipe;
};
class Inventor
{
	public:

	//load
	void load(Layout design,int w, int h)
	{
		w3 = w;
		h2 = h;
		slot = design.child<Layer<void>>("slot");
		int i = 0;
		for (int y = 0; y < 5; y++)
			for (int x = 0; x < 9; x++)
			{
				Slot slott;
				slott.num = i;
				slots.push_back(slott);
				auto& slo = slot.load("slot.json", x*w3, y*h2);
				slo.child<Texture>("obj").hide();
				slo.child<DrawObj>("num").hide();
				i++;
			}
		readDB();
		readRDB();
	}
	void readDB()
	{
		ifstream input("data/load/item/db.txt");
		for (int res = 0; 1 < 2; res++)
		{
			string name2;
			input >> name2;
			if (name2 == "end")
				break;
			string b;
			input >> b;
			if (b == "resources")
			{

				string name;
				input >> name;
				for (auto& f : name)
				{
					if (f == '_')
					{
						f = ' ';
					}
				}
				DB[name2].name = name;
				string file;
				input >> file;
				DB[name2].file = file;
				DB[name2].type = resources;
			}
			else if (b == "weapon")
			{

				string name;
				string file;
				string typ;
				input >> name;
				for (auto& f : name)
				{
					if (f == '_')
					{
						f = ' ';
					}
				}
				DB[name2].name = name;
				DB[name2].type = weapons;
				int g;
				input >> g;
				DB[name2].data.weapon.level = g;
				input >> typ;
				if (typ == "axe")
				{
					DB[name2].data.weapon.typeW = DB[name2].data.weapon.axe;
					input >> typ;
					DB[name2].data.weapon.typeD = DB[name2].data.weapon.Near_natural;
					input >> g;
					DB[name2].data.weapon.dam = g;
				}
				input >> file;
				DB[name2].file = file;
			}
		}
		input.close();
	}
	void readRDB()
	{
		for (auto b = 1; b <= 5; b++)
		{
			ifstream input("data/load/recipe/recipe" + toString(b) + ".txt");
			int a = 0;
			input >> a;
			for (auto i = 0; i < a; i++)
			{
				string name;
				input >> name;
				int c;
				input >> c;
				RDB[name].col = c;
				RDB[name].from = b;
				for (auto i2 = 0; i2 < c; i2++)
				{
					string namerec;
					input >> namerec;
					int c2;
					input >> c2;
					RDB[name].recipe[namerec] = c2;
				}
			}
		}
	}

	//slot_work
	void updateSlot(int i)
	{
		auto& a = slots[i];
		auto b = slot.get(i);
		if (a.empty)
		{
			b.child<Texture>("obj").hide();
			b.child<DrawObj>("num").hide();
			b.child<Texture>("active").hide();
			b.child<Texture>("left").hide();
			b.child<Texture>("right").hide();
			return;
		}
		b.child<DrawObj>("num").show();
		if (a.type == resources)
		{
			b.child<DrawObj>("num").show();
			b.child<Label>("col").setText(toString(a.data.resource.number));
			b.child<Texture>("obj").setImageName(DB[a.name].file);
			b.child<Texture>("left").hide();
			b.child<Texture>("right").hide();
		}
		if (a.type == weapons)
		{
			if (a.data.weapon.isLeft && a.data.weapon.active)
			{
				b.child<Texture>("left").show();
				b.child<Texture>("active").show();
			}
			if (!a.data.weapon.isLeft && a.data.weapon.active)
			{
				b.child<Texture>("right").show();
				b.child<Texture>("active").show();
			}
			b.child<DrawObj>("num").hide();
			b.child<Texture>("obj").setImageName(DB[a.name].file);
		}
		b.child<Texture>("obj").show();
	}
	void seekSlot(string type, TypeIt i, int c)
	{
		for (auto& a : slots)
		{
			if (a.empty)
				continue;
			if (a.type == resources)
			{
				if (!a.data.resource.full() && a.name == type)
				{
					if (a.data.resource.number + c <= 20)
					{
						a.data.resource.number += c;
						c = 0;
						break;
					}
					c -= 20 - a.data.resource.number;
					a.data.resource.number = 20;
				}
			}
			else
				if (a.type == potions)
				{

				};
		}
		if (c > 0)
		{
			for (auto& a : slots)
			{
				if (a.empty)
				{
					a.empty = false;
					if (i == resources)
					{
						a.type = resources;
						a.name = type;
						if (c <= 20)
						{
							a.data.resource.number = c;
							break;
						}
						c -= 20;
						a.data.resource.number = 20;
					}
					if (i == weapons)
					{
						a.type = weapons;
						a.name = type;
						c--;
						if (c == 0)
						{
							break;
						}
					}


				}
			}
		}
	}
	
	//item_work
	int seekItem(string i)
	{
		int b = 0;
		for (auto& a : slots)
		{
			if (!a.empty)
			{
				if (a.type == resources)
				{
					if (a.name == i)
					{
						b += a.data.resource.number;
					}
				}
			}
		}
		return b;
	}
	void delItem(string i, int a)
	{
		for (auto& b : slots)
		{
			if (!b.empty)
			{
				if (b.type == resources)
				{
					if (b.name == i)
					{
						if (a > b.data.resource.number)
						{
							a -= b.data.resource.number;
							b.empty = true;
							updateSlot(b.num);
						}
						if (a <= b.data.resource.number)
						{
							b.data.resource.number -= a;
							if (b.data.resource.number == 0)
								b.empty = true;
							updateSlot(b.num);
						}
					}
				}
			}
		}
	}
	
	//members
	map<string, Recipe> RDB;
	map<string, Item> DB;
	vector <Slot> slots;
	Layer<void> slot;
	int nowSlot;
	int recipeCol = 0;
	int w3;
	int h2;
};
