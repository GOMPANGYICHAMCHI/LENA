#include "O_CoilSector.h"
#include "Vector2D.h"
#include "C_Transform.h"
#include "CC_AABBCollision.h"
#include "CC_CoilAnimator.h"
#include "C_Sprite.h"
#include "Sprite.h"
#include "C_Velocity.h"
#include "GameScene.h"
#include "C_PlatformProperty.h"
#include "CC_Debugger.h"
#include "ScreenSize.h"


O_CoilSector::O_CoilSector() :m_PlatformActorID("Platform")
{
	
}

// 코일 섹터 초기화 (액터 생성, 현재씬에 추가, 컴포넌트 추가, 컴포넌트 설정)
void O_CoilSector::Initialize(
	std::shared_ptr<GameScene> currentScene, 
	std::vector<Animation*> input_animation,
	GameStateManager* input_gamestate,
	Sprite* input_coilplatform)
{
	// 액터 새로 생성
	CoilSectorActor = std::make_shared<Actor>();
	// 랜더 오더 설정
	CoilSectorActor->SetRenderOrder(static_cast<int>(RenderOrder::Platform));

	// 필요한 컴포넌트 추가 및 초기화
	this->AddComponents();

	// 추가된 컴포넌트 설정
	this->SetComponents(input_gamestate);

	// 현재 씬에 코일 섹터 액터 추가
	currentScene->Actors.Add(CoilSectorActor);

	// 콜리전 설정 후 콜리전매니저 에게 전달
	currentScene->m_CollisionManager.Add(CoilSectorActor);
	
	// 애니메이션 설정
	this->CoilSectorActor->GetComponent<C_Sprite>()->SetSource(input_coilplatform);
	this->CoilSectorActor->GetComponent<C_Sprite>()->SetImageSize
	(Vector2D(SCREEN_WIDTH, COILSECTOR_PLATFORM_HEIGHT));
	this->CoilSectorActor->GetComponent<C_Sprite>()->RenderOn();

	std::shared_ptr<CC_CoilAnimator> CoilAnimator = this->CoilSectorActor->GetComponent<CC_CoilAnimator>();

	CoilAnimator->SetAnimationState(1);
	CoilAnimator->SetWorldSize(SCREEN_WIDTH, COILSECTOR_COIL_HEIGHT);
	CoilAnimator->SetAnimationVector(input_animation, 0.5);
	CoilAnimator->SetAnimationUse(true);
}

// 코일 섹터 액터 반환
std::shared_ptr<Actor> O_CoilSector::GetActor()
{
	return CoilSectorActor;
}

// 코일 상태 변경
// 입력값 : 코일 상태
void O_CoilSector::SetCoil(bool input_set)
{
	CoilSectorActor->GetComponent<CC_CoilAnimator>()->SetCoil(input_set);
}

// 빈 섹터 종속 코일로설정
void O_CoilSector::SetVoidSecCoil(bool input_set)
{
	CoilSectorActor->GetComponent<CC_CoilAnimator>()->SetVoidSecCoil(input_set);
}

// 위치 변경
void O_CoilSector::SetPosition(Vector2D input_lefttop)
{
	CoilSectorActor->GetComponent<CC_CoilAnimator>()->SetCoilPosY(input_lefttop.Y);
	CoilSectorActor->transform->
		SetPosition(Vector2D( 
			input_lefttop.X + COILSECTOR_WIDTH / 2,
			input_lefttop.Y + COILSECTOR_HEIGHT / 2));
	CoilSectorActor->GetComponent<CC_AABBCollision>()->SetRect(PlatformCollision);
}

// 기존 위치에서 낮추기
// 입력값 : 낮출양
void O_CoilSector::SetDown(float input_amount)
{
	CoilSectorActor->transform->SetPosition(
			CoilSectorActor->transform->GetPosition().X,
			CoilSectorActor->transform->GetPosition().Y + input_amount);
}

// 필요한 컴포넌트 추가 및 초기화
void O_CoilSector::AddComponents()
{
	CoilSectorActor->AddComponent<C_Transform>();
	CoilSectorActor->AddComponent<C_Velocity>();
	CoilSectorActor->AddComponent<CC_AABBCollision>()->SetLayer(CollisionLayer::Platform);
	CoilSectorActor->AddComponent<C_Sprite>();
	CoilSectorActor->AddComponent<CC_CoilAnimator>();
	CoilSectorActor->AddComponent<C_PlatformProperty>()->SetPassable(Passable::Up);
	CoilSectorActor->AddComponent<CC_Debugger>();
}

// 추가된 컴포넌트 설정
void O_CoilSector::SetComponents(GameStateManager* input_gamestate)
{
	Vector2D temp;

	temp.X = 1920;
	temp.Y = COILSECTOR_PLATFORM_HEIGHT;

	CoilSectorActor->GetComponent<C_Sprite>()->SetImageSize(temp);

	temp.X = 0;
	temp.Y = -(COILSECTOR_PLATFORM_HEIGHT / 2);

	CoilSectorActor->GetComponent<C_Sprite>()->SetAdjustAmount(temp);

	PlatformCollision.center = Vector2D(0, 0);
	PlatformCollision.width = COILSECTOR_WIDTH / 2;
	PlatformCollision.height = COILSECTOR_PLATFORM_HEIGHT/2;

	CoilSectorActor->GetComponent<CC_AABBCollision>()->SetRect(PlatformCollision);

	CoilSectorActor->GetComponent<CC_AABBCollision>()->SetAdjust(true, Vector2D(0, temp.Y));

	CoilSectorActor->GetComponent<CC_CoilAnimator>()->
		SetAdjust(true, Vector2D(0, COILSECTOR_COIL_HEIGHT / 2 ));

	CoilSectorActor->GetComponent<CC_CoilAnimator>()->SetGameStateP(input_gamestate);

	CoilSectorActor->GetComponent<C_ActorID>()->SetStringActorID(m_PlatformActorID);
}
