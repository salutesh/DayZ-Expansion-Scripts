class ExpansionAINoGoArea: EffectArea
{
	ExpansionAINoGoAreaTrigger m_Expansion_Trigger;

	void Expansion_Init(ExpansionAINoGoAreaConfig config)
	{
		m_Radius = config.Radius;
		m_PositiveHeight = config.Height;
		m_NegativeHeight = 0;
		m_Position = config.Position;

		CreateTrigger(m_Position, m_Radius);

		s_Expansion_DangerousAreas.UpdateClusters(this);
	}

	override void CreateTrigger(vector pos, int radius)
	{
		if (Class.CastTo(m_Expansion_Trigger, g_Game.CreateObjectEx("ExpansionAINoGoAreaTrigger", pos, ECE_NONE)))
		{
			m_Expansion_Trigger.SetCollisionCylinder(radius, m_PositiveHeight);
			m_Expansion_Trigger.Expansion_Init(this);
		}
	}

	override void EEDelete(EntityAI parent)
	{
		if (m_Expansion_Trigger)
			g_Game.ObjectDelete(m_Expansion_Trigger);

		super.EEDelete(parent);
	}

	//! unused
	override void SetupZoneData(EffectAreaParams params) 
	{
	}
	
	//! unused
	override void OnPlayerEnterServer(PlayerBase player, EffectTrigger trigger)
	{
	}

	//! unused
	override void OnPlayerExitServer(PlayerBase player, EffectTrigger trigger)
	{
	}

	override bool eAI_IsDangerousToAI(eAIBase ai)
	{
		return true;  //! Always avoid this area
	}
}

class ExpansionAINoGoAreaTrigger: CylinderTrigger
{
	EffectArea m_Expansion_EffectArea;

	void Expansion_Init(EffectArea area)
	{
		m_Expansion_EffectArea = area;
	}

	override protected bool CanAddObjectAsInsider(Object object)
	{
		if (eAIBase.Cast(object))
			return true;

		return false;
	}

	override bool ShouldRemoveInsider(TriggerInsider insider)
	{
		return !insider.GetObject().IsAlive();
	}

	override void OnEnterServerEvent(TriggerInsider insider)
	{
		super.OnEnterServerEvent(insider);
		
		if (insider)
		{
			eAIBase ai;
			
			if (Class.CastTo(ai, insider.GetObject()))
				ai.Expansion_OnDangerousAreaEnterServer(m_Expansion_EffectArea, this);
		}
	}

	override void OnLeaveServerEvent(TriggerInsider insider)
	{
		super.OnLeaveServerEvent(insider);
		
		if (insider)
		{
			eAIBase ai;
			
			if (Class.CastTo(ai, insider.GetObject()))
				ai.Expansion_OnDangerousAreaExitServer(m_Expansion_EffectArea, this);
		}
	}
}
