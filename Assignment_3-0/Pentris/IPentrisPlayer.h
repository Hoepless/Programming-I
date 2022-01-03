#pragma once

class IPentrisPlayer
{
public:
    virtual ~IPentrisPlayer() {}
	virtual void play() = 0;
	virtual void showEndMessage() = 0;
};
