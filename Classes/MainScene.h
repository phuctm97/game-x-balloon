#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "Include.h"
#define TIME_PER_GAME 30

class MainScene: public cocos2d::Layer
{
private:
	cocos2d::Vector<cocos2d::Sprite*> balloons;
	cocos2d::Label *lblScore, *lblTime, *lblMain;
	int score;

	float
		timerPerBalloon, intervalPerBalloon,
		timerGame, timeBalloonMove;
	bool playing;


	void doResetGame();
public:
	MainScene()
		: intervalPerBalloon( 0.5f ), timerPerBalloon( intervalPerBalloon )
		, score( 0 ), timerGame( TIME_PER_GAME ), timeBalloonMove( 1.5f )
		, playing( false )
	{}

	CREATE_FUNC( MainScene );
	static cocos2d::Scene* createScene();
private:
	bool init() override;
	void doInitGraphics();
	void doInitEvents();

	void update( float dt ) override;
	void onTouchesBegan( const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event ) override;
};


#endif //!__MAIN_SCENE_H__