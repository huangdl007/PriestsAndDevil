#include "Hint.h"


Hint::Hint()
{
	//<string, int>
	states1.insert(make_pair("00132", 0));
	states1.insert(make_pair("10033", 1));
	states1.insert(make_pair("00231", 2));
	states1.insert(make_pair("01122", 2));
	states1.insert(make_pair("10132", 3));
	states1.insert(make_pair("00330", 4));
	states1.insert(make_pair("10231", 5));
	states1.insert(make_pair("02211", 6));
	states1.insert(make_pair("11122", 7));
	states1.insert(make_pair("03102", 8));
	states1.insert(make_pair("13003", 9));
	states1.insert(make_pair("03201", 10));
	states1.insert(make_pair("12211", 11));
	states1.insert(make_pair("13102", 11));
	states1.insert(make_pair("03300", 12));

	//<int, string>
	states2.insert(make_pair(0, "00132"));
	states2.insert(make_pair(1, "10033"));
	states2.insert(make_pair(2, "00231"));
	states2.insert(make_pair(2, "01122"));
	states2.insert(make_pair(3, "10132"));
	states2.insert(make_pair(4, "00330"));
	states2.insert(make_pair(5, "10231"));
	states2.insert(make_pair(6, "02211"));
	states2.insert(make_pair(7, "11122"));
	states2.insert(make_pair(8, "03102"));
	states2.insert(make_pair(9, "13003"));
	states2.insert(make_pair(10, "03201"));
	states2.insert(make_pair(11, "12211"));
	states2.insert(make_pair(11, "13102"));
	states2.insert(make_pair(12, "03300"));
}


Hint::~Hint()
{
}

Hint* Hint::getInstance()
{
	if(!hintInstance) {
		hintInstance = new Hint();
	}

	return hintInstance;
}

void Hint::stringToInt(string s, int a[])
{
	if (s.size() > 5)
	{
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		a[i] = s[i] - '0';
	}
}

CCString* Hint::getHint(string curState)
{
	int curStateArr[5], nextStateArr[5];
	int cur;

	if(states1.find(curState) == states1.end())
		return CCString::create("");

	cur = states1[curState];

	if(cur == 12)
		return CCString::create("You win!!!");

	if(cur >= 12)
		return CCString::create("");

	stringToInt(curState, curStateArr);
	stringToInt(states2.find(cur+1)->second, nextStateArr);

	int numOfPriests = 0, numOfDevils = 0;

	if (curStateArr[0] == 0)
	{
		numOfPriests = nextStateArr[3] - curStateArr[3];
		numOfDevils = nextStateArr[4] - curStateArr[4];
	} 
	else
	{
		numOfPriests = nextStateArr[1] - curStateArr[1];
		numOfDevils = nextStateArr[2] - curStateArr[2];
	}

	return CCString::createWithFormat("Move (%d Priest(s), %d Devil(s) to the %s bank", 
										numOfPriests, numOfDevils,
									  curStateArr[0] == 0 ? "right" : "left");
}

Hint * Hint::hintInstance = NULL;
