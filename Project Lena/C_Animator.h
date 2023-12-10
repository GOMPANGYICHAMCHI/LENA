#pragma once
#ifndef _CMOTION
#define _CMOTION

#include <vector>
#include <unordered_map>

#include "Component.h"
#include "Renderer.h"

//class Renderer;
class Animation;

using namespace std;

/// <summary>
/// Animater
/// </summary>
class C_Animator : public Component
{
public:
    C_Animator(Actor* owner);
    const static int sortOrder = static_cast<int>(ComponentSortOrder::Animation);

public:
    // �ִϸ��̼� �߰�
    // �Է°� : �ִϸ��̼� ������ 
    void AddAnimation(Animation* input_animation,float input_delayframe);

    // �ִϸ��̼� �� ���� �Լ�
    void SetAnimationVector(vector<Animation*> input_animation, float input_delayframe);

    // ������Ʈ
    virtual void Update(float deltatime) override;
    void C_Update(float deltatime);

    // ����
    virtual void Render(Renderer* renderer) override;

    virtual int GetSortOrder() override;

    // ��ġ ���� ���� ����
    // �Է°� : ��ġ ���� ���� , ��ġ ���� ���� ( ���� ���� true �ÿ��� �ݿ��� )
    void SetAdjust(bool input_set, Vector2D input_amount = Vector2D(0, 0));

    // �ִϸ��̼� ���� ����
    // �Է°� : ���� ���� ����
    void SetAnimationDirection(bool input_isLeft);

    // �ִϸ��̼� ��� ���� ����
    // �Է°� : �ִϸ��̼� ��� ����
    void SetAnimationUse(bool input_isanimation);

    // ���� �ִϸ��̼� ����
    void SetAnimationState(int animation_index);

    // ������ �ִϸ��̼� �� Ư�� �ִϸ��̼� �÷��� ������ �Է°����� ����
    // �Է°� : ��� �ִϸ��̼� �ε��� , ������ ������
    void SetFrameDilay(int input_animationindex,int input_delay);

    // ���� �ִϸ��̼� �� ������ ����
    void SetFrameState(int input_frame);

    // ��ũ�� ��� ���� ����
    void SetIsOnScreen(bool input_isonscreen);

    // ȭ�� ��½� ��ġ
    void SetToScreenPos(Vector2D input_pos);

    // ��������Ʈ ���� ������ ����
    // �Է°� : ��������Ʈ ���α��� , ��������Ʈ ���α���
    void SetWorldSize(float input_width, float input_height);
    void SetWorldSize(Vector2D input_size);

    void RenderOn();

    void RenderOff();

protected:

    bool isRender;

	int ID;                                             // �ִϸ����� ���̵�

	int CurrentAnimation_index;                         // ���� �ִϸ��̼� Ű��
	int CurrentSprite_index;                            // ���� �ִϸ��̼� �� ��������Ʈ �ε���

	int FrameCheck = 0;                                 // Update���� �� �����Ӹ��� 1�� �������� ��

	int SpriteNum;                                      // ��������Ʈ ��ȣ

	//float DelayFrame;                                 // �ִϸ��̼� ��� �ӵ� ( ������ ������ )

	float World_width;                                  // ��������Ʈ ���α���
	float World_height;                                 // ��������Ʈ ���α���

	bool is_Animation;                                  // �ִϸ��̼� ����
    bool is_Left;                                       // �ִϸ��̼� ���� ���� ����
    bool is_AdjustOn;                                   // ��ġ ���� ����
    bool is_OnScreen;                                   // ��ũ���� ��� ����

    Vector2D ToScreenPos;                               // ȭ�� ��½� ��ġ

    Vector2D AdjustAmount;                              // ��ġ ���� ����

    vector<Animation*> m_Animations;                    // �ִϸ��̼� 
};

#endif