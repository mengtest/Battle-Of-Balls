#ifndef _TeamScene_H_
#define _TeamScene_H_

#include "cocos2d.h"

USING_NS_CC;

class TeamScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();   //��ʼ��

	virtual void onExit();

	void menuBackCallback(Ref* pSender);  //����

	CREATE_FUNC(TeamScene);
};
#endif
