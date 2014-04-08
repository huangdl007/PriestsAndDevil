#ifndef __GAMEWINSCENE__H
#define __GMAEWINSCENE__H
#include "cocos2d.h"

using namespace cocos2d;

//class HelloWorld;

class GameWinScene:public cocos2d::CCScene
{
public:
	GameWinScene();

	~GameWinScene(){};

	void newGame();

	void onGoItemCallback(CCObject* pSender);
};

#endif