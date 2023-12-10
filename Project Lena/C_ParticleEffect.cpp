#include "Pch.h"
#include "C_ParticleEffect.h"

const float pi = 3.14159265359;

// ������
C_ParticleEffect::C_ParticleEffect(Actor* owner) : Component(owner)
{
	ptype = SpriteSwap;
	CurrentSprite_index = 0;
	delayframe = 0;
	FrameCheck = 0;
	IsActivate = false;
	IsLoop = false;
}

// ������Ʈ
void C_ParticleEffect::Update(float deltatime)
{
	if (IsActivate)
	{
		switch (ptype)
		{
		// ��������Ʈ ���� ���
		case SpriteSwap:

			FrameCheck++;

			break;

		// ��������Ʈ Ʈ��������Ʈ ���
		// �ش� ��� ������ �ִϸ��̼� ���� delayfram �� ��� �����ӱ��� �� ���Դϴ�
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

// ����
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

// �ִϸ��̼� ����
// �Է°� : �ִϸ��̼� ������
void C_ParticleEffect::SetSource(Sprite* input_sprite)
{
	m_sprite = input_sprite;
}

// �ִϸ��̼� ��� ����
// �Է°� : ��ƼŬ �ִϸ��̼� enum Ÿ��
void C_ParticleEffect::SetAnimationType(ParticleType input_type)
{
	ptype = input_type;
}

// ��������Ʈ ���� ������ ����
// �Է°� : ��������Ʈ ���α��� , ��������Ʈ ���α���
void C_ParticleEffect::SetWorldSize(float input_width, float input_height)
{
	World_width = input_width;
	World_height = input_height;
}

// ������ ������ ����
void C_ParticleEffect::SetDelayFrame(int input_delayframe)
{
	delayframe = input_delayframe;
}

// ��ƼŬ ���� ���� 
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

// ��ƼŬ �ִϸ��̼� ���� ����
// �Է°� : �������� ����
void C_ParticleEffect::SetLoop(bool input_loop)
{
	IsLoop = input_loop;
}

// ��ƼŬ ����
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

// ��ƼŬ ����
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
