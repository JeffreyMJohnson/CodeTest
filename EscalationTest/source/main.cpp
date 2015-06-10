#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned short int Ushort;

struct Vector2
{
	float x, y;

	Vector2(){};
	Vector2(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}
};

class Entity
{
public:
	enum Type 
	{
		HEALTH_PICKUP = -1,
		CHEST = 1,
		TRAP = 2,
		TROLL = 3,
		IMP = 4,
		OGRE = 5
	};

	Ushort GetID(){ return mID; }
	Type GetTypeID(){ return mTypeID; }
	float GetXPosition(){ return mPosition.x; }
	float GetYPosition(){ return mPosition.y; }
	float GetXDirection(){ return mDirection.x; }
	float GetYDirection(){ return mDirection.y; }

	Entity(Ushort id, Type type, float positionX, float positionY, float directionX, float directionY)
	{
		mID = id;
		mTypeID = type;
		mPosition = Vector2(positionX, positionY);
		mDirection = Vector2(directionX, directionY);

	}

	void PrintIt()
	{
		cout << "Entity [";
		cout << "ID: " << mID;
		cout << " Type: " << GetTypeName(mTypeID).c_str();
		cout << " Location: (" << mPosition.x << ", " << mPosition.y << ")";
		cout << " Direction: (" << mDirection.x << ", " << mDirection.y << ")";
	}


private:
	Ushort mID;
	Type mTypeID;
	Vector2 mPosition;
	Vector2 mDirection;

	string GetTypeName(Type type)
	{
		switch (type)
		{
		case Type::CHEST:
			return "Chest";
		case Type::HEALTH_PICKUP:
			return "Health Pickup";
		case Type::IMP:
			return "Imp";
		case Type::OGRE:
			return "Ogre";
		case Type::TRAP:
			return "Trap";
		case Type::TROLL:
			return "Troll";
		}
		return "";
	}
};

void LoadFile()
{

}

vector<Entity> entities;

void main()
{
	Entity chest = Entity(-1, Entity::Type::CHEST, 0, 0, 1, 0);
	Entity monster = Entity(1, Entity::Type::OGRE, 10, 10, 0, 1);
	Entity imp = Entity(2, Entity::Type::IMP, 20, 20, 1, 0);
	chest.PrintIt();

	system("pause");

}

