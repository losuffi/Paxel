#pragma once
#include "PXPCH.h"
enum class EventType
{
	NoneType = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	KeyPressed, KeyReleased, KeyClicked,
};
enum EventCategory
{
	CategoryNone = 0,
	EventCategoryApplication = 1,
	EventCategoryInput = 1<<1,
	EventCategoryKeyboard = 1<<2,
	EventCategoryMouse = 1<<3,
	EventCategoryMouseButton = 1<<4
};
#define  EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::##type;} \
virtual EventType GetEventType() const override{return GetStaticType();}\
virtual const char* GetEventName() const override{return #type;}
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}

#define EVENT_REGISTER_SIMPLE(name,category,type)\
class PAXEL_API name: public Event\
{\
public:\
	EVENT_CLASS_CATEGORY(category)\
	EVENT_CLASS_TYPE(type)\
};
class PAXEL_API Event
{
public:
	virtual const char* GetEventName() const = 0;
	virtual EventType GetEventType() const = 0;
	virtual int GetCategoryFlags() const = 0;
	inline bool IsInCategory(EventCategory category) { return GetCategoryFlags()&category; }
	bool Handled = false;
};
static void TestFunc(const char** args)
{
	auto a = std::atof(args[0]);
	auto b = std::atof(args[1]);
	auto c = a + b;
	Log::GetCoreLogger()->info(c);
}

enum DispatchName
{
	EVENT_DN_FIRST = 0,
	EVENT_DN_TEST,
};
template<typename T>
class PAXEL_API EventDispatcher
{
	using EventFn = std::function<bool(T&)>;
public:

	EventDispatcher(Event& event)
		: m_event(event)
	{
	}
	bool Dispatch(EventFn Fn)
	{
		if(m_event.GetEventType() == T::GetStaticType())
		{
			m_event.Handled = Fn(*static_cast<T*>(&m_event));
			return true;
		}
		return false;
	}

private:
	Event& m_event;
};
