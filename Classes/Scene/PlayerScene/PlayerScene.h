#ifndef _PlayerScene_H_
#define _PlayerScene_H_

#include "cocos2d.h"
USING_NS_CC;

class PlayerScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();   //��ʼ��

	virtual void onExit();

	void menuBackCallback(Ref* pSender);  //����

	CREATE_FUNC(PlayerScene);

};
#endif
