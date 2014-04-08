#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "GameWinScene.h"
USING_NS_CC;
using namespace cocos2d;
using namespace CocosDenshion;

bool HelloWorld::isFirstLoad = true;

//get the position of character
static float getPos(Character* C, float landWidth, bool onRight)
{
	int index = C->getIndex();
	float PosX = index*15 + 2;

	if(onRight)
	{
		return PosX;
	}
	else
	{
		return landWidth-40 - PosX;
	}
}

static void checkAndCollectMan(Character* C,CCArray* priests,CCArray* devils)
{
	if(C != NULL)
	{
		if(C->getType() == 1)
		{
			devils->addObject(C);
		}
		else
		{
			priests->addObject(C);
		}
	}
}

static void checkCharacter(Character* C, CCArray* priests, CCArray* devils)
{
	if(C != NULL)
	{
		if(C->getType() == 0)
		{
			priests->addObject(C);
		}
		else
		{
			devils->addObject(C);
		}
	}
}

HelloWorld::HelloWorld()
{
	lCharacter = NULL;
	rCharacter = NULL;
	l_sprite = NULL;
	r_sprite = NULL;
	boat = NULL;
	currentTime = 60;
}

HelloWorld::~HelloWorld(void)
{
	CC_SAFE_RELEASE(boat);
	CC_SAFE_RELEASE(lCharacter);
	CC_SAFE_RELEASE(rCharacter);
	CC_SAFE_RELEASE(l_sprite);
	CC_SAFE_RELEASE(r_sprite);
}

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
	CCLayer::init();
	CCSize ws = CCDirector::sharedDirector()->getWinSize();

	//Menu
	CCMenuItem* goItem=CCMenuItemImage::create("go.png", "go.png",this, 
						menu_selector(HelloWorld::onGoItemCallback));
	goItem->setPosition(ccp(240,200));
	CCMenu* menu=CCMenu::create(goItem,NULL);
	menu->setAnchorPoint(CCPointZero);
	menu->setPosition(CCPointZero);
	this->addChild(menu, 1000);

	// Hint
	CCMenuItemImage *hintItem = CCMenuItemImage::create(
		"hint.png",
		"hint.png",
		this,
		menu_selector(HelloWorld::menuHintCallback));
	hintItem->setScale(0.20);
	hintItem->setPosition(ccp(240, 290) );

	CCMenu *hintMenu = CCMenu::create(hintItem, NULL);
	hintMenu->setPosition(CCPointZero);
	this->addChild(hintMenu, 1000);
	pHint = NULL;

	//timer
	labelatlas = CCLabelAtlas::create("60", "fps_images.png", 16, 32, 46);  
	labelatlas->setPosition(ccp(ws.width/10*9, ws.height-40));  
	this->addChild(labelatlas);  
	labelatlas->setScale(1.0f);
	schedule(schedule_selector(HelloWorld::updateTimer), 1.0f); 

	//background
	CCLayerColor* bg = CCLayerGradient::create(ccc4(0, 0, 0, 0),ccc4(0, 0, 0, 0), ccp(0, 1));
	bg->setAnchorPoint(CCPointZero);
	bg->setPosition(CCPointZero);
	this->addChild(bg,0);

	//wave
	CCSprite* wave = CCSprite::create("wave.png");
	wave->setAnchorPoint(ccp(0, 0));
	wave->setPosition(ccp(110, -120));
	this->addChild(wave, 3);

	//land
	l_sprite = CCSprite::create("rland.png");
	l_sprite->retain();
	l_sprite->setPosition(ccp(40, 30));
	this->addChild(l_sprite, 4);

	r_sprite = CCSprite::create("lland.png");
	r_sprite->retain();
	r_sprite->setPosition(ccp(440, 30));
	this->addChild(r_sprite, 4);

	//Boat
	boat = Boats::create("boat.png");
	boat->retain();
	boat->setAnchorPoint(ccp(0, 0));
	boat->setPosition(ccp(230, 15));
	this->addChild(boat,2);
	r_pos = ccp(230,15);
	l_pos = ccp(120,15);

	//wave action
	CCSize landSize = l_sprite->getContentSize();
	CCMoveBy* moveBy=CCMoveBy::create(2,ccp(-10, 0));
	CCMoveBy* moveBack=(CCMoveBy*)moveBy->reverse();
	CCSequence* seq=CCSequence::create(moveBy,moveBack,NULL);
	wave->runAction(CCRepeatForever::create(seq));

	//create Character
	rCharacter = CCArray::create();
	rCharacter->retain();
	for(int i = 0; i < 3; i++)
	{
		Character* priest = Character::create("priest.png", 0);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		priest->setPosition(ccp(winSize.width-131 + 17*i, winSize.height-250));
		priest->setIndex(i);
		this->addChild(priest, i+5);
		rCharacter->addObject(priest);
		priest->retain();
	}
	for(int i = 0; i < 3; i++)
	{
		Character* devil = Character::create("devil.png", 1);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		devil->setPosition(ccp(winSize.width-80 + 17*i, winSize.height-250));
		devil->setIndex(i+3);
		this->addChild(devil, i+8);
		rCharacter->addObject(devil);
		devil->retain();
	}
	lCharacter = CCArray::create();
	lCharacter->retain();

	this->setTouchEnabled(true);
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, getTouchPriority(), true);

	//Background Music
	if(isFirstLoad)
	{
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic( "backgroundmusic.mp3" );
		SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("backgroundmusic.mp3", true);
		isFirstLoad = false;
	}
	
    return true;
}

