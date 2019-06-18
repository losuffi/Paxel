#pragma once
#include "PXPCH.h"
enum class EventType
{
	NoneType = 0,
	KeyPressed,
	KeyReleased,
	KeyClicked,
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
class PAXEL_API Event
{
public:
	virtual const char* GetEventName() const = 0;
	virtual EventType GetEventType() const = 0;
	virtual int GetCategoryFlags() const = 0;
	inline bool IsInCategory(EventCategory category) { return GetCategoryFlags()&category; }
};
