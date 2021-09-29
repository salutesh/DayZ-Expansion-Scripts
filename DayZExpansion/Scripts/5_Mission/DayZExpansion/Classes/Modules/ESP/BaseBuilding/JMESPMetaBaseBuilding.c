/**
 * JMESPMetaBaseBuilding.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class JMESPMetaBaseBuilding
{
	ExpansionESPModificationModule exp_Module;

	UIActionText m_Action_Code;

	UIActionButton m_Action_RequestCode;
	UIActionButton m_Action_RemoveCode;

	override void Create( JMESPModule mod )
	{
		Class.CastTo( exp_Module, GetModuleManager().GetModule( ExpansionESPModificationModule ) );

		super.Create( mod );
	}
	

	override void CreateActions( Widget parent )
	{
		super.CreateActions( parent );

		if ( HasCodeLock() )
		{
			UIActionManager.CreatePanel( parent, 0xFF000000, 1 );

			m_Action_Code = UIActionManager.CreateText( parent, "Codelock: ", "UNKNOWN CODE" );
			m_Action_RequestCode = UIActionManager.CreateButton( parent, "Refresh Code", this, "Action_RequestCode" );
			m_Action_RemoveCode = UIActionManager.CreateButton( parent, "Remove Code", this, "Action_RemoveCode" );
		}

		UpdateButtonStates();
	}

	bool HasCodeLock()
	{
		ExpansionWallBase expwall;
		Fence fence;
		if ( Class.CastTo( expwall, target ) )
		{
			return ( expwall.ExpansionHasCodeLock( "codelock_door" ) || expwall.ExpansionHasCodeLock( "codelock_gate" ) );
		}
		else if ( Class.CastTo( fence, target ) )
		{
			if ( CanAttachCodelockFence() )
			{
				return fence.ExpansionHasCodeLock( "codelock" );
			}
		}

		return false;
	}

	//! Only call this after settings have been loaded
	bool CanAttachCodelockFence()
	{
		int attachMode = GetExpansionSettings().GetBaseBuilding().CodelockAttachMode;
		return attachMode == ExpansionCodelockAttachMode.ExpansionAndFence || attachMode == ExpansionCodelockAttachMode.ExpansionAndFenceAndTents;
	}

	override private void UpdateButtonStates()
	{
		super.UpdateButtonStates();

		for ( int i = 0; i < m_StateHeaders.Count(); ++i )
		{
			JMConstructionPartData data;
			Class.CastTo( data, m_StateHeaders[i].GetData() );

			if ( data.m_State == JMConstructionPartState.CONFLICTING_PART )
			{
				m_StateHeaders[i].Hide();
				m_BuildButtons[i].Hide();
				m_DismantleButtons[i].Hide();
				m_RepairButtons[i].Hide();
			} else if ( data.m_State == JMConstructionPartState.REQUIRED_PART_NOT_BUILT )
			{
				m_StateHeaders[i].Hide();
				m_BuildButtons[i].Hide();
				m_DismantleButtons[i].Hide();
				m_RepairButtons[i].Hide();
			} else {
				m_StateHeaders[i].Show();
			}
		}
	}

	void SetCode( string code )
	{
		m_Action_Code.SetText( code );
	}

	void Action_RequestCode( UIEvent eid, UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		exp_Module.RequestCode( target );
	}

	void Action_RemoveCode( UIEvent eid, UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		exp_Module.RemoveCode( target );
	}
};