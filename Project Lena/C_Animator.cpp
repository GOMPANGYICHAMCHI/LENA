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

// �ִϸ��̼� �߰�
// �Է°� : �ִϸ��̼� ������ 
void C_Animator::AddAnimation
(Animation* input_animation, float input_delayframe)
{
	input_animation->delayframe = input_delayframe;
	m_Animations.push_back(input_animation);
}

// �ִϸ��̼� �� ���� �Լ�
void C_Animator::SetAnimationVector
(vector<Animation*> input_animation, float input_delayframe)
{
	m_Animations = input_animation;

	for(int i = 0; i < input_animation.size() ; i++)
	{
		m_Animations[i]->delayframe = input_delayframe;
	}
}

// ������Ʈ
// �������Ӹ��� ������üũ ++ , ������ ������ ������ ���� ��������Ʈ �ε��� ����
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

// ����
// �ִϸ������� ������ ������ ��ġ ���� ������ ������ ��������Ʈ ���
void C_Animator::Render(Renderer* renderer)
{
	if (isRender)
	{
		if (m_Animations.size() > 0)
		{
			// ��ġ ���� ================================================================

			// ��Ʈ�� ��� ��ġ
			Vector2D output_pos;

			// ��ũ�� ��½�
			if (is_OnScreen)
			{
				// ��� ��ġ�� �̸� ������ ȭ�� ��� ��ġ
				output_pos = ToScreenPos;
			}
			// �ƴҰ��, ( ������ǥ ��½� )
			else
			{
				// ��ġ ���� ���ΰ� true �� ���,
				if (is_AdjustOn)
				{
					// ������ ��ġ
					output_pos = Vector2D(
						owner->transform->GetPosition().X + AdjustAmount.X,
						owner->transform->GetPosition().Y + AdjustAmount.Y);
				}
				// �ƴҰ��, ( ��ġ �������� )
				else
				{
					// ������ ���� ��ġ
					output_pos = owner->transform->GetPosition();
				}
			}

			// ��� =====================================================================

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

// ��ġ ���� ���� ����
// �Է°� : ��ġ ���� ���� , ��ġ ���� ���� ( ���� ���� true �ÿ��� �ݿ��� )
void C_Animator::SetAdjust(bool input_set, Vector2D input_amount)
{
	if (input_set)
	{
		AdjustAmount = input_amount;
	}
	is_AdjustOn = input_set;
}

// �ִϸ��̼� ���� ����
// �Է°� : ���� ���� ����
void C_Animator::SetAnimationDirection(bool input_isLeft)
{
	this->is_Left = input_isLeft;
}

// �ִϸ��̼� ��� ���� ����
// �Է°� : �ִϸ��̼� ��� ����
void C_Animator::SetAnimationUse(bool input_isanimation)
{
	is_Animation = input_isanimation;
}

// ���� �ִϸ��̼� ����
// �Է°� : �ִϸ��̼� �ε���
void C_Animator::SetAnimationState(int animation_index)
{
	CurrentAnimation_index = animation_index;
	CurrentSprite_index = 0;
	FrameCheck = 0;
}

// ������ �ִϸ��̼� �� Ư�� �ִϸ��̼� �÷��� ������ �Է°����� ����
// �Է°� : ��� �ִϸ��̼� �ε��� , ������ ������
void C_Animator::SetFrameDilay(int input_animationindex, int input_delay)
{
	m_Animations[input_animationindex]->delayframe = input_delay;
}

// ���� �ִϸ��̼� �� ������ ����
void C_Animator::SetFrameState(int input_frame)
{
	CurrentSprite_index = input_frame;
}

// ��ũ�� ��� ���� ����
void C_Animator::SetIsOnScreen(bool input_isonscreen)
{
	is_OnScreen = input_isonscreen;
}

// ȭ�� ��½� ��ġ
void C_Animator::SetToScreenPos(Vector2D input_pos)
{
	ToScreenPos = input_pos;
}

// ��������Ʈ ���� ������ ����
// �Է°� : ��������Ʈ ���α��� , ��������Ʈ ���α���
void C_Animator::SetWorldSize(float input_width, float input_height)
{
	this->World_width = input_width;
	this->World_height = input_height;
}

// ��������Ʈ ���� ������ ����
// �Է°� : ��������Ʈ ũ��
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