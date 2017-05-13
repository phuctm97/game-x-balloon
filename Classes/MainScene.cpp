#include "MainScene.h"
USING_NS_CC;

cocos2d::Scene* MainScene::createScene()
{
	Scene* scene = Scene::create();
	Layer* layer = MainScene::create();
	scene->addChild( layer );
	return scene;
}

bool MainScene::init()
{
	if( !Layer::init() ) return false;

	doInitGraphics();
	doInitEvents();

	PLAY_BACKGROUND( "bg.mp3" );

	return true;
}

void MainScene::doInitGraphics()
{
	Sprite* bg = Sprite::create( "bg.jpg" );
	bg->setPosition( SIZE_VISIBLE*0.5f );
	addChild( bg, Z_ORDERS::BG0 );

	lblScore = Label::createWithSystemFont( StringUtils::format( "Score: %d", score ), "Arial", 60 );
	lblScore->setAnchorPoint( Vec2( 0, 1 ) );
	lblScore->setPosition( 10, SIZE_VISIBLE.height-10 );
	addChild( lblScore, Z_ORDERS::UI0 );

	lblTime = Label::createWithSystemFont( StringUtils::format( "Time: %d", (int)timerGame ), "Arial", 60 );
	lblTime->setAnchorPoint( Vec2( 1, 1 ) );
	lblTime->setPosition( SIZE_VISIBLE.width-10, SIZE_VISIBLE.height-10 );
	addChild( lblTime, Z_ORDERS::UI0 );

	lblMain = Label::createWithSystemFont( "Touch to start", "Arial", 70 );
	lblMain->setPosition( SIZE_VISIBLE.width*0.5f, SIZE_VISIBLE.height*0.7f );
	addChild( lblMain, Z_ORDERS::UI1 );
}

void MainScene::doInitEvents()
{
	scheduleUpdate();

	EventListenerTouchAllAtOnce* listenerTouches = EventListenerTouchAllAtOnce::create();
	listenerTouches->onTouchesBegan = CC_CALLBACK_2( MainScene::onTouchesBegan, this );
	EVENT_DISPATCHER->addEventListenerWithSceneGraphPriority( listenerTouches, this );
}

void MainScene::update( float dt )
{
	if( !playing ) return;

	timerPerBalloon -= dt;
	if( timerPerBalloon<=0 ) {
		timerPerBalloon = intervalPerBalloon;
		intervalPerBalloon = MAX( 0.2f, intervalPerBalloon-0.01f );

		Sprite* balloon = Sprite::create( "balloon.png" );
		balloon->setAnchorPoint( Vec2( 0.5f, 0.6f ) );
		balloon->setPosition( SIZE_VISIBLE.width*random<float>( 0.1f, 0.9f ), -balloon->getContentSize().height*0.4f );
		balloon->setColor( Color3B( random<int>( 0, 255 ), random<int>( 0, 255 ), random<int>( 0, 255 ) ) );

		addChild( balloon, Z_ORDERS::MAIN0 );
		balloons.pushBack( balloon );

		balloon->runAction(
			Sequence::create(
			MoveBy::create( timeBalloonMove, Vec2( 0, SIZE_VISIBLE.height+balloon->getContentSize().height ) ),
			CallFuncN::create( [this]( Node* sender ){ removeChild( sender ); balloons.eraseObject( (Sprite*)sender ); } ),
			nullptr ) );
		timeBalloonMove = MAX( 0.5f, timeBalloonMove-0.01f );
	}

	timerGame -= dt;
	if( timerGame<0 ) {
		playing = false;
		lblMain->setVisible( true );
		lblMain->setString( StringUtils::format( "Time up!\nYou got %d points\nTouch to try again", score ) );
	}
	else
		lblTime->setString( StringUtils::format( "Time: %d", (int)timerGame ) );
}

void MainScene::doResetGame()
{
	intervalPerBalloon = 0.5f;
	timerPerBalloon = intervalPerBalloon;
	score = 0;
	timerGame = TIME_PER_GAME;
	timeBalloonMove = 1.5f;

	for( Sprite* balloon : balloons )
		removeChild( balloon );
	balloons.clear();

	lblTime->setString( StringUtils::format( "Time: %d", (int)timerGame ) );
	lblScore->setString( StringUtils::format( "Score: %d", score ) );
}

void MainScene::onTouchesBegan( const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event )
{
	if( !playing ) {
		lblMain->setVisible( false );
		doResetGame(); playing = true;

		return;
	}


	Vector<Sprite*> removingBalloons;

	for( Touch* touch: touches ) {
		for( Sprite* balloon: balloons ) {
			if( balloon->getPosition().getDistance( touch->getLocation() ) < balloon->getContentSize().width*0.5f )
				removingBalloons.pushBack( balloon );
		}
	}

	for( Sprite* balloon: removingBalloons ) {
		Sprite* boom = Sprite::create( "pow.png" );
		boom->setPosition( balloon->getPosition() );
		boom->setColor( balloon->getColor() );
		addChild( boom, Z_ORDERS::MAIN1 );

		boom->runAction( Sequence::createWithTwoActions(
			DelayTime::create( 0.3f ),
			CallFuncN::create( [this]( Node* sender ){ removeChild( sender ); } )
			) );

		removeChild( balloon );
		balloons.eraseObject( balloon );

		score++;
		lblScore->setString( StringUtils::format( "Score: %d", score ) );

		PLAY_EFFECT( "pop.mp3" );
	}
	removingBalloons.clear();

	return;
}

