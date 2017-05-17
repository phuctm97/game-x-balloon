#include "PlayLayer.h"
using namespace cocos2d;

bool PlayLayer::init()
{
	if ( !Layer::init() )
		return false;

	initGraphics();

	initEvents();

	initGame();

	return true;
}

void PlayLayer::initGraphics()
{
	// Background
	auto spriteBackground = Sprite::create( "res/fullhd/bg.jpg" );
	addChild( spriteBackground, 0 );
	spriteBackground->setPosition( Director::getInstance()->getVisibleSize().width / 2,
	                               Director::getInstance()->getVisibleSize().height / 2 );

	// Label
	_labelScore = Label::createWithTTF( "Score: 0", "res/joystix monospace.ttf", 40 );
	addChild( _labelScore, 20 );

	_labelScore->setPosition( 20, Director::getInstance()->getVisibleSize().height - 20 );
	_labelScore->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
	_labelScore->enableOutline( Color4B( 0, 0, 0, 255 ), 5 );
	_labelScore->enableUnderline();
}

void PlayLayer::initEvents()
{
	scheduleUpdate();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2( PlayLayer::onTouchBegan, this );

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority( touchListener, 1 );
}

void PlayLayer::initGame()
{
	_timeForNewBalloon = 1.0f;
	_timeForNewDifficulty = 3.0f;

	_score = 0;
}

void PlayLayer::update( float dt )
{
	checkForNewBalloon( dt );

	checkForNewDifficulty( dt );
}

void PlayLayer::checkForNewBalloon( float dt )
{
	_timeForNewBalloon -= dt;
	if ( _timeForNewBalloon <= 0 ) {

		auto spriteBalloon = Sprite::create( "res/fullhd/balloon.png" );
		addChild( spriteBalloon, 10 );
		_balloons.pushBack( spriteBalloon );

		spriteBalloon->setPosition( random<int>( 0, Director::getInstance()->getVisibleSize().width ), -spriteBalloon->getContentSize().height / 2 );

		spriteBalloon->setColor( Color3B( random<int>( 0, 255 ),
		                                  random<int>( 0, 255 ),
		                                  random<int>( 0, 255 ) ) );

		auto actionMove = MoveTo::create( 1.0f, Vec2( spriteBalloon->getPositionX(), Director::getInstance()->getVisibleSize().height + spriteBalloon->getContentSize().height / 2 ) );
		auto actionRemove = CallFuncN::create( CC_CALLBACK_1( PlayLayer::onRemoveBalloon, this ) );

		auto sequenceActions = Sequence::create( actionMove, actionRemove, nullptr );
		spriteBalloon->runAction( sequenceActions );

		_timeForNewBalloon = 1.0f;
	}
}

void PlayLayer::checkForNewDifficulty( float dt )
{
	_timeForNewDifficulty -= dt;
	if ( _timeForNewDifficulty <= 0 ) {
		increaseDifficulty();

		_timeForNewDifficulty = 3.0f;
	}
}

void PlayLayer::increaseDifficulty()
{
	_timeForNewBalloon -= 0.2f;

	if ( _timeForNewBalloon < 0.3f ) _timeForNewBalloon = 0.3f;
}

bool PlayLayer::onTouchBegan( cocos2d::Touch* touch, cocos2d::Event* e )
{
	checkBalloonTouched( touch->getLocation() );

	return true;
}

void PlayLayer::onRemoveBalloon( cocos2d::Node* sender )
{
	sender->removeFromParent();
	_balloons.eraseObject( (Sprite*)sender );


}

void PlayLayer::checkBalloonTouched( const cocos2d::Vec2& touchLocation )
{
	for ( auto balloon : _balloons ) {
		float d = touchLocation.getDistance( balloon->getPosition() );

		if ( d <= balloon->getContentSize().width / 2 ) {
			onTouchBalloon( balloon );
		}
	}
}

void PlayLayer::onTouchBalloon( cocos2d::Sprite* balloon )
{
	auto spritePow = Sprite::create( "res/fullhd/pow.png" );
	spritePow->setPosition( balloon->getPosition() );
	spritePow->setColor( balloon->getColor() );
	addChild( spritePow, 10 );

	// remove pow
	auto actionBlink = Blink::create( 1.0f, 10 );
	auto actionRemovePow = RemoveSelf::create();
	spritePow->runAction( Sequence::create( actionBlink, actionRemovePow, nullptr ) );

	// remove balloon
	auto actionRemoveBalloon = CallFuncN::create( CC_CALLBACK_1( PlayLayer::onRemoveBalloon, this ) );
	balloon->runAction( actionRemoveBalloon );

	// increase score
	_score++;

	std::stringstream stringBuilder;
	stringBuilder << "Score: " << _score;
	_labelScore->setString( stringBuilder.str() );
}
