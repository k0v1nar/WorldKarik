#pragma once

#include <gamebase/Gamebase.h>

using namespace gamebase;
using namespace std;

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
	Obj,
	gamer,
	Enemy
};

enum TypE
{
	obj,
	fon,
	enemy
};

struct Chunk
{
	GameMap map;
	IntVec2 pos;
	chunkType Type;
};

struct objset
{
	int hp = 50;
	IntVec2 thisObj;
	string type;
	chunkObj typ;
	float vision = 255;
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

class World {
public:

	// load all
	void load(Layout design)
	{
		readDB();
		Back = design.child<Layer<void>>("Back");
		Front = design.child<Layer<objset>>("Front");
		auto& newChunk = createchunk(IntVec2(0, 0));
		newChunk.map[5][5] = gamer;
		chunkload(newChunk);
		genchunk(IntVec2(-1, 0));
		genchunk(IntVec2(1, 0));
		genchunk(IntVec2(0, 1));
		genchunk(IntVec2(0, -1));
		nowChunk = newChunk.pos;
		player = design.child<GameObj>("player");
		p.x = 5;
		p.y = 5;
		player.setPos(p.x * w, p.y * h);
		player.setSize(w - 10, h - 10);
	}
	void readDB()
	{
		ifstream input("data/load/obj/db.txt");
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
				DB[name][name2].chance = a2;
				string b;
				input >> b;
				if (b == "fon")
				{
					DB[name][name2].type = fon;
					string c;
					input >> c;
					DB[name][name2].file = c;
				}
				if (b == "obj")
				{
					DB[name][name2].type = obj;
					int c;
					input >> c;
					DB[name][name2].obect.hp = c;
					string d;
					input >> d;
					DB[name][name2].file = d;
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
						DB[name][name2].obect.drop.push_back(f);
					}
				}
				if (b == "enemy")
				{
					DB[name][name2].type = enemy;
					string c;
					input >> c;
					DB[name][name2].file = c;
				}
			}

		}
		input.close();
	}
	
	//world work
	void updateNO(vector<GameObj> objs)
	{
		if (!nowObjInter || nowObj.pos() != objs.front().pos());
		{
			nowObj = objs.front();
			nowObjInter = true;
			objs.front().anim.play("highlight");
		}
	}


	//generation Chunk
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
				back.skin<Texture>().setImageName(DB[a]["grass"].file);
				if (chunk.map[x][y] == gamer)
					continue;
				int ddd2 = randomInt(1, 3);
				if (ddd2 == 1)
				{
					int dd3 = 0;

					for (auto& b : DB[a])
					{
						dd3 += b.second.chance;
					}
					int dd2 = randomInt(1, dd3);
					for (auto& b : DB[a])
					{
						if (b.second.type == fon)
						{
							continue;
						}
						if (dd2 <= b.second.chance)
						{
							if (b.second.type == obj)
							{
								chunk.map[x][y] = Obj;
								auto&object = Front.load("obj.json", (i.x * 10 + x) * w, (i.y * 10 + y) * h);
								object.setSize(w - 0.001, h - 0.01);
								Front.data(object).hp = b.second.obect.hp;
								Front.data(object).thisObj = IntVec2(x, y);
								Front.data(object).type = b.first;
								Front.data(object).typ = Obj;
								object.skin<Texture>().setImageName(b.second.file);
								break;
							}
							if (b.second.type == enemy)
							{
								chunk.map[x][y] = Enemy;
								auto&object = Front.load("obj.json", (i.x * 10 + x) * w, (i.y * 10 + y) * h);
								object.setSize(w - 0.001, h - 0.01);
								Front.data(object).thisObj = IntVec2(x, y);
								Front.data(object).type = b.first;
								Front.data(object).typ = Enemy;
								object.skin<Texture>().setImageName(b.second.file);
								break;
							}
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
		if (chunks.count(i) == 0)
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
	Chunk& createchunk(IntVec2 i)
	{
		Chunk& chunk = chunks[i];
		chunk.Type = Forest;
		chunk.map = createMap(10, 10);
		chunk.pos = i;
		return chunk;
	}
	
	//members
	IntVec2 nowChunk;
	map<string, map<string, RObj>> DB;
	map<IntVec2, Chunk> chunks;
	Layer<void> Back;
	Layer<objset> Front;
	GameObj player;
	IntVec2 p;
	GameObj nowObj;
	bool nowObjInter = false;
	float w, h;
};
