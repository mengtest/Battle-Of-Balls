#ifndef _PlayerScene_H_
#define _PlayerScene_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

class PlayerScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();

	CREATE_FUNC(PlayerScene);

	void menuHomeCallback(Ref * pSender, CheckBox::EventType type);
	void menuPictureCallback(Ref * pSender, CheckBox::EventType type);
	void menuGameCallback(Ref * pSender, CheckBox::EventType type);
	void menuSkinCallback(Ref * pSender, CheckBox::EventType type);
	void menuMessageCallback(Ref * pSender, CheckBox::EventType type);
	void menuReturnCallback(Ref * pSender);
private:
	CheckBox * _homeItem;
	CheckBox * _pictureItem;
	CheckBox * _gameItem;
	CheckBox * _skinItem;
	CheckBox * _messageItem;

	Layer * _homeLayer;
	Layer * _pictureLayer;
	Layer * _playerDataLayer;
	Layer * _playerSkinLayer;
	Layer * _messageLayer;
};
#endif
