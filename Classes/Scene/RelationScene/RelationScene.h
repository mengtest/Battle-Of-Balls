#ifndef _RelationScene_H_
#define _RelationScene_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

class RelationScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();   //≥ı ºªØ

	virtual void onExit();

	void menuBackCallback(Ref* pSender);  //∑µªÿ

	CREATE_FUNC(RelationScene);

};
#endif
