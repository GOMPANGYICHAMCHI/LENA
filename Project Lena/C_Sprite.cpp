#include "Pch.h"
#include "C_Sprite.h"

C_Sprite::C_Sprite(Actor* owner) : Component(owner)
{
	isrender = false;
	isadjustpos = false;
	adjustamount = Vector2D(0, 0);
	World_width = 2560;
	World_height = 1600;
}

void C_Sprite::Update(float deltaTime)
{

}

void C_Sprite::Render(Renderer* renderer)
{
	if (isrender && Source != NULL)
	{
		if (!isadjustpos)
		{
			renderer->DrawBitmap(
				owner->transform->GetPosition(),
				World_width, World_height,
				Source->sc_rect,
				Source->source, false, false, 1);
		}
		else
		{
			renderer->DrawBitmap(
				Vector2D(
					owner->transform->GetPosition().X + adjustamount.X,
					owner->transform->GetPosition().Y + adjustamount.Y),
				World_width, World_height,
				Source->sc_rect,
				Source->source, false, false, 1);
		}
	}
}



int C_Sprite::GetSortOrder()
{
	return sortOrder;
}

// 위치 보정 정도 설정
// 입력값 : 위치 보정 정도
void C_Sprite::SetAdjustAmount(Vector2D input_amount)
{
	adjustamount = input_amount;
	isadjustpos = true;
}

// 위치 보정 여부 설정 ( 호출시 자동으로 보정이 ON 됩니다 )
// 입력값 : 위치 보정 여부 설정
void C_Sprite::SetAdjustPos(bool input_set)
{
	isadjustpos = input_set;
}

Vector2D C_Sprite::GetPivot()
{
	return Vector2D(Source->pivotPosX, Source->pivotPosY);
}

void C_Sprite::SetSource(Sprite* source)
{
	this->Source = source;
}

void C_Sprite::SetImageSize(Vector2D input_size)
{
	World_width = input_size.X;
	World_height = input_size.Y;
}

// 스프라이트 렌더 활성화
void C_Sprite::RenderOn()
{
	this->isrender = true;
}

void C_Sprite::RenderOff()
{
	this->isrender = false;
}
