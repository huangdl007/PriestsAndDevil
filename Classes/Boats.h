#ifndef __BOATS_H__
#define __BOATS_H__

#include "Character.h"
#include "cocos2d.h"

using namespace cocos2d;

class Boats : public CCNode
{
public:
	int side;
	Character* LC, *RC;
	bool ready;
	bool moving;

	int getSide(){ return side; }
	bool isReady(){ return ready; }
	Character* getLC(){ return LC; }
	Character* getRC(){ return RC; }
	virtual bool init(const char* spritePath);

	void putCharacter(Character* C);
	void removeCharacter(Character* C);
	bool isEmpty();
	bool hasEmpty();
	void startMove();
	void stopMove();
	bool isMoving(){ return moving; }
	static Boats* create(const char* spritePath);
};

#endif