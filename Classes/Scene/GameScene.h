 #ifndef _GameScene_H_
#define _GameScene_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Player;

USING_NS_CC;
using namespace ui;

class GameScene : public Layer {
public:
	static Scene * createScene();  //创建场景

	virtual bool init();  //初始化

	CREATE_FUNC(GameScene);


	void menuSpitCallback(Ref * pSender, CheckBox::EventType type);  //吐孢子回调
	void menuDivideCallback(Ref * pSender, CheckBox::EventType type);  //分身回调

	Player * _player;
};
#endif
