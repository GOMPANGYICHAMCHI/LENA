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

// ���� ���� �ʱ�ȭ (���� ����, ������� �߰�, ������Ʈ �߰�, ������Ʈ ����)
void O_CoilSector::Initialize(
	std::shared_ptr<GameScene> currentScene, 
	std::vector<Animation*> input_animation,
	GameStateManager* input_gamestate,
	Sprite* input_coilplatform)
{
	// ���� ���� ����
	CoilSectorActor = std::make_shared<Actor>();
	// ���� ���� ����
	CoilSectorActor->SetRenderOrder(static_cast<int>(RenderOrder::Platform));

	// �ʿ��� ������Ʈ �߰� �� �ʱ�ȭ
	this->AddComponents();

	// �߰��� ������Ʈ ����
	this->SetComponents(input_gamestate);

	// ���� ���� ���� ���� ���� �߰�
	currentScene->Actors.Add(CoilSectorActor);

	// �ݸ��� ���� �� �ݸ����Ŵ��� ���� ����
	currentScene->m_CollisionManager.Add(CoilSectorActor);
	
	// �ִϸ��̼� ����
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

// ���� ���� ���� ��ȯ
std::shared_ptr<Actor> O_CoilSector::GetActor()
{
	return CoilSectorActor;
}

// ���� ���� ����
// �Է°� : ���� ����
void O_CoilSector::SetCoil(bool input_set)
{
	CoilSectorActor->GetComponent<CC_CoilAnimator>()->SetCoil(input_set);
}

// �� ���� ���� ���Ϸμ���
void O_CoilSector::SetVoidSecCoil(bool input_set)
{
	CoilSectorActor->GetComponent<CC_CoilAnimator>()->SetVoidSecCoil(input_set);
}

// ��ġ ����
void O_CoilSector::SetPosition(Vector2D input_lefttop)
{
	CoilSectorActor->GetComponent<CC_CoilAnimator>()->SetCoilPosY(input_lefttop.Y);
	CoilSectorActor->transform->
		SetPosition(Vector2D( 
			input_lefttop.X + COILSECTOR_WIDTH / 2,
			input_lefttop.Y + COILSECTOR_HEIGHT / 2));
	CoilSectorActor->GetComponent<CC_AABBCollision>()->SetRect(PlatformCollision);
}

// ���� ��ġ���� ���߱�
// �Է°� : �����
void O_CoilSector::SetDown(float input_amount)
{
	CoilSectorActor->transform->SetPosition(
			CoilSectorActor->transform->GetPosition().X,
			CoilSectorActor->transform->GetPosition().Y + input_amount);
}

// �ʿ��� ������Ʈ �߰� �� �ʱ�ȭ
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

// �߰��� ������Ʈ ����
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
