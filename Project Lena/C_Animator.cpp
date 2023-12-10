#include "Pch.h"
#include "C_Animator.h"
#include "Animation.h"

C_Animator::C_Animator(Actor* owner) :Component(owner), isRender(true)
{
	is_Animation = false;
	is_AdjustOn = false;
	is_OnScreen = false;
	ToScreenPos = Vector2D(0, 0);
	//CurrentSprite_index = 0;
}

// 애니메이션 추가
// 입력값 : 애니메이션 포인터 
void C_Animator::AddAnimation
(Animation* input_animation, float input_delayframe)
{
	input_animation->delayframe = input_delayframe;
	m_Animations.push_back(input_animation);
}

// 애니메이션 맵 대입 함수
void C_Animator::SetAnimationVector
(vector<Animation*> input_animation, float input_delayframe)
{
	m_Animations = input_animation;

	for(int i = 0; i < input_animation.size() ; i++)
	{
		m_Animations[i]->delayframe = input_delayframe;
	}
}

// 업데이트
// 매프레임마다 프레임체크 ++ , 지정된 딜레이 프레임 마다 스프라이트 인덱스 변경
void C_Animator::Update(float deltatime)
{
	if (is_Animation)
	{
		FrameCheck += 1;

		if (FrameCheck > m_Animations[CurrentAnimation_index]->delayframe)
		{
			if (CurrentSprite_index + 1
				== m_Animations[CurrentAnimation_index]->sprites.size())
			{
				if (m_Animations[CurrentAnimation_index]->isloop)
				{
					CurrentSprite_index = 0;
				}
			}
			else
			{
				CurrentSprite_index += 1;
			}
 
			FrameCheck = 0;
		}
	}
}

void C_Animator::C_Update(float deltatime)
{
	if (is_Animation)
	{
		FrameCheck += 1;

 		if (FrameCheck > m_Animations[CurrentAnimation_index]->delayframe)
		{
			if (CurrentSprite_index + 1
				== m_Animations[CurrentAnimation_index]->sprites.size())
			{
				if (m_Animations[CurrentAnimation_index]->isloop)
				{
					CurrentSprite_index = 0;
				}
			}
			else
			{
				CurrentSprite_index += 1;
			}

			FrameCheck = 0;
		}
	}
}

// 렌더
// 애니메이터의 오너인 액터의 위치 값을 참조해 지정된 스프라이트 출력
void C_Animator::Render(Renderer* renderer)
{
	if (isRender)
	{
		if (m_Animations.size() > 0)
		{
			// 위치 설정 ================================================================

			// 비트맵 출력 위치
			Vector2D output_pos;

			// 스크린 출력시
			if (is_OnScreen)
			{
				// 출력 위치는 미리 지정된 화면 출력 위치
				output_pos = ToScreenPos;
			}
			// 아닐경우, ( 월드좌표 출력시 )
			else
			{
				// 위치 보정 여부가 true 일 경우,
				if (is_AdjustOn)
				{
					// 보정된 위치
					output_pos = Vector2D(
						owner->transform->GetPosition().X + AdjustAmount.X,
						owner->transform->GetPosition().Y + AdjustAmount.Y);
				}
				// 아닐경우, ( 위치 보정없음 )
				else
				{
					// 오너의 원본 위치
					output_pos = owner->transform->GetPosition();
				}
			}

			// 출력 =====================================================================

			if (CurrentSprite_index
				< m_Animations[CurrentAnimation_index]->sprites.size())
			{
				renderer->DrawBitmap(
					output_pos,
					World_width, World_height,
					m_Animations[CurrentAnimation_index]->sprites[CurrentSprite_index]->sc_rect,
					m_Animations[CurrentAnimation_index]->sprites[CurrentSprite_index]->source, 
					is_OnScreen, is_Left);
			}
		}
	}
}

int C_Animator::GetSortOrder()
{
	return sortOrder;
}

// 위치 조정 여부 설정
// 입력값 : 위치 조정 여부 , 위치 조정 정도 ( 조정 여부 true 시에만 반영됨 )
void C_Animator::SetAdjust(bool input_set, Vector2D input_amount)
{
	if (input_set)
	{
		AdjustAmount = input_amount;
	}
	is_AdjustOn = input_set;
}

// 애니메이션 방향 설정
// 입력값 : 방향 왼쪽 여부
void C_Animator::SetAnimationDirection(bool input_isLeft)
{
	this->is_Left = input_isLeft;
}

// 애니메이션 사용 여부 설정
// 입력값 : 애니메이션 사용 여부
void C_Animator::SetAnimationUse(bool input_isanimation)
{
	is_Animation = input_isanimation;
}

// 현재 애니메이션 설정
// 입력값 : 애니메이션 인덱스
void C_Animator::SetAnimationState(int animation_index)
{
	CurrentAnimation_index = animation_index;
	CurrentSprite_index = 0;
	FrameCheck = 0;
}

// 설정된 애니메이션 중 특정 애니메이션 플레임 딜레이 입력값으로 수정
// 입력값 : 대상 애니메이션 인덱스 , 프레임 딜레이
void C_Animator::SetFrameDilay(int input_animationindex, int input_delay)
{
	m_Animations[input_animationindex]->delayframe = input_delay;
}

// 현재 애니매이션 내 프레임 설정
void C_Animator::SetFrameState(int input_frame)
{
	CurrentSprite_index = input_frame;
}

// 스크린 출력 여부 설정
void C_Animator::SetIsOnScreen(bool input_isonscreen)
{
	is_OnScreen = input_isonscreen;
}

// 화면 출력시 위치
void C_Animator::SetToScreenPos(Vector2D input_pos)
{
	ToScreenPos = input_pos;
}

// 스프라이트 월드 사이즈 변경
// 입력값 : 스프라이트 가로길이 , 스프라이트 세로길이
void C_Animator::SetWorldSize(float input_width, float input_height)
{
	this->World_width = input_width;
	this->World_height = input_height;
}

// 스프라이트 월드 사이즈 변경
// 입력값 : 스프라이트 크기
void C_Animator::SetWorldSize(Vector2D input_size)
{
	this->World_width = input_size.X;
	this->World_height = input_size.Y;
}

void C_Animator::RenderOn()
{
	this->isRender = true;
}

void C_Animator::RenderOff()
{
	this->isRender = false;
}