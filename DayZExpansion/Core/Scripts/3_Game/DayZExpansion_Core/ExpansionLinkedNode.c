/**
 * ExpansionLinkedNode.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_DOUBLYLINKEDNODES
//! @brief a collection of doubly linked nodes (doubly linked list)
class CF_DoublyLinkedNodes<Class T>
{
	ref CF_DoublyLinkedNode<T> m_Head;
	CF_DoublyLinkedNode<T> m_Current;

	int m_Count;

	void ~CF_DoublyLinkedNodes()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(true, this);
#endif

		m_Head = null;
	}

	CF_DoublyLinkedNode<T> Add(T value)
	{
		auto node = new CF_DoublyLinkedNode<T>(value);

		Add(node);

		return node;
	}

	void Add(CF_DoublyLinkedNode<T> node)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(true, this).Add("" + node);
#endif

		node.Link(m_Head, null);

		m_Head = node;

		m_Count++;

#ifdef EXPANSIONTRACE
		trace.Debug("Count: %1", "" + m_Count);
#endif
	}

	void Remove(CF_DoublyLinkedNode<T> node)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(true, this).Add("" + node);
#endif

		if (!node)
			return;

		bool isHead = m_Head == node;

		if (!isHead && node.m_Next == null && node.m_Prev == null)
			return;

		auto next = node.m_Next;

		delete node;

		if (isHead)
		{
#ifdef EXPANSIONTRACE
			trace.Debug("Assigning %1 as head node", "" + next);
#endif
			m_Head = next;
		}

		m_Count--;

#ifdef EXPANSIONTRACE
		trace.Debug("Count: %1", "" + m_Count);
#endif
	}

	//! @brief processes elements in the linked list.
	//! if `limit` is set to a nonzero value, only `n` elements are processed each call.
	//! The value of each node is passed to the callback.
	//! Returns the number of processed elements.
	int Each(ScriptCaller callback, int limit = 0)
	{
		if (!m_Current)
			m_Current = m_Head;

		//! @note: Enforce garbage collects references to function arguments inside loops after the first iteration,
		//! so we have to assign a local variable and use that instead to avoid premature garbage collection
		ScriptCaller localCallback = callback;

		int count;
		while (m_Current)
		{
			localCallback.Invoke(m_Current.m_Value);
			m_Current = m_Current.m_Next;
			count++;
			if (limit > 0 && count == limit)
				break;
		}

		return count;
	}
}

class CF_DoublyLinkedNode<Class T>
{
	ref CF_DoublyLinkedNode<T> m_Next;
	ref CF_DoublyLinkedNode<T> m_Prev;

	ref T m_Value;

	void CF_DoublyLinkedNode(T value)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(true, this).Add("" + value);
#endif

		m_Value = value;
	}

	void ~CF_DoublyLinkedNode()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(true, this);
#endif

		Unlink();
	}

	void Link(CF_DoublyLinkedNode<T> next, CF_DoublyLinkedNode<T> prev)
	{
		if (next == this || prev == this || (next != null && next == prev))
		{
			Error(string.Format("Trying to create circular reference (next=%1, prev=%2)", next, prev));
			return;
		}

		m_Next = next;
		m_Prev = prev;

		if (next)
			next.m_Prev = this;

		if (prev)
			prev.m_Next = this;
	}

	void Unlink()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(true, this);
#endif

		T value;
		m_Value = value;

		if (m_Next)
		{
#ifdef EXPANSIONTRACE
			trace.Debug("Assigning %1 as prev node to next node %2", "" + m_Prev, "" + m_Next);
#endif
			m_Next.m_Prev = m_Prev;
		}

		if (m_Prev)
		{
#ifdef EXPANSIONTRACE
			trace.Debug("Assigning %1 as next node to prev node %2", "" + m_Next, "" + m_Prev);
#endif
			m_Prev.m_Next = m_Next;
		}

		m_Next = null;
		m_Prev = null;
	}
}
#endif

#ifdef EXPANSION_DOUBLYLINKEDNODES_WEAKREF
class CF_DoublyLinkedNodes_WeakRef<Class T>
{
	ref CF_DoublyLinkedNode_WeakRef<T> m_Head;
	CF_DoublyLinkedNode_WeakRef<T> m_Current;

	int m_Count;

	void ~CF_DoublyLinkedNodes_WeakRef()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(true, this);
#endif

		m_Head = null;
	}

	CF_DoublyLinkedNode_WeakRef<T> Add(T value)
	{
		auto node = new CF_DoublyLinkedNode_WeakRef<T>(value);

		Add(node);

		return node;
	}

	void Add(CF_DoublyLinkedNode_WeakRef<T> node)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(true, this).Add("" + node);
#endif

		node.Link(m_Head, null);

		m_Head = node;

		m_Count++;

#ifdef EXPANSIONTRACE
		trace.Debug("Count: %1", "" + m_Count);
#endif
	}

	void Remove(CF_DoublyLinkedNode_WeakRef<T> node)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(true, this).Add("" + node);
#endif

		if (!node)
			return;

		bool isHead = m_Head == node;

		if (!isHead && node.m_Next == null && node.m_Prev == null)
			return;

		auto next = node.m_Next;

		delete node;

		if (isHead)
		{
#ifdef EXPANSIONTRACE
			trace.Debug("Assigning %1 as head node", "" + next);
#endif
			m_Head = next;
		}

		m_Count--;

#ifdef EXPANSIONTRACE
		trace.Debug("Count: %1", "" + m_Count);
#endif
	}

	//! @brief processes elements in the linked list.
	//! if `limit` is set to a nonzero value, only `n` elements are processed each call.
	//! The value of each node is passed to the callback.
	//! Returns the number of processed elements.
	int Each(ScriptCaller callback, int limit = 0)
	{
		if (!m_Current)
			m_Current = m_Head;

		//! @note: Enforce garbage collects references to function arguments inside loops after the first iteration,
		//! so we have to assign a local variable and use that instead to avoid premature garbage collection
		ScriptCaller localCallback = callback;

		int count;
		while (m_Current)
		{
			localCallback.Invoke(m_Current.m_Value);
			m_Current = m_Current.m_Next;
			count++;
			if (limit > 0 && count == limit)
				break;
		}

		return count;
	}
}

class CF_DoublyLinkedNode_WeakRef<Class T>
{
	ref CF_DoublyLinkedNode_WeakRef<T> m_Next;
	ref CF_DoublyLinkedNode_WeakRef<T> m_Prev;

	T m_Value;

	void CF_DoublyLinkedNode_WeakRef(T value)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(true, this).Add("" + value);
#endif

		m_Value = value;
	}

	void ~CF_DoublyLinkedNode_WeakRef()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(true, this);
#endif

		Unlink();
	}

	void Link(CF_DoublyLinkedNode_WeakRef<T> next, CF_DoublyLinkedNode_WeakRef<T> prev)
	{
		if (next == this || prev == this || (next != null && next == prev))
		{
			Error(string.Format("Trying to create circular reference (next=%1, prev=%2)", next, prev));
			return;
		}

		m_Next = next;
		m_Prev = prev;

		if (next)
			next.m_Prev = this;

		if (prev)
			prev.m_Next = this;
	}

	void Unlink()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(true, this);
#endif

		T value;
		m_Value = value;

		if (m_Next)
		{
#ifdef EXPANSIONTRACE
			trace.Debug("Assigning %1 as prev node to next node %2", "" + m_Prev, "" + m_Next);
#endif
			m_Next.m_Prev = m_Prev;
		}

		if (m_Prev)
		{
#ifdef EXPANSIONTRACE
			trace.Debug("Assigning %1 as next node to prev node %2", "" + m_Next, "" + m_Prev);
#endif
			m_Prev.m_Next = m_Next;
		}

		m_Next = null;
		m_Prev = null;
	}
}
#endif
