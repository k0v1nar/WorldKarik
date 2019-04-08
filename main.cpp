#include <gamebase/Gamebase.h>
#include <SFML/Window/VideoMode.hpp>
#include "world.h"
#include "inventor.h"
#include "fight.h"

using namespace gamebase;
using namespace std;
float w, h, w3, h2,hf;
bool save = false; 
class MyApp : public App
{
	int dropnumcr=1;

	int isMouse = 0;

	int dropnum = 1;

	void icon_del(string i)
	{
		fieldEquiped.child<GameObj>(i).child<Texture>("slot2").show();
		fieldEquiped.child<GameObj>(i).child<Texture>("slot").hide();
	}
	
	void clearQieup()
	{
		if (inventor.slots[inventor.nowSlot].type == weapons)
		{
			inventor.slot.get(inventor.nowSlot).child<Texture>("left").hide();
			inventor.slot.get(inventor.nowSlot).child<Texture>("right").hide();
			if (inventor.slots[inventor.nowSlot].data.weapon.active)
			{
				if (inventor.slots[inventor.nowSlot].data.weapon.isLeft)
				{
					icon_del("weapon1");
					MenuW.hide();
					inventor.slots[inventor.nowSlot].data.weapon.active = false;
					inventor.slots[inventor.nowSlot].data.weapon.isLeft = false;
					inventor.nowSlot = -1;
				}
				else
				{
					icon_del("weapon2");
					MenuW.hide();
					inventor.slots[inventor.nowSlot].data.weapon.active = false;
					inventor.slots[inventor.nowSlot].data.weapon.isLeft = false;
					inventor.nowSlot = -1;
				}
			}
		}
		if (inventor.slots[inventor.nowSlot].type == potions)
		{
			inventor.slot.get(inventor.nowSlot).child<Texture>("left").hide();
			inventor.slot.get(inventor.nowSlot).child<Texture>("right").hide();
			if (inventor.slots[inventor.nowSlot].data.potion.active)
			{
				if (inventor.slots[inventor.nowSlot].data.potion.isLeft)
				{
					icon_del("potion1");
					MenuP.hide();
					inventor.slots[inventor.nowSlot].data.potion.active = false;
					inventor.slots[inventor.nowSlot].data.potion.isLeft = false;
					inventor.nowSlot = -1;
				}
				else
				{
					icon_del("potion2");
					MenuP.hide();
					inventor.slots[inventor.nowSlot].data.potion.active = false;
					inventor.slots[inventor.nowSlot].data.potion.isLeft = false;
					inventor.nowSlot = -1;
				}
			}
		}
		if (inventor.slots[inventor.nowSlot].data.armor.active)
			if (inventor.slots[inventor.nowSlot].type == armors)
			{
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.boots)
				{
					icon_del("boots");
				}
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.chesplat)
				{
					icon_del("chair_armor");
				}
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.helmet)
				{
					icon_del("helmet");
				}
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.leggs)
				{
					icon_del("legss");
				}
				inventor.slots[inventor.nowSlot].data.armor.active = false;
				inventor.nowSlot = -1;
			}
		Menu.hide();
	}
	
	void handuse(bool i)
	{

		if (!inventor.slots[inventor.nowSlot].data.weapon.active)
		{
			if (i)
			{
				icon_use("weapon1", inventor.nowSlot);
				MenuW.hide();
				inventor.slot.get(inventor.nowSlot).child<Texture>("left").show();
				inventor.slots[inventor.nowSlot].data.weapon.active = true;
				inventor.slots[inventor.nowSlot].data.weapon.isLeft = true;
				inventor.nowSlot = -1;
			}
			else
			{
				icon_use("weapon2", inventor.nowSlot);
				MenuW.hide();
				inventor.slot.get(inventor.nowSlot).child<Texture>("right").show();
				inventor.slots[inventor.nowSlot].data.weapon.active = true;
				inventor.slots[inventor.nowSlot].data.weapon.isLeft = false;
				inventor.nowSlot = -1;
			}
		}
	}
	
	void potionuse(bool i)
	{
		if (i)
		{
			icon_use("potion1", inventor.nowSlot);
			MenuP.hide();
			inventor.slots[inventor.nowSlot].data.potion.active = true;
			inventor.slots[inventor.nowSlot].data.potion.isLeft = true;
		}
		else
		{
			icon_use("potion2", inventor.nowSlot);
			MenuP.hide();
			inventor.slots[inventor.nowSlot].data.potion.active = true;
			inventor.slots[inventor.nowSlot].data.potion.isLeft = false;
			
		}
		inventor.updateSlot(inventor.nowSlot);
		inventor.nowSlot = -1;
	}
	
	void icon_use(string i, int i2)
	{
		fieldEquiped.child<GameObj>(i).child<Texture>("slot2").hide();
		fieldEquiped.child<GameObj>(i).child<Texture>("slot").show();
		fieldEquiped.child<GameObj>(i).child<Texture>("slot").setImageName(inventor.DB[inventor.slots[i2].name].file);
	}
	
	void drawQieup()
	{
		if (inventor.slots[inventor.nowSlot].type == weapons)
		{
			connect(w1,handuse, true);
			connect(w2, handuse, false);
			MenuW.show();
		}
		if (inventor.slots[inventor.nowSlot].type == potions)
		{
			connect(p1, potionuse, true);
			connect(p2, potionuse, false);
			MenuP.show();
		}
		if (!inventor.slots[inventor.nowSlot].data.armor.active)
			if (inventor.slots[inventor.nowSlot].type == armors)
			{
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.boots)
				{
					icon_use("boots", inventor.nowSlot);
				}
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.chesplat)
				{
					icon_use("chair_armor", inventor.nowSlot);
				}
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.helmet)
				{
					icon_use("helmet", inventor.nowSlot);
				}
				if (inventor.slots[inventor.nowSlot].data.armor.type == inventor.slots[inventor.nowSlot].data.armor.leggs)
				{
					icon_use("legss", inventor.nowSlot);
				}
				inventor.slots[inventor.nowSlot].data.armor.active = true;
				inventor.nowSlot = -1;
			}
		Menu.hide();
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
		for (auto a : inventor.slot.all())
		{
			if ((inventor.slots[i].data.armor.active && inventor.slots[i].type == armors) || 
				(inventor.slots[i].data.weapon.active && inventor.slots[i].type == weapons) ||
				(inventor.slots[i].data.potion.active && inventor.slots[i].type == potions))
			{
				if (inventor.slots[i].type == weapons && inventor.slots[i].data.weapon.isLeft)
				{
					icon_use("weapon1", i);
				}
				if (inventor.slots[i].type == weapons && !inventor.slots[i].data.weapon.isLeft)
				{
					icon_use("weapon2", i);
				}
				if (inventor.slots[i].type == potions && inventor.slots[i].data.potion.isLeft)
				{
					icon_use("potion1", i);
				}
				if (inventor.slots[i].type == potions && !inventor.slots[i].data.potion.isLeft)
				{
					icon_use("potion2", i);
				}
				if (inventor.slots[i].type == armors)
				{
					if (inventor.slots[i].data.armor.type == inventor.slots[i].data.armor.boots)
					{
						icon_use("boots", i);
					}
					if (inventor.slots[i].data.armor.type == inventor.slots[i].data.armor.chesplat)
					{
						icon_use("chair_armor", i);
					}
					if (inventor.slots[i].data.armor.type == inventor.slots[i].data.armor.helmet)
					{
						icon_use("helmet", i);
					}
					if (inventor.slots[i].data.armor.type == inventor.slots[i].data.armor.leggs)
					{
						icon_use("legss", i);
					}
				}
			}
			i++;
		}
	}

	void recipeLoad()
	{
		for (auto b = 1; b <= 5; b++)
		{
			auto cr = design.child<Layout>("cr" + toString(b - 1));
			cr.child<Layout>("stolb").clear();
		}
		for (auto& b : inventor.RDB)
		{
			auto cr = design.child<Layout>("cr" + toString(b.second.from - 1));
			auto b2 = cr.child<Layout>("stolb").load("recipe.json");
			b2.child<Label>("name").setText(inventor.DB[b.first].name);
			b2.child<Texture>("crpict").setImageName(inventor.DB[b.first].file);
			for (auto& i : b.second.recipe)
			{
				auto bb = b2.child<Layout>("item").load("sl.json");
				bb.child<Texture>("pict").setImageName(inventor.DB[i.first].file);
				bb.child<Label>("col") << " x " << i.second;
			}
			connect(b2.child<Button>("create"), craftItem, b.first);
		}
		design.update();
	}
	
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
			for (auto& b : inventor.RDB[a].recipe)
			{
				inventor.delItem(b.first,dropnumcr*b.second);
			}
			inventor.seekSlot(a, inventor.DB[a].type, dropnumcr);
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
			if (dropnumcr == inventor.recipeCol)
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
			dropnumcr = inventor.recipeCol;
			Min.show();
			less.show();
			Max.hide();
			more.hide();
		}
		coldrop << dropnumcr;
		if (inventor.recipeCol == 1)
		{
			Max.hide();
			more.hide();
		}
	}
	
	void craftItem(string i)
	{
		int min = 99999999;
		for (auto& a : inventor.RDB[i].recipe)
		{
			int c = inventor.seekItem(a.first) / a.second;
			if (c < min)
			{
				min = c;
			}
		}
		inventor.recipeCol = min;
		coldrop << 1;
		Menudr.show();
		less.hide();
		Min.hide();
		Max.show();
		more.show();
		if (inventor.recipeCol == 1)
		{
			less.hide();
			more.hide();
			Min.hide();
			Max.hide();
		}
		if (inventor.recipeCol == 0)
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
	
	void Drop(int i)
	{
		if (inventor.slots[inventor.nowSlot].type == resources || inventor.slots[inventor.nowSlot].type == potions)
		{
			Menudr.show();
			if (inventor.slots[inventor.nowSlot].data.resource.number == 1)
			{
				Max.hide();
				more.hide();
			}
		}
		else
		{
			inventor.slots[inventor.nowSlot].empty = true;
			inventor.updateSlot(inventor.nowSlot);
			updateQuiep();
		}
		Menu.hide();
		Min.hide();
		less.hide();
		if (inventor.slots[inventor.nowSlot].data.resource.number > 1)
		{
			Max.show();
			more.show();
		}
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
	
	void conmenu()
	{
			connect(back, drm, 0);
			connect(cont, drm, 1);
			connect(less, drm, 2);
			connect(more, drm, 3);
			connect(Min, drm, 4);
			connect(Max, drm, 5);
	}
	
	void drm(int i)
	{
		if (i == 0)
		{
			Menudr.hide();
			inventor.nowSlot = -1;
			dropnum = 1;
		}
		if (i == 1)
		{
			Menudr.hide();
			if (inventor.slots[inventor.nowSlot].type == potions)
			{
				inventor.slots[inventor.nowSlot].data.potion.num -= dropnum;
				if (inventor.slots[inventor.nowSlot].data.potion.num == 0)
				{
					inventor.slots[inventor.nowSlot].empty = true;
				}
			}
			if (inventor.slots[inventor.nowSlot].type == resources)
			{
				inventor.slots[inventor.nowSlot].data.resource.number -= dropnum;
				if (inventor.slots[inventor.nowSlot].data.resource.number == 0)
				{
					inventor.slots[inventor.nowSlot].empty = true;
				}
			}
			inventor.updateSlot(inventor.nowSlot);
			design.update();
			Menudr.hide();
			dropnum = 1;
			inventor.nowSlot = -1;
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
			if (dropnum == inventor.slots[inventor.nowSlot].data.resource.number)
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
			dropnum = inventor.slots[inventor.nowSlot].data.resource.number;
			Min.show();
			less.show();
			Max.hide();
			more.hide();
		}
		coldrop << dropnum;
		if (inventor.slots[inventor.nowSlot].data.resource.number == 1)
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

	void nexti(int i)
	{
		changer.select(i);
		if (i == 0)
		{
			for (auto& a : inventor.slots)
			{
				inventor.updateSlot(a.num);
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

	int Vec2ToInt(Vec2 i)
	{
		IntVec2 a;
		a.x = (i.x + w3 / 2) / w3;
		a.y = (i.y + h2 / 2) / h2;
		int b = a.y * 9 + a.x;
		return b;
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
			iv.y = (v.y + h / 2) / h / 10 - 1;
		}
		return iv;
	}

	void load()
	{
		ifstream input("data/save.txt");
		bool s;
		input >> save;
		loadTextBank("textbank.json");
		world.w = w;
		world.h = h;
		world.load(design);
		inventor.load(design, w3, h2);
		fight.load(design, hf);
		Menu.hide();
		
		hideCursor();
		field.setView(world.player.pos());
		connect(equiped, nexti, 1);
		connect(map, nexti, 2);
		connect(crafts, nexti, 4);
		connect(skill, nexti, 3);
		connect(inventors, nexti, 0);
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
		readPerson();
		inventor.seekSlot("wooden_axe",weapons,1);
		inventor.seekSlot("lowheal",potions,15);
	}
	
	Attac attact(bool isLeft)
	{
		int i=0;
		for (auto a : inventor.slot.all())
		{
			if (inventor.slots[i].data.weapon.active && 
				inventor.slots[i].type == weapons)
			{
				if (inventor.slots[i].data.weapon.isLeft == isLeft)
				{
					Attac b;
					b.dam = inventor.slots[i].data.weapon.dam;
					b.TypeD = inventor.slots[i].data.weapon.typeD;
					return b;
				}
			}
			i++;
		}
	}
	void resultFight()
	{
		int d = 0;
		for (auto& c : fight.enemys.all())
		{
			if (!c.isVisible())
			{
				d++;
			}
		}
		if (d == 5)
		{
			selector.select(1);
			for (auto& a : fight.enemys.all())
			{
				string b = fight.enemys.data(a).name;
				string c = fight.enemys.data(a).type;
				for (auto& e : fight.DB[c].enemy[b].drop)
				{
					int f = randomInt(1, 100);
					if (f <= e.second)
					{
						if (inventor.DB[e.first].type == resources)
						inventor.seekSlot(e.first, resources, 1);
						if (inventor.DB[e.first].type == potions)
							inventor.seekSlot(e.first, potions, 1);
					}
				}
			}
			hideCursor();
			world.nowObj = GameObj();
		}
	}
	void process(Input input)
    {
        using namespace gamebase::InputKey;
		if (selector.selected() == 3)
		{
			if (fight.Your)
			{
				if (input.justPressed(MouseLeft))
				{
					auto b = inventor.findWeapon(true);
					if (b.is)
					{
						int a = fight.enemyPosLocat();
						if (a >= 0)
						{
							fight.resultAttact(attact(true), a);
							resultFight();
							fight.enemyAttact();
						}
					}
				}
				if (input.justPressed(MouseRight))
				{
					auto b = inventor.findWeapon(false);
					if (b.is)
					{
						int a = fight.enemyPosLocat();
						if (a >= 0)
						{
							fight.resultAttact(attact(false), a);
							resultFight();
							fight.enemyAttact();
						}
					}
				}
				if (input.justPressed(Q))
				{
					auto b = inventor.findPotion(true);
					if (b.is)
					{
						auto a = inventor.slots[b.i];
						if (a.data.potion.type == a.data.potion.Heal)
						{
							if (fight.dataYou.life + a.data.potion.PowEffects <= 50)
								fight.dataYou.life += a.data.potion.PowEffects;
							else
								fight.dataYou.life = 50;
							fight.updateYou();
							a.data.potion.num--;
							inventor.delItem(a.name, 1);
							design.child<Label>("fpotion1").setText(toString(a.data.potion.num));
							design.update();
							if (a.data.potion.num <= 0)
							{
								design.child<Layout>("potion1f").hide();
							}
						}
						fight.enemyAttact();
					}
				}
				else if (input.justPressed(E))
				{
					auto b = inventor.findPotion(false);
					if (b.is)
					{
						auto a = inventor.slots[b.i];
						if (a.data.potion.type == a.data.potion.Heal)
						{
							if (fight.dataYou.life + a.data.potion.PowEffects <= 50)
								fight.dataYou.life += a.data.potion.PowEffects;
							else
								fight.dataYou.life = 50;
							fight.updateYou();
							a.data.potion.num--;
							design.child<Label>("fpotion2").setText(toString(a.data.potion.num));
							design.update();
							inventor.delItem(a.name, 1);
							if (a.data.potion.num <= 0)
							{
								design.child<Layout>("potion2f").hide();
							}
						}
						fight.enemyAttact();
					}
				}
				else if (fight.nowPos>0 && input.justPressed(A))
				{
					fight.nowPos--;
					fight.updateYou();
					fight.enemyAttact();
				}
				else if (fight.nowPos<4 && input.justPressed(D))
				{
					fight.nowPos++;
					fight.updateYou();
					fight.enemyAttact();
				}
			}
		}
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
						inventor.slot.get(inventor.nowSlot).child<DrawObj>("sel").hide();
						inventor.nowSlot = -1;
						isMouse = 0;
					}
					else
					{
						int a = Vec2ToInt(fieldInventor.mousePos());
						inventor.nowSlot = a;
						if (!inventor.slots[inventor.nowSlot].empty)
						{
							auto b = inventor.slot.get(inventor.nowSlot);
							if (inventor.nowSlot==8|| inventor.nowSlot==17|| inventor.nowSlot==26|| inventor.nowSlot==35|| inventor.nowSlot==44)
							Menu.setPos(b.pos().x - w3, b.pos().y);
							else
								Menu.setPos(b.pos().x + w3, b.pos().y);
							Menu.show();
							Menu.child<Button>("use").show();
							if (inventor.slots[inventor.nowSlot].type == resources)
								Menu.child<Button>("use").hide();
							if ((inventor.slots[inventor.nowSlot].data.armor.active && inventor.slots[inventor.nowSlot].type==armors) || 
								(inventor.slots[inventor.nowSlot].data.weapon.active && inventor.slots[inventor.nowSlot].type == weapons) ||
								(inventor.slots[inventor.nowSlot].data.potion.active && inventor.slots[inventor.nowSlot].type == potions))
							{
								Menu.child<Button>("use").child<Label>("label").setText(tr("drop"));
								connect(use, clearQieup);
							}
							if ((!inventor.slots[inventor.nowSlot].data.armor.active && inventor.slots[inventor.nowSlot].type == armors) ||
								(!inventor.slots[inventor.nowSlot].data.weapon.active && inventor.slots[inventor.nowSlot].type == weapons) ||
								(!inventor.slots[inventor.nowSlot].data.potion.active && inventor.slots[inventor.nowSlot].type == potions))
							{
								Menu.child<Button>("use").child<Label>("label").setText(tr("use"));
								connect(use, drawQieup);
							}
							connect(drop, Drop, inventor.nowSlot);
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
						inventor.nowSlot = Vec2ToInt(fieldInventor.mousePos());
						auto b = inventor.slot.get(inventor.nowSlot);
						b.child<DrawObj>("sel").show();
					}
					else
					{
						isMouse = 0;
						int a = Vec2ToInt(fieldInventor.mousePos());
						auto b2 = inventor.slot.get(inventor.nowSlot);
						b2.child<DrawObj>("sel").hide();
						int a2 = inventor.slots[inventor.nowSlot].num;
						inventor.slots[inventor.nowSlot].num = inventor.slots[a].num;
						inventor.slots[a].num = a2;
						swap(inventor.slots[inventor.nowSlot], inventor.slots[a]);
						inventor.updateSlot(inventor.nowSlot);
						inventor.updateSlot(a);
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
						inventor.nowSlot = -1;
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
								inventor.seekSlot(b2.name, resources, 1);
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
						fight.loadMap(world.Front.data(world.nowObj).type, world.Front.data(world.nowObj).rare);
						obj.kill();
						showCursor();
						world.nowObjInter = false;
						int i=0;
						design.child<Layout>("weapon1f").hide();
						design.child<Layout>("weapon2f").hide();
						design.child<Layout>("potion1f").hide();
						design.child<Layout>("potion2f").hide();
						for (auto a : inventor.slot.all())
						{
							if ((inventor.slots[i].data.armor.active && inventor.slots[i].type == armors) ||
								(inventor.slots[i].data.weapon.active && inventor.slots[i].type == weapons) ||
								(inventor.slots[i].data.potion.active && inventor.slots[i].type == potions))
							{
								if (inventor.slots[i].type == weapons && inventor.slots[i].data.weapon.isLeft)
								{
									design.child<Layout>("weapon1f").show();
									design.child<Layout>("weapon1f").child<Texture>("obj").setImageName(inventor.DB[inventor.slots[i].name].file);
								}
								if (inventor.slots[i].type == weapons && !inventor.slots[i].data.weapon.isLeft)
								{
									design.child<Layout>("weapon2f").show();
									design.child<Layout>("weapon2f").child<Texture>("obj").setImageName(inventor.DB[inventor.slots[i].name].file);
								}
								if (inventor.slots[i].type == potions && inventor.slots[i].data.potion.isLeft)
								{
									design.child<Layout>("potion1f").show();
									design.child<Layout>("potion1f").child<Texture>("obj").setImageName(inventor.DB[inventor.slots[i].name].file);
									design.child<Label>("fpotion1").setText(toString(inventor.slots[i].data.potion.num));
								}
								if (inventor.slots[i].type == potions && !inventor.slots[i].data.potion.isLeft)
								{
									design.child<Layout>("potion2f").show();
									design.child<Layout>("potion2f").child<Texture>("obj").setImageName(inventor.DB[inventor.slots[i].name].file);
									design.child<Label>("fpotion2").setText(toString(inventor.slots[i].data.potion.num));
								}
							}
							i++;
						}
						
					}
				}
			}
		}
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
		if (selector.selected()==3)
		for (auto& a : fight.you.all())
		{
			if (fight.dataYou.life <= 0)
			{
				selector.select(1);
				hideCursor();
				world.nowObj = GameObj();
			}
		}
    }

	World world;
	Inventor inventor;
	Fight fight;

	LayerFromDesign(void, you);
	LayerFromDesign(void, enemys);
	LayerFromDesign(void, slot2);
	LayerFromDesign(void, Player);
	FromDesign(Button, inventors);
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
	FromDesign(GameView, fightField);
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
	
	
};

int main(int argc, char** argv)
{
    MyApp app;
    app.setConfig("WorldKarikConfig.json");
    app.setDesign("Design.json");
	randomize();
	w = sf::VideoMode::getDesktopMode().width / 10.0;
	h = sf::VideoMode::getDesktopMode().height / 6.0;
	hf = sf::VideoMode::getDesktopMode().height / 3;
	w3 = sf::VideoMode::getDesktopMode().width * 0.11;
	h2 = sf::VideoMode::getDesktopMode().height * 0.19;
	app.setSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	app.setMinSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    if (!app.init(&argc, argv))
        return 1;
    app.run();
    return 0;
}
