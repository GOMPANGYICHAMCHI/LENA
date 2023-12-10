#pragma once
#include "Pch.h"
#include "Vector2D.h"

/// <summary>
/// 스프라이트
/// 
/// 원본이 될 비트맵의 주소값과 
/// 비트맵 내부에서의 해당 스프라이트가 될 위치인 렉트값을 갖는다
/// 
/// 파티클 이펙트 에서 타입이 SpriteTranslate 라면,
/// 월드위치를 초기화 받아 사용된다 ( 아닐경우 null )
/// </summary>
class Sprite
{
public:
	Sprite();

public:

	ID2D1Bitmap* source;	// 리소스 주소 ( 비트맵 포인터 )
	D2D_RECT_F sc_rect;		// 리소스 내 스프라이트 위치 ( 렉트 )

	int AnimationID;		// 애니메이션 아이디
	int index;				// 인덱스

	int pivotPosX;			// X 피봇 
	int pivotPosY;			// Y 피봇

	/// -------------------------------------------------------------
	/// 파티클 이펙트 에서 타입이 SpriteTranslate 일 경우 사용되는 변수
	/// ( 아니라면 null )
	/// -------------------------------------------------------------
	Vector2D worldpos;		// 파티클 이펙트용 월드 위치
	/// -------------------------------------------------------------
};

