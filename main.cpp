#include <gamebase/Gamebase.h>
#include<SFML/Window/VideoMode.hpp>
using namespace gamebase;
using namespace std;
float w, h, w3, h2;
class MyApp : public App
{
	map<int,string > intToRes;
	map<int, string> intToWeap;
	map< string,int> resToInt;
	map< string, int> weapToInt;
	IntVec2 ch;
	enum TypeIt
	{
		armors,
		weapons,
		resources,
		potions
	};
	enum TypE
	{
		objt,
		fon
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
		bool isRight;
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
		bool isRight;
		int type;
	};

	struct Resource
	{
		int number;
		int type;
		bool full() { return number == 20; };
	};
	struct Slot
	{
		bool empty = true;
		TypeIt type;
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
					if (a.data.resource.type == resToInt[i])
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
					if (b.data.resource.type == resToInt[i])
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
	void readRDB()
	{
		for (auto b = 1; b <= 5; b++)
		{
			ifstream input("data/recipe/recipe"+toString(b)+".txt");
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
	void readDBR()
	{
		ifstream input("data/obj/db.txt");
		for (;;)
		{
			string name;
			input >> name;
			if (name == "end")
			{
				break;
			}
			int a;
			input >> a;
			for (int i = 0; i < a; i++)
			{
				string name2;
				input >> name2;
				int a2;
				input >> a2;
				DBR[name][name2].chance = a2;
				string b;
				input >> b;
				if (b == "fon")
				{
					DBR[name][name2].type = fon;
					string c;
					input >> c;
					DBR[name][name2].file = c;
				}
				if (b == "obj")
				{
					DBR[name][name2].type = objt;
					int c;
					input >> c;
					DBR[name][name2].obect.hp = c;
					string d;
					input >> d;
					DBR[name][name2].file = d;
					input >> c;
					for (int i2 = 0; i2 < c; i2++)
					{
						int c2;
						input >> c2;
						string d2;
						input >> d2;
						Dropp f;
						f.chance = c2;
						f.name = d2;
						DBR[name][name2].obect.drop.push_back(f);
					}
				}
				
			}
			
		}
		input.close();
	}
	void readDB()
	{
		ifstream input("data/item/db.txt");
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
				intToRes[res] = name2;
				resToInt[name2] = res;
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
				intToWeap[res] = name2;
				weapToInt[name2] = res;
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
	bool nowObjInter = false;
    void load()
    {
		readDB();
		readDBR();
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
		auto& nowChunk=createchunk(IntVec2(0, 0));
		nowChunk.map[5][5] = gamer;
		chunkload(nowChunk);
		ch = nowChunk.pos;
		p.x = 5;
		p.y = 5;
		player.setPos((nowChunk.pos.x*10+p.x) * w, (nowChunk.pos.y * 10 + p.y) * h);
		player.setSize(w - 10, h - 10);
		field.setView(player.pos());
		
		genchunk(IntVec2(-1, 0));
		genchunk(IntVec2(1, 0));
		genchunk(IntVec2(0, 1));
		genchunk(IntVec2(0, -1));
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
    }
	void drawQieup()
	{
		if (slots[nowSlot].type == weapons)
		{
			MenuW.show();
		}
		if (slots[nowSlot].type == potions)
		{
			MenuP.show();
		}
		if (slots[nowSlot].type == armors)
		{
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.boots)
			{

			}
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.chesplat)
			{

			}
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.helmet)
			{

			}
			if (slots[nowSlot].data.armor.type == slots[nowSlot].data.armor.leggs)
			{

			}
		}
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
			return;
		}
		b.child<DrawObj>("num").show();
		if (a.type == resources)
		{
			b.child<DrawObj>("num").show();
			b.child<Label>("col").setText(toString(a.data.resource.number));
			b.child<Texture>("obj").setImageName(DB[intToRes[a.data.resource.type]].file);
		}
		if (a.type == weapons)
		{
			b.child<DrawObj>("num").hide();
			b.child<Texture>("obj").setImageName(DB[intToWeap[a.data.weapon.type]].file);
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
	void updateNO(vector<GameObj> objs)
	{
		if (!nowObj || nowObj.pos() != objs.front().pos());
		{
			if (nowObj)
				nowObj.anim.play("lowlight");
			nowObj = objs.front();
			nowObjInter = true;
			objs.front().anim.play("highlight");
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
				if (!a.data.resource.full() && intToRes[a.data.resource.type]== type)
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
						a.data.resource.type = resToInt[type];
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
						a.data.weapon.type = weapToInt[type];
						break;
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
							Menu.setPos(b.pos().x + w3, b.pos().y);
							Menu.show();
							Menu.child<Button>("use").show();
							if (slots[nowSlot].type == resources)
								Menu.child<Button>("use").hide();
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
				player.move(Vec2(-300, 0) * timeDelta());
				auto&objs = roundWorld.find(player);
				if (!objs.empty())
				{
					updateNO(objs);
					player.move(Vec2(300, 0) * timeDelta());
					b++;
				}
				a++;
			}
			if (input.pressed(W))
			{
				player.move(Vec2(0, 300) * timeDelta());
				auto&objs = roundWorld.find(player);
				if (!objs.empty())
				{
					updateNO(objs);
					player.move(Vec2(0, -300) * timeDelta());
					b++;
				}
				a++;
			}
			if (input.pressed(S))
			{
				player.move(Vec2(0, -300) * timeDelta());
				auto&objs = roundWorld.find(player);
				if (!objs.empty())
				{
					updateNO(objs);
					player.move(Vec2(0, 300) * timeDelta());
					b++;
				}
				a++;
			}
			if (input.pressed(D))
			{
				player.move(Vec2(300, 0) * timeDelta());
				auto&objs = roundWorld.find(player);
				if (!objs.empty())
				{
					updateNO(objs);
					player.move(Vec2(-300, 0) * timeDelta());
					b++;
				}
				a++;
			}
			if (a != b && nowObj)
			{
				nowObjInter = false;
				nowObj.anim.play("lowlight");
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
				if (nowObjInter == true)
				{
					roundWorld.data(nowObj).vision -= 25.5;
					auto b = nowObj.skin<Texture>().color();
					b.a = roundWorld.data(nowObj).vision;
					nowObj.skin<Texture>().setColor(b);
					roundWorld.data(nowObj).hp -= 10;
					for (auto& b : DBR["forest"])
					{
						if (b.second.type == fon)
						{
							continue;
						}
						if (roundWorld.data(nowObj).type == toObj[b.first])
						{
							for (auto& b2 : b.second.obect.drop)
							{
								int dd2 = randomInt(1, 100);
								if (dd2 <= b2.chance)
								{
									seekSlot(b2.name, resources,1);
								}
							}
							break;
						}
					}
					if (roundWorld.data(nowObj).hp <= 0)
					{
						auto obj = roundWorld.find(nowObj).front();
						GameObj i;
						nowObj = i;
						chunks[Vec2ToIntVec2(obj.pos())].map[roundWorld.data(obj).thisObj] = None;
						obj.kill();
						nowObjInter = false;
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
		
		field.setView(player.pos());
		auto viewBox = field.viewBox();
		viewBox.extend(w, h);
		IntVec2 iv2_min,iv2_max;
		iv2_min = Vec2ToIntVec2(viewBox.leftBottom());
		iv2_max = Vec2ToIntVec2(viewBox.rightTop());
		for (int x = iv2_min.x; x <= iv2_max.x; x++)
		{
			for (int y = iv2_min.y; y <= iv2_max.y; y++)
			{
				genchunk(IntVec2(x, y));
			}
		}
    }


	enum chunkType {
		Swamp,
		Forest,
		Road,
		SwampRoad,
		ForestRoad,
		SwampForest
	};
	enum chunkObj {
		None,
		Boulder,
		Tree,
		Grass,
		gamer
	};
	map<string, chunkObj> toObj = { {"gras",Grass}, {"stone",Boulder},{"tree",Tree} };
	struct Chunk
	{
		GameMap map;
		IntVec2 pos;
		chunkType Type;
	};
	Chunk& createchunk(IntVec2 i)
	{
		Chunk& chunk = chunks[i];
		chunk.Type = Forest;
		chunk.map = createMap(10, 10);
		chunk.pos = i;
		return chunk;
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
	struct Dropp
	{
		string name;
		int chance;
	};
	struct object
	{
		int hp = 0;
		vector <Dropp> drop;
	};
	
	struct RObj
	{
		string file;
		int chance;
		TypE type;
		object obect;
	};
	struct Recipe
	{
		int col;
		int from;
		map<string, int> recipe;
	};
	map<string, Recipe> RDB;
	map<string, Item> DB;
	map<string, map<string, RObj>>DBR;
	vector <Slot> slots;


	void chunkload(Chunk& chunk)
	{
		auto i = chunk.pos;
		string a;
		if (chunk.Type == Forest)
		{
			a = "forest";
		}
		if (chunk.Type == Swamp)
		{
			a = "swamp";
		}
		for (int x = 0; x < chunk.map.w; x++)
		{
			for (int y = 0; y < chunk.map.h; y++)
			{
				auto& back = Back.load("back.json", (i.x * 10 + x) * w, (i.y * 10 + y) * h);
				back.setSize(w, h);
				back.skin<Texture>().setImageName(DBR[a]["grass"].file);
				if (chunk.map[x][y] == gamer)
					continue;
				int ddd2=randomInt(1,3);
				if (ddd2 == 1)
				{
					int dd3 = 0;
					
					for (auto& b : DBR[a])
					{
						dd3 += b.second.chance;
					}
					int dd2 = randomInt(1, dd3);
					for (auto& b : DBR[a])
					{
						if (b.second.type == fon)
						{
							continue;
						}

						if (dd2 <= b.second.chance)
						{
							chunk.map[x][y] = toObj[b.first];
							auto&obj = roundWorld.load("obj.json", (i.x * 10 + x) * w, (i.y * 10 + y) * h);
							obj.setSize(w - 0.001, h - 0.01);
							roundWorld.data(obj).hp = b.second.obect.hp;
							roundWorld.data(obj).thisObj = IntVec2(x, y);
							roundWorld.data(obj).type = toObj[b.first];
							obj.skin<Texture>().setImageName(b.second.file);
							break;
						}
						else
						{
							dd2 -= b.second.chance;
						}
					}
				}
			}
		}
	}


	Chunk& genchunk(IntVec2 i)
	{
		if (chunks.count(i)==0)
		{
			Chunk& chunk = createchunk(i);
			chunkload(chunk);
			return chunk;
		}
		else
		{
			return chunks[i];
		}
	}


	struct objset
	{
		int hp=50;
		IntVec2 thisObj;
		chunkObj type;
		float vision=255;
	};


	map<IntVec2, Chunk> chunks;
	LayerFromDesign(void, Back);
	LayerFromDesign(void, slot);
	LayerFromDesign(void, slot2);
	LayerFromDesign(objset, roundWorld);
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
	FromDesign(GameObj, player);
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
	FromDesign(Label, coldrop);
	GameObj nowObj;
	IntVec2 p;
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
