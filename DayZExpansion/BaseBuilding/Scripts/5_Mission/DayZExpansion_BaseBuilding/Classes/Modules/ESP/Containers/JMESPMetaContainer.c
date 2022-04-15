/**
 * JMESPMetaContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
class JMESPMetaContainer : JMESPMeta
{
	ExpansionESPModificationModule exp_Module;

	UIActionText m_Action_Code;

	UIActionButton m_Action_RequestCode;
	UIActionButton m_Action_RemoveCode;

	override void Create( JMESPModule mod )
	{
		CF_Modules<ExpansionESPModificationModule>.Get(exp_Module);

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
	}

	bool HasCodeLock()
	{
		ItemBase item;
		if ( Class.CastTo( item, target ) && item.HasCode() )
		{
			return true;
		}

		return false;
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
#endif
