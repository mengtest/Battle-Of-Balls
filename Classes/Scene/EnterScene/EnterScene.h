#ifndef _EnterScene_H_
#define _EnterScene_H_

#include "cocos2d.h"

USING_NS_CC;

class EnterScene : public Layer {
public:
	static Scene * createScene();

	EnterScene();
	~EnterScene();

	virtual bool init();
	CREATE_FUNC(EnterScene);

	virtual void update(float delta);
	virtual void onExit();
private:
	void startLoding();		//��ʼ������Դ
	void enterMenuScene(float dt);		//�����������
private:
	ProgressTimer * _timer;		//�������ؼ�
	Label * _loadingLabel;		//����������ʾ
};

#endif
