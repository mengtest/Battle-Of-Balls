#ifndef _PlayerScene_H_
#define _PlayerScene_H_

#include "cocos2d.h"
USING_NS_CC;

class PlayerScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();   //初始化

	virtual void onExit();

	void menuBackCallback(Ref* pSender);  //返回

	CREATE_FUNC(PlayerScene);

};
#endif
