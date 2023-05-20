/**
 * ZombieBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ZombieBase
{
	static ref CF_DoublyLinkedNodes_WeakRef<ZombieBase> s_Expansion_AllInfected = new CF_DoublyLinkedNodes_WeakRef<ZombieBase>();
	ref CF_DoublyLinkedNode_WeakRef<ZombieBase> m_Expansion_InfectedNode = new CF_DoublyLinkedNode_WeakRef<ZombieBase>(this);

	void ZombieBase()
	{
		m_Expansion_InfectedNode = s_Expansion_AllInfected.Add(this);
	}

	void ~ZombieBase()
	{
		if (s_Expansion_AllInfected)
			s_Expansion_AllInfected.Remove(m_Expansion_InfectedNode);
	}

	override bool CanBeSkinned()
	{
		return true;
	}
};