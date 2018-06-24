#ifndef _GameOverScene_H_
#define _GameOverScene_H_

#include "cocos2d.h"

USING_NS_CC;

class GameOverScene: public Layer {
public:
	static Scene * createScene();

	virtual bool init();
	void menuBackCallback(Ref* pSender);

	CREATE_FUNC(GameOverScene);
};
#endif
