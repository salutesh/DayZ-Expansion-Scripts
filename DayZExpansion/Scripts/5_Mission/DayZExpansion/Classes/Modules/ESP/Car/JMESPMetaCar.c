/**
 * JMESPMetaCar.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
class JMESPMetaCar : JMESPMeta
{
	ExpansionESPModificationModule exp_Module;

	UIActionText m_Action_Code;

	UIActionButton m_Action_UnPair;
	UIActionButton m_Action_UnLock;

	override void Create( JMESPModule mod )
	{
		Class.CastTo( exp_Module, GetModuleManager().GetModule( ExpansionESPModificationModule ) );

		super.Create( mod );
	}

	override void CreateActions( Widget parent )
	{
		super.CreateActions( parent );

		CarScript car;
		if ( Class.CastTo( car, target ) )
		{
			if ( car.HasKey() )
			{
				UIActionManager.CreatePanel( parent, 0xFF000000, 1 );

				m_Action_UnPair = UIActionManager.CreateButton( parent, "Unpair", this, "Action_UnPair" );

				if ( car.IsLocked() )
				{
					m_Action_UnLock = UIActionManager.CreateButton( parent, "Unlock", this, "Action_UnLock" );
				}
			}
		}
	}

	void Action_UnPair( UIEvent eid, UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		exp_Module.CarUnPair( target );
	}

	void Action_UnLock( UIEvent eid, UIActionBase action )
	{
		if ( eid != UIEvent.CLICK )
			return;

		exp_Module.CarUnLock( target );
	}
};