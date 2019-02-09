#include <gamebase/Gamebase.h>
#include<SFML/Window/VideoMode.hpp>
using namespace gamebase;
using namespace std;
float w, h, w2, h2;
class MyApp : public App
{
	vector<string> resnames = {
		"wood.png",
		"stone.png"
	};
	IntVec2 ch;
	enum TypeRes
	{
		Wood,
		Stone
	};
	bool nowObjInter = false;
    void load()
    {
		Menu.hide();
		int i = 0;
		for (int y = 0; y < 5; y++)
			for (int x=0;x<9;x++)
			{
				Slot sl;
				sl.num = i;
				slots.push_back(sl);
				auto& slo = slot.load("slot.json", x*w2, y*h2);
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
    }
	void nexti(int i)
	{
		changer.select(i);
		if (i == 0)
		{
			for (auto& a : slots)
			{
				if (a.empty)
				{
					continue;
				}
				for (auto& b : slot.all())
				{
					if (b.id() == a.num)
					{
						b.child<DrawObj>("num").show();
						if (a.type == Slot::resources)
						{
							b.child<Label>("col").setText(toString(a.data.resource.number));
							b.child<Texture>("obj").setImageName(resnames[a.data.resource.type]);
						}
						b.child<Texture>("obj").show();
					}
				}
			}
			design.update();
			fieldInventor.setView(4 * w2, 2 * h2);
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
	void seekSlot(TypeRes type)
	{
		int b=1;
		for (auto& a : slots)
		{
			if (a.type == Slot::resources)
			{
				if (!a.data.resource.full() && a.data.resource.type==type)
				{
					a.data.resource.number++;
					b--;
					break;
				}
			}
		}
		if (b > 0)
		{
			for (auto& a : slots)
			{
				if (a.empty)
				{
					a.type = Slot::resources;
					a.data.resource.type = type;
					a.data.resource.number=1;
					a.empty = false;
					break;
				}
			}
		}
	}


	int Vec2ToInt(Vec2 i)
	{
		IntVec2 a;
		a.x = (i.x + w2 / 2) / w2;
		a.y = (i.y + h2 / 2) / h2;
		int b = a.y * 9 + a.x;
		return b;
	}


    void process(Input input)
    {
        using namespace gamebase::InputKey;
		
		if (selector.selected() == 2)
		{
			if (input.justPressed(Escape))
			{
				selector.select(1);
				hideCursor();
				return;
			}
			if (input.justPressed(MouseRight))
			{
				int a = Vec2ToInt(fieldInventor.mousePos());
				nowSlot = a;
				if (!slots[nowSlot].empty)
				{
					for (auto b : slot.all())
					{
						if (nowSlot == b.id())
							Menu.setPos(b.pos().x + w2, b.pos().y);
					}
					Menu.show();
				}
				design.update();
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
					if (roundWorld.data(nowObj).type == Tree)
					{
						seekSlot(Wood);
					}
					if (roundWorld.data(nowObj).type == Boulder)
					{
						seekSlot(Stone);
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
		gamer
	};
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
		int dam, level;
		TypeD typeD;
		Rarity rarity;
		bool active;
		bool isLeft;
		bool isRight;
	};

	struct Resource
	{
		
		int number;
		TypeRes type;
		bool full() { return number == 20; };
	};
	struct Slot
	{
		bool empty;
		enum Type
		{
			armors,
			weapons,
			resources,
			potions
		};
		Type type;
		union 
		{
			Armor armor;
			Weapon weapon;
			Resource resource;
			Potion potion;
		} data;
		int x, y,num;
	};


	/*struct A
	{
		int x, y;
	};

	struct B
	{
		float arr[10];
	};

	struct D
	{
		enum Type
		{
			TypeA,
			TypeB,
			TypeC
		};
		Type type;

		union
		{
			A a;
			B b;
			double c;
		} data;
	};*/


	vector <Slot> slots;


	void chunkload(Chunk& chunk)
	{
		auto i = chunk.pos;
		for (int x = 0; x < chunk.map.w; x++)
		{
			for (int y = 0; y < chunk.map.h; y++)
			{
				auto& back = Back.load("back.json", (i.x * 10 + x) * w, (i.y * 10 + y) * h);
				back.setSize(w, h);
				if (chunk.Type == Forest)
					back.skin<Texture>().setImageName("grass1.png");
				if (chunk.Type == Swamp)
					back.skin<Texture>().setImageName("grass2.png");
				if (chunk.map[x][y] == gamer)
					continue;
				int obt = randomInt(1, 30);
				if (obt == 1 || obt == 2 || obt == 3)
				{
					chunk.map[x][y] = Tree;
					auto&obj = roundWorld.load("obj.json", (i.x * 10 + x) * w, (i.y * 10 + y) * h);
					obj.setSize(w-0.001, h-0.01);
					if (chunk.Type == Forest)
						obj.skin<Texture>().setImageName("wood1.png");
					if (chunk.Type == Swamp)
						obj.skin<Texture>().setImageName("wood2.png");
					roundWorld.data(obj).thisObj = IntVec2(x, y);
					roundWorld.data(obj).type = Tree;
				}
				if (obt == 4 || obt == 5)
				{
					chunk.map[x][y] = Boulder;
					auto&obj = roundWorld.load("obj.json", (i.x * 10 + x) * w, (i.y * 10 + y) * h);
					roundWorld.data(obj).hp = 100;
					obj.setSize(w-0.01, h-0.01);
					obj.skin<Texture>().setImageName("boulder.png");
					roundWorld.data(obj).thisObj = IntVec2(x, y);
					roundWorld.data(obj).type = Boulder;
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
	FromDesign(GameView, field);
	FromDesign(GameObj, player);
	FromDesign(Selector, selector);
	FromDesign(Selector, changer);
	FromDesign(GameView, fieldInventor);
	FromDesign(GameView, fieldEquiped);
	FromDesign(GameView, fieldMap);
	FromDesign(GameView, fieldSkill);
	FromDesign(GameView, fieldCrafts);
	FromDesign(Layout, Menu);
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
	w2 = sf::VideoMode::getDesktopMode().width * 0.11;
	h2 = sf::VideoMode::getDesktopMode().height * 0.19;
	app.setSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	app.setMinSize(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
    if (!app.init(&argc, argv))
        return 1;
    app.run();
    return 0;
}
