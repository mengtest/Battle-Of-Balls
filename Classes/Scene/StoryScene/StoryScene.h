#ifndef _StoryScene_H_
#define _StoryScene_H_

#include "cocos2d.h"

USING_NS_CC;

class StoryScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();

	CREATE_FUNC(StoryScene);

	bool onTouchBegan(Touch * touch, Event * event);
	void onTouchMoved(Touch * touch, Event * event);
	void onTouchEnded(Touch * touch, Event * event);

	void menuReturnCallback(Ref * pSender);
	void menuNextCallback(Ref * pSender);
	void menuPreviousCallback(Ref * pSender);

	void createStorySprite();
private:
	MenuItemImage * _nextItem;
	MenuItemImage * _previousItem;
	int _selectedSprite;
	Vec2 _startPoint;
};
#endif
