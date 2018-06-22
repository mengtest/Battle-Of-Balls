#ifndef _GameScene_H_
#define _GameScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace ui;

class GameScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();

	CREATE_FUNC(GameScene);

	void menuSpitCallback(Ref * pSender, CheckBox::EventType type);
	void menuDivideCallback(Ref * pSender, CheckBox::EventType type);
};
#endif
