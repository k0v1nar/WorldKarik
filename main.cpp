#include <gamebase/Gamebase.h>
#include <SFML/Window/VideoMode.hpp>
#include "world.h"

using namespace gamebase;
using namespace std;
float w, h, w3, h2;
bool save = false; 
class MyApp : public App
{
	
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
	int seekItem(string i)
	{
		int b=0;
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
							b.data.resource.number-=a;
							if (b.data.resource.number==0)
							b.empty = true;
							updateSlot(b.num);
						}
					}
				}
			}
		}
	}
	int dropnumcr=1;
	void menucraft(int i, string a)
	{
		if (i == 0)
		{
			Menudr.hide();
			Max.show();
			more.show();
			dropnumcr = 1;
		}
		if (i == 1)
		{
			Menudr.hide();
			for (auto& b : RDB[a].recipe)
			{
				delItem(b.first,dropnumcr*b.second);
			}
			seekSlot(a, DB[a].type, dropnumcr);
			dropnumcr = 1;
			Max.show();
			more.show();
		}
		if (i == 2)
		{
			dropnumcr -= 1;
			Max.show();
			more.show();
			if (dropnumcr == 1)
			{
				less.hide();
				Min.hide();
			}
		}
		if (i == 3)
		{
			dropnumcr += 1;
			Min.show();
			less.show();
			if (dropnumcr == recipeCol)
			{
				Max.hide();
				more.hide();
			}
		}
		if (i == 4)
		{
			dropnumcr = 1;
			Min.hide();
			less.hide();
			Max.show();
			more.show();
		}
		if (i == 5)
		{
			dropnumcr = recipeCol;
			Min.show();
			less.show();
			Max.hide();
			more.hide();
		}
		coldrop << dropnumcr;
		if (recipeCol == 1)
		{
			Max.hide();
			more.hide();
		}
	}
	int recipeCol = 0;
	void craftItem(string i)
	{
		int min = 99999999;
		for (auto& a : RDB[i].recipe)
		{
			int c = seekItem(a.first) / a.second;
			if (c < min)
			{
				min = c;
			}
		}
		recipeCol = min;
		coldrop << 1;
		Menudr.show();
		less.hide();
		Min.hide();
		Max.show();
		more.show();
		if (recipeCol == 1)
		{
			less.hide();
			more.hide();
			Min.hide();
			Max.hide();
		}
		if (recipeCol == 0)
		{
			Menudr.hide();
		}
		connect(back, menucraft, 0,i);
		connect(cont, menucraft, 1,i);
		connect(less, menucraft, 2,i);
		connect(more, menucraft, 3,i);
		connect(Min, menucraft, 4,i);
		connect(Max, menucraft, 5,i);
	}
	void loadFight()
	{
		
	}
	void recipeLoad()
	{
		for (auto b = 1; b <= 5; b++)
		{
			auto cr = design.child<Layout>("cr" + toString(b - 1));
			cr.child<Layout>("stolb").clear();
		}
		for (auto& b : RDB)
		{
			auto cr = design.child<Layout>("cr" + toString(b.second.from - 1));
			auto b2 = cr.child<Layout>("stolb").load("recipe.json");
			b2.child<Label>("name").setText(DB[b.first].name);
			b2.child<Texture>("crpict").setImageName(DB[b.first].file);
			for (auto& i : b.second.recipe)
			{
				auto bb = b2.child<Layout>("item").load("sl.json");
				bb.child<Texture>("pict").setImageName(DB[i.first].file);
				bb.child<Label>("col") << " x " << i.second;
			}
			connect(b2.child<Button>("create"),craftItem,b.first);
		}
		design.update();
	}
	void readPerson()
	{
		ifstream input;
		if(save)
			input.open("data/save/progress/person/stats.txt");
		else
		{
			input.open("data/load/person/stats.txt");
		}	
		string b;
		input >> b;
		auto& name = stats.load<Label>("stat.json");	
		name.setText(tr("name") + b);
		input >> b;
		auto& clas = stats.load<Label>("stat.json");
		clas.setText(tr("class") + tr(b));
		int b2;
		input >> b2;
		auto& lev = stats.load<Label>("stat.json");
		lev.setText(tr("level") + toString(b2));
		input >> b2;
		auto& life = stats.load<Label>("stat.json");
		life.setText(tr("life") + toString(b2));
		input >> b2;
		auto& armor = stats.load<Label>("stat.json");
		armor.setText(tr("arm") + toString(b2));
		input >> b2;
		auto& armorM = stats.load<Label>("stat.json");
		armorM.setText(tr("armM") + toString(b2));
		input >> b2;
		auto& manna = stats.load<Label>("stat.json");
		manna.setText(tr("manna") + toString(b2));
	}
	void readRDB()
	{
		for (auto b = 1; b <= 5; b++)
		{
			ifstream input("data/load/recipe/recipe"+toString(b)+".txt");
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
	
	void readDB()
	{
		ifstream input("data/load/item/db.txt");
		for (int res=0;1<2;res++)
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
			else if (b=="weapon")
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
	
    void load()
    {
		ifstream input("data/save.txt");
		bool s;
		input >> save;
		loadTextBank("textbank.json");
		readDB();
		world.w = w;
		world.h = h;
		world.load(design);
		readRDB();
		Menu.hide();
		int i = 0;
		for (int y = 0; y < 5; y++)
			for (int x=0;x<9;x++)
			{
				Slot slott;
				slott.num = i;
				slots.push_back(slott);
				auto& slo = slot.load("slot.json", x*w3, y*h2);
				slo.child<Texture>("obj").hide();
				slo.child<DrawObj>("num").hide();
				i++;
			}
		hideCursor();
		field.setView(world.player.pos());
		connect(equiped, nexti, 1);
		connect(map, nexti, 2);
		connect(crafts, nexti, 4);
		connect(skill, nexti, 3);
		connect(inventor, nexti, 0);
		connect(outfit, nextCr, 0);
		connect(tools, nextCr, 1);
		connect(items, nextCr, 2);
		connect(furniture, nextCr, 3);
		connect(material, nextCr, 4);
		connect(back, drm, 0);
		connect(cont, drm, 1);
		connect(less, drm, 2);
		connect(more, drm, 3);
		connect(Min, drm, 4);
		connect(Max, drm, 5);
		connect(use, drawQieup);
		connect(w1, handuse, true);
		connect(w2, handuse, false);
		connect(p1, potionuse, true);
		connect(p2, potionuse, false);
		seekSlot("wooden_axe", weapons, 1);
		readPerson();
		/*
		seekSlot("grass", resources, 100);
		seekSlot("wood", resources, 100);
		seekSlot("stick", resources, 100);*/
    }
	void icon_del(string i)
	{
		fieldEquiped.child<GameObj>(i).child<Texture>("slot2").show();
		fieldEquiped.child<GameObj>(i).child<Texture>("slot").hide();
	}
	void clearQieup()
	{
		slot.get(nowSlot).child<Texture>("active").hide();
		if (slots[nowSlot].type == weapons)
		{
			if (slots[nowSlot].data.weapon.active)
			{
				if (slots[nowSlot].data.weapon.isLeft)
				{
					icon_del("weapon1");
					MenuW.hide();
					slots[nowSlot].data.weapon.active = false;
					slots[nowSlot].data.weapon.isLeft = false;
					nowSlot = -1;
				}
				else
				{
					icon_del("weapon2");
					MenuW.hide();
					slots[nowSlot].data.weapon.active = false;
					slots[nowSlot].data.weapon.isLeft = false;
					nowSlot = -1;
				}
			}
		}
		if (slots[nowSlot].type == potions)
		{

		}
		if (slots[nowSlot].data.armor.active)
			if (slots[nowSlot].type == armors)
			{
				if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.boots)
				{
					icon_del("boots");
				}
				if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.chesplat)
				{
					icon_del("chair_armor");
				}
				if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.helmet)
				{
					icon_del("helmet");
				}
				if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.leggs)
				{
					icon_del("legss");
				}
				slots[nowSlot].data.armor.active = false;
				nowSlot = -1;
			}
		Menu.hide();
	}
	void handuse(bool i)
	{
		
		if (!slots[nowSlot].data.weapon.active)
		{
			if (i)
			{
				icon_use("weapon1",nowSlot);
				MenuW.hide();
				slots[nowSlot].data.weapon.active = true;
				slots[nowSlot].data.weapon.isLeft = true;
				nowSlot = -1;
			}
			else
			{
				icon_use("weapon2", nowSlot);
				MenuW.hide();
				slots[nowSlot].data.weapon.active = true;
				slots[nowSlot].data.weapon.isLeft = false;
				nowSlot = -1;
			}
		}
	}
	void potionuse(bool i)
	{
		if (i)
		{
			icon_use("poiton1", nowSlot);
			MenuP.hide();
			slots[nowSlot].data.potion.active = true;
			slots[nowSlot].data.potion.isLeft = true;
			nowSlot = -1;
		}
		else
		{
			icon_use("poiton2", nowSlot);
			MenuP.hide();
			slots[nowSlot].data.potion.active = true;
			slots[nowSlot].data.potion.isLeft = true;
			nowSlot = -1;
		}
	}
	void icon_use(string i,int i2)
	{
		fieldEquiped.child<GameObj>(i).child<Texture>("slot2").hide();
		fieldEquiped.child<GameObj>(i).child<Texture>("slot").show();
		fieldEquiped.child<GameObj>(i).child<Texture>("slot").setImageName(DB[slots[i2].name].file);
	}
	void drawQieup()
	{
		slot.get(nowSlot).child<Texture>("active").show();
		if (slots[nowSlot].type == weapons)
		{
			connect(w1, handuse, true);
			connect(w2, handuse, false);
			MenuW.show();
		}
		if (slots[nowSlot].type == potions)
		{
			connect(p1, potionuse, true);
			connect(p2, potionuse, false);
			MenuP.show();
		}
		if (!slots[nowSlot].data.armor.active)
		if (slots[nowSlot].type == armors)
		{
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.boots)
			{
				icon_use("boots", nowSlot);
			}
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.chesplat)
			{
				icon_use("chair_armor", nowSlot);
			}
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.helmet)
			{
				icon_use("helmet", nowSlot);
			}
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.leggs)
			{
				icon_use("legss", nowSlot);
			}
			slots[nowSlot].data.armor.active = true;
			nowSlot = -1;
		}
		Menu.hide();
	}
	void conmenu()
	{
			connect(back, drm, 0);
			connect(cont, drm, 1);
			connect(less, drm, 2);
			connect(more, drm, 3);
			connect(Min, drm, 4);
			connect(Max, drm, 5);
	}
	int dropnum = 1;
	void drm(int i)
	{
		
		if (i == 0)
		{
			Menudr.hide();
			nowSlot = -1;
			dropnum = 1;
		}
		if (i == 1)
		{
			Menudr.hide();
			slots[nowSlot].data.resource.number -= dropnum;
			if (slots[nowSlot].data.resource.number == 0)
			{
				slots[nowSlot].empty = true;
			}
			updateSlot(nowSlot);
			design.update();
			Menudr.hide();
			dropnum = 1;
			nowSlot = -1;
		}
		if (i == 2)
		{
			dropnum -= 1;
			Max.show();
			more.show();
			if (dropnum == 1)
			{
				less.hide();
				Min.hide();
			}
		}
		if (i == 3)
		{
			dropnum += 1;
			Min.show();
			less.show();
			if (dropnum == slots[nowSlot].data.resource.number)
			{
				Max.hide();
				more.hide();
			}
		}
		if (i == 4)
		{
			dropnum = 1;
			Min.hide();
			less.hide();
			Max.show();
			more.show();
		}
		if (i == 5)
		{
			dropnum = slots[nowSlot].data.resource.number;
			Min.show();
			less.show();
			Max.hide();
			more.hide();
		}
		coldrop << dropnum;
		if (slots[nowSlot].data.resource.number == 1)
		{
			Max.hide();
			more.hide();
		}
	}
	void nextCr(int i)
	{
		changerCr.select(i);
		design.update();
	}
	void updateSlot(int i)
	{
		auto& a = slots[i];
		auto b = slot.get(i);
		if (a.empty)
		{
			b.child<Texture>("obj").hide();
			b.child<DrawObj>("num").hide();
			b.child<Texture>("active").hide();
			return;
		}
		b.child<DrawObj>("num").show();
		if (a.type == resources)
		{
			b.child<DrawObj>("num").show();
			b.child<Label>("col").setText(toString(a.data.resource.number));
			b.child<Texture>("obj").setImageName(DB[a.name].file);
		}
		if (a.type == weapons)
		{
			b.child<DrawObj>("num").hide();
			b.child<Texture>("obj").setImageName(DB[a.name].file);
		}
		b.child<Texture>("obj").show();
		design.update();
	}
	
	void nexti(int i)
	{
		changer.select(i);
		if (i == 0)
		{
			for (auto& a : slots)
			{
				updateSlot(a.num);
			}
			design.update();
			conmenu();
			fieldInventor.setView(4 * w3, 2 * h2);
		}
		if (i == 4)
		{
			recipeLoad();
		}
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


	int Vec2ToInt(Vec2 i)
	{
		IntVec2 a;
		a.x = (i.x + w3 / 2) / w3;
		a.y = (i.y + h2 / 2) / h2;
		int b = a.y * 9 + a.x;
		return b;
	}
	void updateQuiep()
	{
		int i = 0;
		icon_del("weapon1");
		icon_del("weapon2");
		icon_del("potion1");
		icon_del("potion2");
		icon_del("legss");
		icon_del("chair_armor");
		icon_del("boots");
		icon_del("helmet");
		for (auto a : slot.all())
		{
			if ((slots[i].data.armor.active && slots[i].type == armors) || (slots[i].data.weapon.active && slots[i].type == weapons) || (slots[i].data.potion.active && slots[i].type == potions))
			{
				if (slots[i].type == weapons && slots[i].data.weapon.isLeft)
				{
					icon_use("weapon1",i);
				}
				if (slots[i].type == weapons && !slots[i].data.weapon.isLeft)
				{
					icon_use("weapon2",i);
				}
				if (slots[i].type == potions && slots[i].data.potion.isLeft)
				{
					icon_use("potion1",i);
				}
				if (slots[i].type == potions && !slots[i].data.potion.isLeft)
				{
					icon_use("potion2",i);
				}
				if (slots[i].type == armors)
				{
					if (slots[i].data.armor.type == slots[i].data.armor.boots)
					{
						icon_use("boots",i);
					}
					if (slots[i].data.armor.type == slots[i].data.armor.chesplat)
					{
						icon_use("chair_armor",i);
					}
					if (slots[i].data.armor.type == slots[i].data.armor.helmet)
					{
						icon_use("helmet",i);
					}
					if (slots[i].data.armor.type == slots[i].data.armor.leggs)
					{
						icon_use("legss",i);
					}
				}
			}
			i++;
		}
	}
	void Drop(int i)
	{
		if (slots[nowSlot].type == resources || slots[nowSlot].type == potions)
		{
			Menudr.show();
			if (slots[nowSlot].data.resource.number == 1)
			{
				Max.hide();
				more.hide();
			}
		}
		else
		{
			slots[nowSlot].empty = true;
			updateSlot(nowSlot);
			updateQuiep();
		}
		Menu.hide();
		Min.hide();
		less.hide();
		if (slots[nowSlot].data.resource.number> 1)
		{
			Max.show();
			more.show();
		}
	}
	int isMouse=0;
    void process(Input input)
    {
        using namespace gamebase::InputKey;
		
		if (selector.selected() == 2)
		{
			if (input.justPressed(Escape))
			{
				selector.select(1);
				cr0.child<Layout>("stolb").clear();
				cr1.child<Layout>("stolb").clear();
				cr2.child<Layout>("stolb").clear();
				cr3.child<Layout>("stolb").clear();
				cr4.child<Layout>("stolb").clear();
				hideCursor();
				return;
			}
			if (changer.selected()==0 && !Menudr.isVisible() && !MenuW.isVisible() && !MenuP.isVisible())
			{ 
				if (input.justPressed(MouseRight))
				{
					if (isMouse == 1)
					{
						slot.get(nowSlot).child<DrawObj>("sel").hide();
						nowSlot = -1;
						isMouse = 0;
					}
					else
					{
						int a = Vec2ToInt(fieldInventor.mousePos());
						nowSlot = a;
						if (!slots[nowSlot].empty)
						{
							auto b = slot.get(nowSlot);
							if (nowSlot==8||nowSlot==17||nowSlot==26||nowSlot==35||nowSlot==44)
							Menu.setPos(b.pos().x - w3, b.pos().y);
							else
								Menu.setPos(b.pos().x + w3, b.pos().y);
							Menu.show();
							Menu.child<Button>("use").show();
							if (slots[nowSlot].type == resources)
								Menu.child<Button>("use").hide();
							if ((slots[nowSlot].data.armor.active && slots[nowSlot].type==armors) || (slots[nowSlot].data.weapon.active && slots[nowSlot].type == weapons) || (slots[nowSlot].data.potion.active && slots[nowSlot].type == potions))
							{
								Menu.child<Button>("use").child<Label>("label").setText(tr("drop"));
								connect(use, clearQieup);
							}
							if ((!slots[nowSlot].data.armor.active && slots[nowSlot].type == armors) || (!slots[nowSlot].data.weapon.active && slots[nowSlot].type == weapons) || (!slots[nowSlot].data.potion.active && slots[nowSlot].type == potions))
							{
								Menu.child<Button>("use").child<Label>("label").setText(tr("use"));
								connect(use, drawQieup);
							}
							connect(drop, Drop, nowSlot);
						}
						isMouse = 2;
						design.update();
					}
				}
			
			if (!Menu.isVisible())
				if (input.justPressed(MouseLeft))
				{
					 if (isMouse == 0)
					{
						isMouse = 1;
						nowSlot = Vec2ToInt(fieldInventor.mousePos());
						auto b = slot.get(nowSlot);
						b.child<DrawObj>("sel").show();
					}
					else
					{
						isMouse = 0;
						int a = Vec2ToInt(fieldInventor.mousePos());
						auto b2 = slot.get(nowSlot);
						b2.child<DrawObj>("sel").hide();
						swap(slots[nowSlot], slots[a]);
						updateSlot(nowSlot);
						updateSlot(a);
						design.update();
					}
				}
			}
			if (Menu.isVisible())
				if (input.justPressed(MouseLeft))
				{
					if (isMouse == 2
						&& !impl::isMouseOn(
							dynamic_cast<impl::Drawable*>(Menu.getImpl()->getInternalObj().get())))
					{
						Menu.hide();
						nowSlot = -1;
						isMouse = 0;
						design.update();
					}
				}
		}
		if (selector.selected() == 1)
		{
			int a = 0, b = 0;
			if (input.justPressed(Escape))
			{
				selector.select(0);
				showCursor();
			}
			if (input.justPressed(M))
			{
				selector.select(2);
				nexti(2);
				showCursor();
			}
			if (input.justPressed(T))
			{
				selector.select(2);
				nexti(1);
				showCursor();
			}
			if (input.justPressed(Y))
			{
				selector.select(2);
				nexti(4);

				recipeLoad();
				design.update();
				showCursor();
			}
			if (input.justPressed(U))
			{
				selector.select(2);
				nexti(3);
				showCursor();
			}
			if (input.pressed(A))
			{
				world.player.move(Vec2(-300, 0) * timeDelta());
				auto&objs = world.Front.find(world.player);
				if (!objs.empty())
				{
					world.updateNO(objs);
					world.player.move(Vec2(300, 0) * timeDelta());
					b++;
				}
				a++;
			}
			if (input.pressed(W))
			{
				world.player.move(Vec2(0, 300) * timeDelta());
				auto&objs = world.Front.find(world.player);
				if (!objs.empty())
				{
					world.updateNO(objs);
					world.player.move(Vec2(0, -300) * timeDelta());
					b++;
				}
				a++;
			}
			if (input.pressed(S))
			{
				world.player.move(Vec2(0, -300) * timeDelta());
				auto&objs = world.Front.find(world.player);
				if (!objs.empty())
				{
					world.updateNO(objs);
					world.player.move(Vec2(0, 300) * timeDelta());
					b++;
				}
				a++;
			}
			if (input.pressed(D))
			{
				world.player.move(Vec2(300, 0) * timeDelta());
				auto&objs = world.Front.find(world.player);
				if (!objs.empty())
				{
					world.updateNO(objs);
					world.player.move(Vec2(-300, 0) * timeDelta());
					b++;
				}
				a++;
			}
			if (a != b && world.nowObj)
			{
				world.nowObjInter = false;
				world.nowObj.anim.play("lowlight");
			}
			if (input.pressed(Tab))
			{
				selector.select(2);
				nexti(0);
				conmenu();
				showCursor();
			}
			if (input.justPressed(E))
			{

				if (world.nowObjInter == true)
				{
					if (world.Front.data(world.nowObj).typ == Obj)
					{
						world.Front.data(world.nowObj).vision -= 25.5;
						auto color = world.nowObj.skin<Texture>().color();
						color.a = world.Front.data(world.nowObj).vision;
						world.nowObj.skin<Texture>().setColor(color);
						world.Front.data(world.nowObj).hp -= 10;
						auto& b = world.DB["forest"][world.Front.data(world.nowObj).type];
						for (auto& b2 : b.obect.drop)
						{
							int dd2 = randomInt(1, 100);
							if (dd2 <= b2.chance)
							{
								seekSlot(b2.name, resources, 1);
							}
						}
						if (world.Front.data(world.nowObj).hp <= 0)
						{
							auto obj = world.Front.find(world.nowObj).front();
							GameObj i;
							world.nowObj = i;
							world.chunks[Vec2ToIntVec2(obj.pos())].map[world.Front.data(obj).thisObj] = None;
							obj.kill();
							world.nowObjInter = false;
						}
					}
					else if (world.Front.data(world.nowObj).typ == Enemy)
					{
						auto obj = world.Front.find(world.nowObj).front();
						selector.select(3);
						obj.kill();
						showCursor();
						world.nowObjInter = false;
					}
				}
			}
		}
    }


	IntVec2 Vec2ToIntVec2(Vec2 v)
	{
		IntVec2 iv;
		if (v.x + w / 2 >= 0)
		{
			iv.x = (v.x + w / 2) / w / 10;
		}
		else
		{
			iv.x = (v.x + w / 2) / w / 10 - 1;
		}
		if (v.y + h / 2 >= 0)
		{
			iv.y = (v.y + h / 2) / h / 10;
		}
		else
		{
			iv.y = (v.y + h / 2) / h / 10-1;
		}
		return iv;
	}


    void move()
    {
		
		field.setView(world.player.pos());
		auto viewBox = field.viewBox();
		viewBox.extend(w, h);
		IntVec2 iv2_min,iv2_max;
		iv2_min = Vec2ToIntVec2(viewBox.leftBottom());
		iv2_max = Vec2ToIntVec2(viewBox.rightTop());
		for (int x = iv2_min.x; x <= iv2_max.x; x++)
		{
			for (int y = iv2_min.y; y <= iv2_max.y; y++)
			{
				world.genchunk(IntVec2(x, y));
			}
		}
    }

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
	map<string, Recipe> RDB;
	map<string, Item> DB;
	vector <Slot> slots;

	struct enemy_data
	{

	};

	World world;
	LayerFromDesign(void, you);
	LayerFromDesign(void, enemys);
	LayerFromDesign(void, slot);
	LayerFromDesign(void, slot2);
	LayerFromDesign(void, Player);
	FromDesign(Button, inventor);
	FromDesign(Button, crafts);
	FromDesign(Button, map);
	FromDesign(Button, equiped);
	FromDesign(Button, skill);
	FromDesign(Button, drop);
	FromDesign(Button, items);
	FromDesign(Button, outfit);
	FromDesign(Button, tools);
	FromDesign(Button, furniture);
	FromDesign(Button, material);
	FromDesign(Button, cont);
	FromDesign(Button, back);
	FromDesign(Button, more);
	FromDesign(Button, less);
	FromDesign(Button, Min);
	FromDesign(Button, Max);
	FromDesign(Button, use);
	FromDesign(Button, w1);
	FromDesign(Button, w2);
	FromDesign(Button, p1);
	FromDesign(Button, p2);
	FromDesign(GameView, field);
	FromDesign(Selector, selector);
	FromDesign(Selector, changer);
	FromDesign(Selector, changerCr);
	FromDesign(GameView, fieldInventor);
	FromDesign(GameView, fieldEquiped);
	FromDesign(GameView, fieldMap);
	FromDesign(GameView, fieldSkill);
	FromDesign(GameView, fieldCrafts);
	FromDesign(Layout, cr0);
	FromDesign(Layout, cr1);
	FromDesign(Layout, cr2);
	FromDesign(Layout, cr3);
	FromDesign(Layout, cr4);
	FromDesign(Layout, Menu);
	FromDesign(Layout, Menudr);
	FromDesign(Layout, MenuW);
	FromDesign(Layout, MenuP);
	FromDesign(Layout, stats);
	FromDesign(Label, coldrop);
	
	int nowSlot;
};

int main(int argc, char** argv)
{
    MyApp app;
    app.setConfig("WorldKarikConfig.json");
    app.setDesign("Design.json");
	randomize();
	w = sf::VideoMode::getDesktopMode().width / 10.0;
	h = sf::VideoMode::getDesktopMode().height / 6.0;
	w3 = sf::VideoMode::getDesktopMode().width * 0.11;
	h2 = sf::VideoMode::getDesktopMode().height * 0.19;
	app.setSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	app.setMinSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    if (!app.init(&argc, argv))
        return 1;
    app.run();
    return 0;
}
