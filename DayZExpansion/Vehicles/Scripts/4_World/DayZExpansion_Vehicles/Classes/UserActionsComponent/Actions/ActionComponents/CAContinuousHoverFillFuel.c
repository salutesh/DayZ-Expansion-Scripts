class CAContinuousHoverFillFuel : CAContinuousBase
{
	protected float 				m_SpentQuantity;
	protected float 				m_SpentQuantity_total;
	protected float 				m_EmptySpace; //basically free capacity
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_AdjustedQuantityUsedPerSecond;
	protected float 				m_DefaultTimeStep;
	protected ref Param1<float>		m_SpentUnits;
	
	void CAContinuousHoverFillFuel( float quantity_used_per_second, float time_to_progress )
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeStep = time_to_progress;
	}
	
	//---------------------------------------------------------------------------
	override void Setup( ActionData action_data )
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Player);
		
		m_TimeElpased = 0;
		m_SpentQuantity = 0;
		
		if ( !m_SpentUnits )
		{
			m_SpentUnits = new Param1<float>( 0 );
		}
		else
		{
			m_SpentUnits.param1 = 0;
		}
		
		float fuelCapacity;
		float currentFuel;
		if ( vehicle )
		{
			m_QuantityUsedPerSecond *= vehicle.GetEntity().GetLiquidThroughputCoef();
			
			fuelCapacity = vehicle.GetFluidCapacity( CarFluid.FUEL );
			currentFuel = vehicle.GetFluidFraction( CarFluid.FUEL );
		}
		currentFuel = currentFuel * fuelCapacity;

		m_EmptySpace = (fuelCapacity - currentFuel) * 1000;
	}

	//---------------------------------------------------------------------------
	override int Execute( ActionData action_data  )
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		if ( m_EmptySpace > m_SpentQuantity_total )
		{
			m_AdjustedQuantityUsedPerSecond = action_data.m_Player.GetSoftSkillsManager().SubtractSpecialtyBonus( m_QuantityUsedPerSecond, m_Action.GetSpecialtyWeight(), true);
			m_SpentQuantity += m_AdjustedQuantityUsedPerSecond * action_data.m_Player.GetDeltaT();
			m_TimeElpased += action_data.m_Player.GetDeltaT();
			
			if ( m_TimeElpased >= m_DefaultTimeStep )
			{
				CalcAndSetQuantity( action_data );
				m_TimeElpased = 0;
			}
			
			return UA_PROCESSING;
		}
		else
		{
			CalcAndSetQuantity( action_data );
			OnCompletePogress(action_data);
			return UA_FINISHED;
		}
	}
	
	//---------------------------------------------------------------------------
	override int Cancel( ActionData action_data )
	{
		if ( !action_data.m_Player )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity( action_data );
		return UA_INTERRUPT;
	}
	
	//---------------------------------------------------------------------------
	override float GetProgress()
	{
		return -(m_SpentQuantity_total / m_EmptySpace);
	}
	
	//---------------------------------------------------------------------------
	void CalcAndSetQuantity( ActionData action_data )
	{
		m_SpentQuantity_total += m_SpentQuantity;
	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		
		
		if ( GetGame().IsServer() )
		{
			auto vehicle = ExpansionVehicle.Get(action_data.m_Player);
			
			if ( vehicle )
				vehicle.FillFluid( CarFluid.FUEL, (m_SpentQuantity * 0.001) );
		}
		
		m_SpentQuantity = 0;
	}
}