bool HelloWorld::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCPoint worldPoint = touch->getLocation();

	if(boat->isMoving())
	{
		return false;
	}

	//find which character was clicked
	//try to put it onto the boat
	if(boat->getSide() == 0)
	{
		for(int i = 0;i < rCharacter->count();i++)
		{
			Character* character = (Character*)rCharacter->objectAtIndex(i);
			if(character->isClickOnMe(worldPoint) && boat->hasEmpty())
			{
				boat->putCharacter(character);
				rCharacter->removeObject(character);
				/*CCLOG("r :%d", rCharacter->count());
				CCLOG("l :%d", lCharacter->count());*/
				return false;
			}
		}
	}
	else
	{
		for(int i = 0;i < lCharacter->count();i++)
		{
			Character* character = (Character*)lCharacter->objectAtIndex(i);
			if(character->isClickOnMe(worldPoint) && boat->hasEmpty())
			{
				boat->putCharacter(character);
				lCharacter->removeObject(character);
				/*CCLOG("r :%d", rCharacter->count());
				CCLOG("l :%d", lCharacter->count());*/
				return false;
			}
		}
	}

	Character* leftCharacter = boat->getLC();
	Character* rightCharacter = boat->getRC();

	//remove the clicked character from boat
	if(leftCharacter && leftCharacter->isClickOnMe(worldPoint))
	{
		JumpOut(leftCharacter);
	}
	if(rightCharacter && rightCharacter->isClickOnMe(worldPoint))
	{
		JumpOut(rightCharacter);
	}
	CCLOG("l :%d", lCharacter->count());
	CCLOG("r :%d", rCharacter->count());
	return false;
}

void HelloWorld::JumpOut(Character* C)
{
	C->retain();

	//C's worldPos
	CCPoint preWorldPos = C->getParent()->convertToWorldSpace(C->getPosition());
	CCPoint prePosInThis = this->convertToNodeSpace(preWorldPos);
	bool onRight = boat->getSide() == 0;
	CCSize landSize = r_sprite->getContentSize();

	float landPointX = getPos(C, landSize.width, onRight);
	CCPoint landPosInWorld;
	if(onRight)
	{
		landPosInWorld = l_sprite->convertToWorldSpace(ccp(landPointX+415, landSize.height-9));
		rCharacter->addObject(C);
	}
	else
	{
		landPosInWorld = r_sprite->convertToWorldSpace(ccp(landPointX-400, landSize.height));
		lCharacter->addObject(C);
	}

	//remove the clicked character
	boat->removeCharacter(C);
	C->setPosition(prePosInThis);
	this->addChild(C, C->getIndex()+5);

	//jump action
	CCPoint landPosInThis = this->convertToNodeSpace(ccp(landPosInWorld.x, landPosInWorld.y-22));
	CCJumpTo* jumpTo=CCJumpTo::create(1.0,landPosInThis,C->getContentSize().height,1);
	C->runAction(jumpTo);

	//judge the game state
	if(!onRight)
	{
		if(lCharacter->count() == 6)
		{
			CCScene* gameWin = new GameWinScene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, gameWin));
		}

	}

	C->release();

}

//go menuitem action
void HelloWorld::onGoItemCallback( CCObject* pSender )
{
	//judge the state of boat
	if(boat->isReady() && !boat->isMoving())
	{
		boat->startMove();
		CCPoint targetPos;
		if(boat->getSide()== 0)
		{
			targetPos = l_pos;
		}
		else
		{
			targetPos =r_pos;
		}

		//move action
		CCMoveTo* moveTo=CCMoveTo::create(1.0,targetPos);
		CCCallFunc* callFunc=CCCallFunc::create(this,callfunc_selector(HelloWorld::onBoatReachLand));
		boat->runAction(CCSequence::create(moveTo,callFunc,NULL));

		//judge the state of game
		bool isPriestOK;
		int side=boat->getSide();
		if(side == 0)
		{
			isPriestOK=checkRightLandIsOK();
		}
		else
		{
			isPriestOK=checkLeftLandIsOK();
		}
		if(!isPriestOK)
		{
			CCScene* gameOver = new GameOverScene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, gameOver));
			//CCLOG ("%s \n", "Failed");
		}
	}
		/*CCLOG("l :%d", lCharacter->count());
	CCLOG("r :%d", rCharacter->count());*/
}

