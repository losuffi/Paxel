#pragma once
#include "Event.h"


class PAXEL_API KeyEvent :public Event
{
public:
	inline int Getint() const { return m_int; }
	EVENT_CLASS_CATEGORY(EventCategoryInput|EventCategoryKeyboard)
protected:
	KeyEvent(int code) :m_int(code){}
	int m_int;
};

class PAXEL_API KeyPresssedEvent : public KeyEvent
{
public:
	KeyPresssedEvent(int code):KeyEvent(code){}
	EVENT_CLASS_TYPE(KeyPressed)
};
