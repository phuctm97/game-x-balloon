#ifndef __PLAY_SCENE_H_
#define __PLAY_SCENE_H_

#include <cocos2d.h>

class PlayLayer : public cocos2d::Layer
{
private:
	float _timeForNewBalloon;
	float _timeForNewDifficulty;
	cocos2d::Vector<cocos2d::Sprite*> _balloons;

	cocos2d::Label *_labelScore;
	int _score;

public:
	CREATE_FUNC( PlayLayer );

	bool init() override;

	void initGraphics();

	void initEvents();

	void initGame();

	void update( float dt ) override;

	void checkForNewBalloon(float dt);

	void checkForNewDifficulty( float dt );

	void increaseDifficulty();

	bool onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* e );

	void onRemoveBalloon( cocos2d::Node* sender );

	void checkBalloonTouched( const cocos2d::Vec2& touchLocation );

	void onTouchBalloon( cocos2d::Sprite* balloon );
};

#endif //!__PLAY_SCENE_H_