#pragma once

#include "Pch.h"
#include "Component.h"
#include "Renderer.h"

#include "Animation.h"
#include "Actor.h"

// ��ƼŬ �ִϸ��̼� ���
enum ParticleType
{
	SpriteSwap,
	SpriteTranslate
};

/// <summary>
/// ��ƼŬ ����Ʈ
/// 
/// �ִϸ����Ϳ� ������ ������� �����ϴ� 'SpriteSwap' ���İ�
/// ��������Ʈ �������� ���� ���� �����̴� 'SpriteTranslate' ���� �� �ִ�
/// ���Ǵ� ������ �����ϳ� ���Ŀ����� ������ �ٸ���
/// </summary>
class C_ParticleEffect : public Component
{
public:
	C_ParticleEffect(Actor* owner);
	const static int SortOrder = static_cast<int>(ComponentSortOrder::Animation);

public:
	// ������Ʈ
	virtual void Update(float deltatime) override;
	// ����
	virtual void Render(Renderer* renderer) override;

	// �ִϸ��̼� ����
	// �Է°� : �ִϸ��̼� ������
	void SetSource(Sprite* input_sprite);

	// �ִϸ��̼� ��� ����
	// �Է°� : ��ƼŬ �ִϸ��̼� enum Ÿ��
	void SetAnimationType(ParticleType input_type);

	// ��������Ʈ ���� ������ ����
	// �Է°� : ��������Ʈ ���α��� , ��������Ʈ ���α���
	void SetWorldSize(float input_width, float input_height);

	// ������ ������ ����
	void SetDelayFrame(int input_delayframe);

	// ��ƼŬ ���� ����
	void SetParticleAmount(int input_particleamount);

	// ��ƼŬ �ִϸ��̼� ���� ����
	// �Է°� : �������� ����
	void SetLoop(bool input_loop);

	// ��ƼŬ ����
	void Play();

	// ��ƼŬ ����
	void Stop();

private :
	ParticleType ptype;					// ��ƼŬ �ִϸ��̼� Ÿ��

	int CurrentSprite_index;			// ���� ��������Ʈ �ε���
	
	int delayframe;						// �ִϸ��̼� ��� �ð�
	int FrameCheck;						// ������ üũ ( ������Ʈ ���� �������� ���� 1�� ������ )

	int ParticleAmount;					// ��ƼŬ ����

	float World_width;                  // ��������Ʈ ���α���
    float World_height;                 // ��������Ʈ ���α���

	bool IsActivate;					// ��ƼŬ ����Ʈ Ȱ��ȭ ����
	bool IsLoop;						// ��ƼŬ �ִϸ��̼� ���� ����

	Sprite* m_sprite;				    // ��ƼŬ �ִϸ��̼� ���� ���ҽ�

	vector<Vector2D> particle_pos;		// ��ü ��ƼŬ ��ġ
};

