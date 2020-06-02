class ExpansionAITestForm extends JMFormBase
{
	private Widget m_ActionsWrapper;
	private ExpansionAIBase m_AI;

	void ExpansionAITestForm()
	{
    }

	void ~ExpansionAITestForm()
	{
	}

	override void OnInit()
	{
		m_ActionsWrapper = layoutRoot.FindAnyWidget( "actions_wrapper" );

        UIActionManager.CreateButton( m_ActionsWrapper, "Spawn AKM", this, "SpawnAKM" );
        UIActionManager.CreateButton( m_ActionsWrapper, "Spawn Soldier", this, "SpawnSoldier" );
	}

	private vector GetPointerPosition()
	{
		if ( !GetGame().GetPlayer() )
		{
			return "0 0 0";
		}

		vector position = GetGame().GetCurrentCameraPosition();
		vector rayStart = position + GetGame().GetCurrentCameraDirection() * 0.1;
		vector rayEnd = position + GetGame().GetCurrentCameraDirection() * 20.0;

		Object hitObj;
		vector hitPosition;
		vector hitNormal;
		float hitFraction;

		if ( !DayZPhysics.RayCastBullet( rayStart, rayEnd, 0xFFFFFFFF, GetGame().GetPlayer(), hitObj, hitPosition, hitNormal, hitFraction ) )
			hitPosition = rayEnd;

		return hitPosition;
	}

	void SpawnAKM( UIEvent eid, ref UIActionBase action ) 
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		if ( !player )
			return;
			
		player.CreateInInventory( "AKM" );
		player.CreateInInventory( "Mag_AKM_Drum75Rnd" );
	}

	void SpawnSoldier( UIEvent eid, ref UIActionBase action ) 
	{
		Class.CastTo( m_AI, GetGame().CreatePlayer( NULL, "ExpansionSoldierGabi", GetPointerPosition(), 0, "NONE" ) );
	}
}