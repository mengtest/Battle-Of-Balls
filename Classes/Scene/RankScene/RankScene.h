#ifndef _RankScene_H_
#define _RankScene_H_

#include "cocos2d.h"

USING_NS_CC;

class RankScene : public Layer {
public:
	static Scene * createScene();

	virtual bool init();   //��ʼ��

	virtual void onExit();

	void menuBackCallback(Ref* pSender);  //����

	CREATE_FUNC(RankScene);
};
#endif
