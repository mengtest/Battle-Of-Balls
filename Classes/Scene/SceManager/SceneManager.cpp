#include "SceneManager.h"
#include "EnterScene.h"
#include "MenuScene.h"
#include "GameScene.h"

SceneManager * SceneManager::s_SceneManager = NULL;

SceneManager* SceneManager::getInstance()  //��������������
{
	if (s_SceneManager == NULL)
	{
		s_SceneManager = new SceneManager();
		if (s_SceneManager && s_SceneManager->init())
		{
			s_SceneManager->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(s_SceneManager);
			s_SceneManager = NULL;
		}
	}
	return s_SceneManager;
}

bool SceneManager::init()  ��ʼ��
{
	return true;
}

void SceneManager::changeScene(SceneType enSceneType)  //�任����
{
	Scene * scene = NULL;
	TransitionScene * ccts = NULL;

	switch (enSceneType)
	{
	case en_EnterScene:
		scene = EnterScene::createScene();
		break;
	case en_MenuScene:
		scene = MenuScene::createScene();
		break;
	/*case en_GameScene:
		scene = GameScene::createScene();
		break;
	case en_PlayerScene:
		scene = PlayerScene::createScene();
		break;
	case en_TeamScene:
		scene = TeamScene::createScene();
		break;
	case en_RankScene:
		scene = RankScene::createScene();
		break;*/
	}
	
	if (scene == NULL)
		return;

	auto pDirector = Director::getInstance();
	auto curScene = pDirector->getRunningScene();   //���е�ǰ����

	if (ccts == NULL)
		ccts = CCTransitionFadeTR::create(1.0f, scene);

	if (curScene == NULL)
		pDirector->runWithScene(scene);
	else
		pDirector->replaceScene(scene);
}



