//! Literally a verbatim copy of vanilla CAContinuousFillOil
class CAContinuousExpansionFillHydraulicFluid: CAContinuousBase
{
	protected float 				m_ItemQuantity;
	protected float 				m_SpentQuantity;
	protected float 				m_SpentQuantity_total;
	protected float 				m_EmptySpace;  //! basically free capacity
	protected float					m_TimeElpased;
	protected float 				m_QuantityUsedPerSecond;
	protected float 				m_AdjustedQuantityUsedPerSecond;
	protected float 				m_DefaultTimeStep;
	protected ref Param1<float>		m_SpentUnits;

	protected PlayerBase 			m_Player;

	void CAContinuousExpansionFillHydraulicFluid(float quantity_used_per_second, float time_to_progress)
	{
		m_QuantityUsedPerSecond = quantity_used_per_second;
		m_DefaultTimeStep = time_to_progress;
	}

	override void Setup(ActionData action_data)
	{
		m_Player = action_data.m_Player;
		
		Car car = Car.Cast(action_data.m_Target.GetObject());
		
		if (!car)
			return;
		
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

		float oilCapacity = car.GetFluidCapacity(CarFluid.OIL);
		float currentOil = car.GetFluidFraction(CarFluid.OIL);
		currentOil = currentOil * oilCapacity;

		m_EmptySpace = (oilCapacity - currentOil) * 1000;
		m_ItemQuantity = action_data.m_MainItem.GetQuantity();

		if (m_EmptySpace <= m_ItemQuantity)
			m_ItemQuantity = m_EmptySpace;
	}

	override int Execute(ActionData action_data)
	{
		Car car = Car.Cast(action_data.m_Target.GetObject());

		if (!car)
			return UA_ERROR;

		if (!action_data.m_Player)
		{
			return UA_ERROR;
		}
		
		if (m_ItemQuantity <= 0)
		{
			return UA_FINISHED;
		}
		else
		{
			if (m_SpentQuantity_total < m_ItemQuantity)
			{
				m_AdjustedQuantityUsedPerSecond = m_QuantityUsedPerSecond;
				m_SpentQuantity += m_AdjustedQuantityUsedPerSecond * action_data.m_Player.GetDeltaT();
				m_TimeElpased += action_data.m_Player.GetDeltaT();
				
				if (m_TimeElpased >= m_DefaultTimeStep)
				{
					CalcAndSetQuantity( action_data );
					m_TimeElpased = 0;
					//Setup(action_data);  //! reset data after repeat
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
	}

	override int Cancel(ActionData action_data)
	{
		if (!action_data.m_Player)
		{
			return UA_ERROR;
		}

		CalcAndSetQuantity( action_data );
		return UA_CANCEL;
	}	

	override float GetProgress()
	{
		if (m_ItemQuantity <= 0)
			return 1;

		return -(m_SpentQuantity_total / m_ItemQuantity);
	}
	
	void CalcAndSetQuantity(ActionData action_data)
	{
		m_SpentQuantity_total += m_SpentQuantity;
	
		if (m_SpentUnits)
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}

		if (g_Game.IsServer())
		{
			if (action_data.m_MainItem)  //! Item gets deleted after full consumption
				action_data.m_MainItem.AddQuantity(-m_SpentQuantity);
			
			Car car = Car.Cast(action_data.m_Target.GetObject());	
			car.Fill(CarFluid.OIL, (m_SpentQuantity * 0.001));
		}

		m_SpentQuantity = 0;
	}
}
