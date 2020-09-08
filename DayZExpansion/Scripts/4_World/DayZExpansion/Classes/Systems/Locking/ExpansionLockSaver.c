/**
 * ExpansionLockSaver.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLock
{
	int lowBit;
	int highBit;
	string code;
}

class ExpansionLockSaver
{
	protected static autoptr ExpansionLockSaver m_Instance;
	
	static ref ExpansionLockSaver GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new ExpansionLockSaver;
		}
		
		return m_Instance;
	}
	
	protected autoptr array<ref ExpansionLock> m_LockSaved;
	
	void ExpansionLockSaver()
	{
		m_LockSaved = new array<ref ExpansionLock>;
	}
	
	string GetSavedCode(ItemBase item)
	{
		if ( !item || !item.HasNetworkID())
			return "";
		
		int low;
		int high;
		
		item.GetNetworkID( low, high );
		
		for (int i = 0; i < m_LockSaved.Count(); ++i)
		{
			if (!m_LockSaved[i])
				continue;
			
			if (m_LockSaved[i].lowBit != low || m_LockSaved[i].highBit != high)
				continue;
			
			return m_LockSaved[i].code;
		}
		
		return "";
	}
	
	void SaveCode(ItemBase item, string code)
	{
		if ( !item || !item.HasNetworkID())
			return;
		
		int low;
		int high;
		
		item.GetNetworkID( low, high );
		
		for (int i = 0; i < m_LockSaved.Count(); ++i)
		{
			if (!m_LockSaved[i])
				continue;
			
			if (m_LockSaved[i].lowBit != low || m_LockSaved[i].highBit != high)
				continue;
			
			m_LockSaved[i].code = code;
			return;
		}
		
		ExpansionLock lock = new ExpansionLock;
		lock.lowBit = low;
		lock.highBit = high;
		lock.code = code;
		
		m_LockSaved.Insert( lock );
	}
	
	void RemoveCode(ItemBase item)
	{
		if ( !item || !item.HasNetworkID())
			return;
		
		int low;
		int high;
		
		item.GetNetworkID( low, high );
		
		for (int i = 0; i < m_LockSaved.Count(); ++i)
		{
			if (!m_LockSaved[i])
				continue;
			
			if (m_LockSaved[i].lowBit != low || m_LockSaved[i].highBit != high)
				continue;
			
			m_LockSaved.Remove(i);
			return;
		}
	}
}