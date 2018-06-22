#ifndef _SettingScene_H_
#define _SettingScene_H_

#include "cocos2d.h"
USING_NS_CC;

class SettingScene : public cocos2d::Scene
{
public:
	static Scene* createScene();

	virtual bool init();

	//menu item callback
	void menuBackCallback(Ref* pSender);
	void menuSoundToggleCallback(Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(SettingScene);
};

#endif
