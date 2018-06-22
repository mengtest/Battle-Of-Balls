#ifndef _RankScene_H_
#define _RankScene_H_

#include "cocos2d.h"

USING_NS_CC;

class RankScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();   //≥ı ºªØ

	virtual void onExit();

	void menuBackCallback(Ref* pSender);  //∑µªÿ

	CREATE_FUNC(RankScene);
};
#endif
