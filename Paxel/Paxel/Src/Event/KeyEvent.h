#pragma once
#include "Event.h"

enum KeyCode
{
	None = 0,
	W,
	A,
	S,
	D,
};

class PAXEL_API KeyEvent :public Event
{
public:
	inline KeyCode GetKeyCode() const { return m_KeyCode; }
	EVENT_CLASS_CATEGORY(EventCategoryInput|EventCategoryKeyboard)
protected:
	KeyEvent(KeyCode code) :m_KeyCode(code){}
	KeyCode m_KeyCode;
};

