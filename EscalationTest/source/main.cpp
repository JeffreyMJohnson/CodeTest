#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned short int Ushort;

struct Vector2
{
	float x, y;

	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float a_x, float a_y)
	{
		x = a_x;
		y = a_y;
	}
};

class Entity
{
public:
	enum Type : Ushort
	{
		HEALTH_PICKUP = 0,
		CHEST = 1,
		TRAP = 2,
		TROLL = 3,
		IMP = 4,
		OGRE = 5
	};

	Ushort GetID(){ return mID; }
	Entity::Type GetTypeID(){ return mTypeID; }
	float GetXPosition(){ return mPosition.x; }
	float GetYPosition(){ return mPosition.y; }
	float GetXDirection(){ return mDirection.x; }
	float GetYDirection(){ return mDirection.y; }
	float GetCollisionRadius(){ return mCollisionRadius; }
	float GetFOV(){ return mFOV; }

	Entity(Ushort id, Entity::Type type, float positionX, float positionY, float directionX, float directionY, float collisionRadius)
	{
		mID = id;
		mTypeID = type;
		mPosition = Vector2(positionX, positionY);
		mDirection = Vector2(directionX, directionY);
		mCollisionRadius = collisionRadius;
		mFOV = LoadFOV();
	}

	void PrintIt()
	{
		cout << "Entity [";
		cout << "ID:" << mID;
		cout << " Entity::Type:" << GetTypeName(mTypeID).c_str();
		cout << " Location:(" << mPosition.x << ", " << mPosition.y << ")";
		cout << " Direction:(" << mDirection.x << ", " << mDirection.y << ")";
		cout << " Collision Radius:" << mCollisionRadius;
		cout << " FOV:" << mFOV << "]\n";
	}

	bool IsCollided(Entity& otherEntity)
	{
		//(x2-x1)^2 + (y1-y2)^2 <= (r1+r2)^2
		return (((otherEntity.GetXPosition() - mPosition.x) * (otherEntity.GetXPosition() - mPosition.x)) + ((otherEntity.GetYPosition() - mPosition.y) * (otherEntity.GetYPosition() - mPosition.y))) <
			(mCollisionRadius + GetCollisionRadius()) * (mCollisionRadius + GetCollisionRadius());
	}


private:
	Ushort mID;
	Entity::Type mTypeID;
	Vector2 mPosition;
	Vector2 mDirection;
	float mCollisionRadius;
	float mFOV;

	string GetTypeName(Entity::Type type)
	{
		switch (type)
		{
		case Entity::Type::CHEST:
			return "Chest";
		case Entity::Type::HEALTH_PICKUP:
			return "Health Pickup";
		case Entity::Type::IMP:
			return "Imp";
		case Entity::Type::OGRE:
			return "Ogre";
		case Entity::Type::TRAP:
			return "Trap";
		case Entity::Type::TROLL:
			return "Troll";
		}
		return "";
	}

	float LoadFOV()
	{
		if ((unsigned int)mTypeID > 2)
		{
			switch (mTypeID)
			{
			case Entity::Type::TROLL:
				return 45.0f;
			case Entity::Type::IMP:
				return 65.0f;
			case Entity::Type::OGRE:
				return 90.0f;
			}
			cout << "Could not load FOV from type.";
		}
		return 0.0f;
	}
};

void LoadFile()
{

}

vector<Entity*> entities;

float GetCollisionRadius(Entity::Type entityType)
{
	switch (entityType)
	{
	case Entity::Type::HEALTH_PICKUP:
		return 10;
	case Entity::Type::CHEST:
		return 20;
	case Entity::Type::TRAP:
		return 64;
	case Entity::Type::TROLL:
		return 12;
	case Entity::Type::IMP:
		return 10;
	case Entity::Type::OGRE:
		return 25;
	}
	cout << "Could not get collision radius for entity.\n";
	return 0.0f;

}

void LoadData()
{
	ifstream inFile;
	inFile.open("MapData.bin", ios::in | ios::binary);
	if (!inFile)
	{
		cout << "File didn't load.\n";
		return;
	}
	//cout << "loading entities...\n";
	inFile.seekg(0, ios::beg);
	unsigned int numEntities;

	//get the number of entities
	inFile.read(reinterpret_cast<char*>(&numEntities), sizeof(unsigned int));

	//position now at 4;
	//cout << "position now at: " << inFile.tellg() << endl;

	//cout << "num: " << numEntities << endl;
	//loop through and load entity
	for (unsigned int i = 0; i < numEntities; i++)
	{
		//each entity is 20 bytes of 32 byte structure
		//2 byte
		Ushort id;
		inFile.read(reinterpret_cast<char*>(&id), sizeof(Ushort));
		//2 byte
		Ushort typeId;
		inFile.read(reinterpret_cast<char*>(&typeId), sizeof(Ushort));
		Entity::Type type = static_cast<Entity::Type>(typeId);
		//4 byte X 2
		float posX, posY;
		inFile.read(reinterpret_cast<char*>(&posX), sizeof(float));
		inFile.read(reinterpret_cast<char*>(&posY), sizeof(float));
		//4 byte X 2
		float forwX, forwY;
		inFile.read(reinterpret_cast<char*>(&forwX), sizeof(float));
		inFile.read(reinterpret_cast<char*>(&forwY), sizeof(float));
		entities.push_back(new Entity(id, type, posX, posY, forwX, forwY, GetCollisionRadius(type)));
		//cout << "loaded entity #" << i << endl;
		entities.back()->PrintIt();

		//cout << "position now at: " << inFile.tellg() << endl;
		//advance the 12 bytes (32 byte total - 16 byte for data) to next entity
		//cout << "advancing 12 byte...\n";
		inFile.seekg(12, ios::cur);
	}
	//cout << "done loading...\n";

	inFile.close();
}



void Cleanup()
{
	//delete entities created on heap.
	for (int i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
	entities.clear();
}

void main()
{
	LoadData();

	Cleanup();
	system("pause");

}

