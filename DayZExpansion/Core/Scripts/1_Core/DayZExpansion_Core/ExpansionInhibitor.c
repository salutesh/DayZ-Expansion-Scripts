/**
 * ExpansionInhibitor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionInhibitor: Managed
{
	protected static ref array<Class> s_Instances = new array<Class>;

	protected Class m_Instance;

	private void ExpansionInhibitor(Class instance)
	{
		m_Instance = instance;
		s_Instances.Insert(instance);
	}

	void ~ExpansionInhibitor()
	{
		int index = s_Instances.Find(m_Instance);
		if (index > -1)
			s_Instances.Remove(index);
	}

	static ExpansionInhibitor Add(Class instance)
	{
		return new ExpansionInhibitor(instance);
	}

	static bool Contains(Class instance)
	{
		return s_Instances.Find(instance) > -1;
	}
}
