#include "Magnus/Magnus.h"
#include "Magnus/ControlList.h"

using namespace Magnus;
using namespace Magnus::Controls;

void ControlList::Enable()
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->SetDisabled(false);
	}
}

void ControlList::Disable()
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->SetDisabled(true);
	}
}

void ControlList::Show()
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->Show();
	}
}

void ControlList::Hide()
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->Hide();
	}
}

Magnus::TextObject ControlList::GetValue()
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		return (*it)->GetValue();
	}

	return "";
}

void ControlList::SetValue(const Magnus::TextObject & value)
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->SetValue(value);
	}
}

void ControlList::MoveBy(const Magnus::Point & point)
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->MoveBy(point.x, point.y);
	}
}

void ControlList::DoAction()
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->DoAction();
	}
}

void ControlList::SetActionInternal(Magnus::Event::Handler* pObject, void (Magnus::Event::Handler::*f)(Magnus::Event::Info), const Magnus::Event::Packet & packet)
{
	for (List::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		(*it)->SetAction(pObject, f, packet);
	}
}