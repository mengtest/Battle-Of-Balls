#ifndef _StoreScene_H_
#define _StoreScene_H_

#include "cocos2d.h"
USING_NS_CC;

class StoreScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();   //≥ı ºªØ

	virtual void onExit();

	void menuBackCallback(Ref* pSender);  //∑µªÿ

	CREATE_FUNC(StoreScene);

};
#endif