void HelloWorld::onBoatReachLand()
{
	bool isPriestOK;
	boat->stopMove();
	int side=boat->getSide();
	if(side == 0)
	{
		isPriestOK=checkRightLandIsOK();
	}
	else
	{
		isPriestOK=checkLeftLandIsOK();
	}
	if(!isPriestOK)
	{
		CCScene* gameOver = new GameOverScene();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, gameOver));
		//CCLOG ("%s \n", "Failed");
	}
}

bool HelloWorld::checkRightLandIsOK()
{
	CCArray* priests=CCArray::create();
	CCArray* devils=CCArray::create();

	collectRightLandMan(priests,devils);

	if(priests->count()>0&&devils->count()>priests->count())
	{
		return false;
	}
	return true;
}

bool HelloWorld::checkLeftLandIsOK()
{
	CCArray* priests=CCArray::create();
	CCArray* devils=CCArray::create();

	collectLeftLandMan(priests,devils);

	if(priests->count()>0&&devils->count()>priests->count())
	{
		return false;
	}
	return true;
}

void HelloWorld::collectRightLandMan( CCArray* priestArr,CCArray* devilArr )
{
	for(int i = 0; i < rCharacter->count(); i++)
	{
		Character* character=(Character*)rCharacter->objectAtIndex(i);
		checkAndCollectMan(character,priestArr,devilArr);
	}
	if(!boat->isMoving()&&boat->getSide() == 0)
	{
		checkAndCollectMan(boat->getLC(),priestArr,devilArr);
		checkAndCollectMan(boat->getRC(),priestArr,devilArr);
	}
}

void HelloWorld::collectLeftLandMan( CCArray* priestArr,CCArray* devilArr )
{
	for(int i = 0; i < lCharacter->count(); i++)
	{
		Character* character=(Character*)lCharacter->objectAtIndex(i);
		checkAndCollectMan(character,priestArr,devilArr);
	}
	if(!boat->isMoving()&&boat->getSide() == 1)
	{
		checkAndCollectMan(boat->getLC(),priestArr,devilArr);
		checkAndCollectMan(boat->getRC(),priestArr,devilArr);
	}
}

void HelloWorld::updateTimer(float dt)
{
	checkTime();
	char buffer[20];
	sprintf(buffer, "%d", currentTime);
	labelatlas->setString(buffer);
	currentTime--;
}

void HelloWorld::checkTime()
{
	if(currentTime == 0)
	{
		if(rCharacter->count() != 6)
		{
			CCScene* gameOver = new GameOverScene();
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2, gameOver));
		}
	}
}

void HelloWorld::menuHintCallback(CCObject* pSender)
{
	if(!boat->isMoving())
	{
		string curstate = "";
		int ld = 0, lp = 0, rd= 0, rp = 0;

		float midX = CCDirector::sharedDirector()->getVisibleSize().width / 2.5;

		if (boat->getPositionX() < midX)
		{
			curstate += '0';
			if (boat->LC != NULL)
			{
				if (((Character*)boat->LC)->getType() == 1)
				{
					ld++;
				} 
				else
				{
					lp++;
				}
			}

			if (boat->RC != NULL)
			{
				if (((Character*)boat->RC)->getType() == 1)
				{
					ld++;
				} 
				else
				{
					lp++;
				}
			}
		}
		else
		{
			curstate += '1';
			if (boat->LC != NULL)
			{
				if (((Character*)boat->LC)->getType() == 1)
				{
					rd++;
				} 
				else
				{
					rp++;
				}
			}

			if (boat->RC != NULL)
			{
				if (((Character*)boat->RC)->getType() == 1)
				{
					rd++;
				} 
				else
				{
					rp++;
				}
			}
		}

		for (int i = 0; i < lCharacter->count(); i++)
		{
			if (((Character*)lCharacter->objectAtIndex(i))->getType() == 1)
			{
				ld++;
			} 
			else
			{
				lp++;
			}
		}

		for (int i = 0; i < rCharacter->count(); i++)
		{
			if (((Character*)rCharacter->objectAtIndex(i))->getType() == 1)
			{
				rd++;
			} 
			else
			{
				rp++;
			}
		}
		CCLOG("L_D%d", ld);
		CCLOG("L_P%d", lp);
		CCLOG("R_D%d", rd);
		CCLOG("R_P%d", rp);
		//		CCLOG("%.2f  %.2f", boat->getPositionX(), midX);
		curstate += '0' + lp;
		curstate += '0' + ld;
		curstate += '0' + rp;
		curstate += '0' + rd;
		CCLog(&curstate[0]);

		CCLog(Hint::getInstance()->getHint(curstate)->getCString());

		if (pHint != NULL)
		{
			pHint->removeFromParent();
			pHint = NULL;
		}
		pHint = CCLabelTTF::create(Hint::getInstance()->getHint(curstate)->getCString(), "Thonburi", 20);
		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
		pHint->setPosition(ccp(size.width / 2, size.height - 70));
		this->addChild(pHint);
	}
}