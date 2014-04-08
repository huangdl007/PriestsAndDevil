#include "Boats.h"
#include "cocos2d.h"

using namespace cocos2d;

Boats* Boats::create(const char* spritePath)
{
	Boats* res=new Boats;
	res->init(spritePath);
	res->autorelease();

	return res;
}

bool Boats::init( const char* spritePath )
{
	CCNode::init();

	ready = false;
	moving = false;
	side = 0;
	LC = RC = NULL;
	CCSprite* sprite = CCSprite::create(spritePath);
	sprite->setAnchorPoint(CCPointZero);
	sprite->setPosition(CCPointZero);
	this->addChild(sprite);

	this->setContentSize(sprite->getContentSize());
	return true;
}

void Boats::putCharacter(Character* C)
{
	CCAssert(hasEmpty(),"not empty for man");
	CCSize contentSize = this->getContentSize();
	C->retain();

	CCPoint preParentPos=C->getPosition();
	CCPoint preWorldPos=C->getParent()->convertToWorldSpace(preParentPos);
	C->removeFromParent();

	CCPoint preLocalPos=this->convertToNodeSpace(preWorldPos);
	C->setPosition(preLocalPos);
	this->addChild(C);
	C->release();

	CCPoint jumpToPos;
	if(LC==NULL)
	{
		jumpToPos=ccp(contentSize.width/3,contentSize.height-10);
		LC = C;
	}
	else
	{
		jumpToPos=ccp(contentSize.width/3*2,contentSize.height-10);
		RC = C;
	}

	CCJumpTo* jumpTo=CCJumpTo::create(1.0,jumpToPos,C->getContentSize().height,1);
	C->runAction(jumpTo);
	ready = true;
}

bool Boats::hasEmpty()
{
	return LC==NULL || RC==NULL;
}

bool Boats::isEmpty()
{
	return ((LC == NULL) && (RC == NULL)); 
}

void Boats::removeCharacter( Character* C )
{
	if(C == LC)
	{
		this->removeChild(LC);
		LC = NULL;
	}
	else if(C == RC)
	{
		this->removeChild(RC);
		RC = NULL;
	}

	if(this->isEmpty())
	{
		ready=false;
	}
}

void Boats::startMove()
{
	moving = true;
}

void Boats::stopMove()
{
	moving = false;
	if(side == 1)
		side = 0;
	else
		side = 1;
}