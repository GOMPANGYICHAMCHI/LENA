#include "Pch.h"
#include "Animation.h"

Animation::Animation()
{
	isloop = true;
}

void Animation::Update(float deltatime)
{
	
}

void Animation::AddSprite(Sprite* sprite)
{
	sprites.push_back(sprite);
}

void Animation::SetDelayFrame(int input_delay)
{
	delayframe = input_delay;
}
