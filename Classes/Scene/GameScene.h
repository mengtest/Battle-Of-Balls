#ifndef _GameScene_H_
#define _GameScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace ui;

class GameScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();  //初始化函数

	CREATE_FUNC(GameScene);

	void menuSpitCallback(Ref * pSender, CheckBox::EventType type);   //吐孢子回调
	void menuDivideCallback(Ref * pSender, CheckBox::EventType type);  //分身回调
};
#endif
