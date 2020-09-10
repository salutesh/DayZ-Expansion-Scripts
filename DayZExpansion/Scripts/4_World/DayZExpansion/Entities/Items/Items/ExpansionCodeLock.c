/**
 * ExpansionCodeLock.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCodeLock
 * @brief		
 **/
class ExpansionCodeLock extends ItemBase
{
	
	private ref set<string> m_AuthorizedPlayers;
	
	void ExpansionCodeLock()
	{
		m_AuthorizedPlayers = new set<string>();
	}
	
	void ~ExpansionCodeLock() 
	{
		delete m_AuthorizedPlayers;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionAttachToConstruction);
	}
	
	override bool CanDetachAttachment(EntityAI parent)
	{
		if ( !super.CanDetachAttachment( parent ) )
		{
			return false;
		}
		
		ItemBase target = ItemBase.Cast( parent );

		if ( target && target.IsLocked() )
		{
			return false;
		}
		
		return true;
	}
	
	void AddAuthorizedPlayer(string player)
	{
		m_AuthorizedPlayers.Insert(player);
	}
	
	void RemoveAuthorizedPlayer(string player)
	{
		m_AuthorizedPlayers.Remove(m_AuthorizedPlayers.Find(player));
	}
	
	void ClearAuthorizedPlayers()
	{
		m_AuthorizedPlayers.Clear();
	}
	
	
	bool IsAuthorizedPlayer(string player)
	{
		return (m_AuthorizedPlayers.Find(player) != -1);
	}
}