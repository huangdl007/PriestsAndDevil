#ifndef __HINT_H__
#define __HINT_H__
#include "cocos2d.h"
#include "map"
#include "string"

using namespace std;

USING_NS_CC;

class Hint
{
public:
	static Hint *hintInstance;
	static Hint *getInstance();

	CCString* getHint(string curState);

	Hint();
	~Hint();

private:
	map<string, int> states1;
	multimap<int, string> states2;
	void stringToInt(string s, int a[]);
};

#endif