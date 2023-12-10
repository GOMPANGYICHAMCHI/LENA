#pragma once

#include "Sprite.h"
#include "Vector2D.h"

#include "Component.h"
#include "Renderer.h"

// ��������Ʈ ������Ʈ
// �ִϸ��̼��� ���� �ʴ� ������ ��ü�� ���� �̹��� ������ ������Ʈ
class C_Sprite : public Component
{
public:
	C_Sprite(Actor* owner);
	const static int sortOrder = static_cast<int>(ComponentSortOrder::Sprite);

	// ������Ʈ 
	virtual void Update(float deltaTime) override;
	// ����
	virtual void Render(Renderer* renderer) override;

	virtual int GetSortOrder() override;

	// ��ġ ���� ���� ����
	// �Է°� : ��ġ ���� ����
	void SetAdjustAmount(Vector2D input_amount);

	// ��ġ ���� ���� ���� ( ȣ��� �ڵ����� ������ ON �˴ϴ� )
	// �Է°� : ��ġ ���� ���� ����
	void SetAdjustPos(bool input_set);

	// �Ǻ� ��ȯ �Լ�
	Vector2D GetPivot();

	virtual void SetSource(Sprite*);

	// �̹��� ������ ����
	void SetImageSize(Vector2D input_size);

	// ��������Ʈ ���� Ȱ��ȭ
	void RenderOn();

	void RenderOff();

protected:
	// ���� ���� 
	bool isrender;
	// ������ ���� ����
	bool isadjustpos;

	// ������ ���� ����
	Vector2D adjustamount;

	// �̹��� ���α���
	float World_width;
	// �̹��� ���α���
	float World_height;

	// ���� �ҽ� �ּҸ� �������ִ� ��������Ʈ ��ü
	Sprite* Source;
};

