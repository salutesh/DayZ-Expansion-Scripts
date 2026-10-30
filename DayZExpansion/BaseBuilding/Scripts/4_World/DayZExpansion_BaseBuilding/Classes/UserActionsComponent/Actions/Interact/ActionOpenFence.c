/**
 * ActionOpenFence.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionOpenFence
{
	bool m_Expansion_HasCodeLock;
	bool m_Expansion_CanOpen;
	bool m_Expansion_IsLocked;
	bool m_Expansion_HasGate;

	override string GetText()
	{
		string text = super.GetText();

		if (m_Expansion_IsLocked)
		{
			if (m_Expansion_HasGate)
				text = "#STR_EXPANSION_OPEN_LOCKED_GATE";
			else
				text = "#STR_EXPANSION_OPEN_LOCKED_DOOR";

			if (GetPermissionsManager().IsAdminToolsToggledOn())
				text = "[ADMIN] " + text;
		}

		return text;
	}

	override bool Can(PlayerBase player, ActionTarget target, ItemBase item, int condition_mask)
	{
		Object targetObject = target.GetObject();
		Fence fence = Fence.Cast( targetObject );

		if (fence)
		{
			m_Expansion_HasCodeLock = fence.ExpansionHasCodeLock();

			if (m_Expansion_HasCodeLock)
			{
				m_Expansion_CanOpen = fence.ExpansionCanOpen( player, "" );

				if (fence.ExpansionIsLocked())
					m_Expansion_IsLocked = true;
				else
					m_Expansion_IsLocked = false;

				if (m_Expansion_IsLocked && m_Expansion_CanOpen)
				{
					string type = fence.GetType();
					type.ToLower();

					//! https://feedback.bistudio.com/T173348
					if (type == "fence" || type.Contains("gate"))
						m_Expansion_HasGate = true;
					else
						m_Expansion_HasGate = false;
				}
			}
			else
			{
				m_Expansion_IsLocked = false;
			}
		}
		else
		{
			m_Expansion_HasCodeLock = false;
			m_Expansion_CanOpen = false;
		}

		return super.Can(player, target, item, condition_mask);
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( m_Expansion_HasCodeLock )
			return m_Expansion_CanOpen;

		return super.ActionCondition( player, target, item );
	}
	
	override void OnStartServer( ActionData action_data )
	{
		Fence fence = Fence.Cast( action_data.m_Target.GetObject() );
		if ( !fence || ( fence.ExpansionHasCodeLock() && !fence.ExpansionCanOpen( action_data.m_Player, "" ) ) )
			return;

		super.OnStartServer( action_data );
	}
}
