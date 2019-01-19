#pragma once
#ifndef MAGNUS_CONTROLLIST_H
#define MAGNUS_CONTROLLIST_H

namespace Magnus
{
	struct Point;
	class TextObject;

	namespace Controls
	{
		class Control;
	}

	namespace Event
	{
		class Handler;
		struct Information;
		struct Packet;

		typedef const Magnus::Event::Information & Info;
	}

	template <typename TYPE>
	class TEasyList
	{
	public:

		typedef std::list<TYPE> List;

		void Add(TYPE pControl)
		{
			if (Contains(pControl)) { return; }

			list.push_back(pControl);
		}

		void Remove(TYPE pControl)
		{
			list.remove(pControl);
		}

		void Add(const List & list)
		{
			for (typename List::const_iterator it = list.begin(); it != list.end(); ++it)
			{
				Add(*it);
			}
		}

		void Add(const TEasyList<TYPE> & list)
		{
			Add(list.list);
		}

		bool Contains(TYPE pControl) const
		{
			typename List::const_iterator it = std::find(list.begin(), list.end(), pControl);
			return it != list.end();
		}

		inline void Clear()
		{
			list.clear();
		}

		List list;
	};

	class ControlList : public TEasyList<Magnus::Controls::Control*>
	{
	public:

		void Enable();
		void Disable();

		void Show();
		void Hide();

		Magnus::TextObject GetValue();
		void SetValue(const Magnus::TextObject & value);

		template<typename T>
		void SetAction(Magnus::Event::Handler* ob,
			void (T::*f)(Magnus::Event::Info),
			const Magnus::Event::Packet & packet)
		{
			SetActionInternal(ob,
				static_cast<void (Magnus::Event::Handler*)(Magnus::Event::Info)>(f),
				packet);
		}

		void MoveBy(const Magnus::Point & point);

		void DoAction();

	protected:

		void SetActionInternal(Magnus::Event::Handler* pObject,
			void (Magnus::Event::Handler::*f)(Magnus::Event::Info),
			const Magnus::Event::Packet & packet);
	};
}

#endif