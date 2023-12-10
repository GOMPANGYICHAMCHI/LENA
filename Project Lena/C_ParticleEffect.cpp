#include "Pch.h"
#include "C_ParticleEffect.h"

const float pi = 3.14159265359;

// 생성자
C_ParticleEffect::C_ParticleEffect(Actor* owner) : Component(owner)
{
	ptype = SpriteSwap;
	CurrentSprite_index = 0;
	delayframe = 0;
	FrameCheck = 0;
	IsActivate = false;
	IsLoop = false;
}

// 업데이트
void C_ParticleEffect::Update(float deltatime)
{
	if (IsActivate)
	{
		switch (ptype)
		{
		// 스프라이트 스왑 방식
		case SpriteSwap:

			FrameCheck++;

			break;

		// 스프라이트 트랜슬레이트 방식
		// 해당 방식 에서는 애니메이션 내의 delayfram 이 재생 프레임길이 로 쓰입니다
		case SpriteTranslate:

			FrameCheck++;

			int particle_count = particle_pos.size();

			for (int i = 0; i < particle_count; i++)
			{
				particle_pos[i] += Vector2D(cos(2*pi/ particle_count*i), sin(2 * pi / particle_count * i))* FrameCheck;
			}

			if (FrameCheck > delayframe)
			{
				if (IsLoop)
				{
					Play();
				}
				else
				{
					Stop();
				}
			}

			break;
		}
	}
}

// 렌더
void C_ParticleEffect::Render(Renderer* renderer)
{
	if (IsActivate)
	{
		switch (ptype)
		{
		case SpriteSwap:

			renderer->DrawBitmap(
				owner->transform->GetPosition(),
				World_width, World_height,
				m_Animation->sprites[CurrentSprite_index]->sc_rect,
				m_Animation->sprites[CurrentSprite_index]->source,false, false);

			break;
		case SpriteTranslate:

			for (int i = 0; i < particle_pos.size(); i++)
			{
				renderer->DrawBitmap(
					particle_pos[i],
					World_width, World_height,
					m_sprite->sc_rect,
					m_sprite->source, false, false);
			}

			break;
		}
	}
}

// 애니메이션 설정
// 입력값 : 애니메이션 포인터
void C_ParticleEffect::SetSource(Sprite* input_sprite)
{
	m_sprite = input_sprite;
}

// 애니메이션 방식 설정
// 입력값 : 파티클 애니메이션 enum 타입
void C_ParticleEffect::SetAnimationType(ParticleType input_type)
{
	ptype = input_type;
}

// 스프라이트 월드 사이즈 변경
// 입력값 : 스프라이트 가로길이 , 스프라이트 세로길이
void C_ParticleEffect::SetWorldSize(float input_width, float input_height)
{
	World_width = input_width;
	World_height = input_height;
}

// 딜레이 프레임 설정
void C_ParticleEffect::SetDelayFrame(int input_delayframe)
{
	delayframe = input_delayframe;
}

// 파티클 수량 설정 
void C_ParticleEffect::SetParticleAmount(int input_particleamount)
{
	ParticleAmount = input_particleamount;
	particle_pos.clear();
	for (int i = 0; i < input_particleamount; i++)
	{
		Vector2D temp(0, 0);
		particle_pos.push_back(temp);
	}
}

// 파티클 애니메이션 루프 설정
// 입력값 : 루프설정 여부
void C_ParticleEffect::SetLoop(bool input_loop)
{
	IsLoop = input_loop;
}

// 파티클 실행
void C_ParticleEffect::Play()
{
	CurrentSprite_index = 0;
	FrameCheck = 0;
	if (ptype == SpriteTranslate)
	{
		Vector2D vec = owner->transform->GetPosition();
		for (int i = 0; i < particle_pos.size(); i++)
		{
			particle_pos[i] = vec;
		}
	}
	IsActivate = true;
}

// 파티클 종료
void C_ParticleEffect::Stop()
{
	IsActivate = false;
	if (ptype == SpriteTranslate)
	{
		Vector2D vec = owner->transform->GetPosition();
		for (int i = 0; i < particle_pos.size(); i++)
		{
			particle_pos[i] = vec;
		}
	}
	CurrentSprite_index = 0;
	FrameCheck = 0;
}
