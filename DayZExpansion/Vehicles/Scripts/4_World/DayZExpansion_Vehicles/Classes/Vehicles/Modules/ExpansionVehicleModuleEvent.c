/*
 * ExpansionVehicleModuleEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleModuleEvent
{
	ExpansionVehicleModule m_Value;

	ref ExpansionVehicleModuleEvent m_Next;

	void ExpansionVehicleModuleEvent(ExpansionVehicleModule module = null)
	{
		m_Value = module;
	}

	void Add(ExpansionVehicleModule module)
	{
		ExpansionVehicleModuleEvent newEvent = new ExpansionVehicleModuleEvent(module);

		ExpansionVehicleModuleEvent current = this;
		ExpansionVehicleModuleEvent next = m_Next;
		while (current && next)
		{
			if (current == module)
				return;

			current = next;
			next = next.m_Next;
		}

		current.m_Next = newEvent;
	}

	void AddStart(ExpansionVehicleModule module)
	{
		ExpansionVehicleModuleEvent newEvent = new ExpansionVehicleModuleEvent(module);

		newEvent.m_Next = m_Next;
		m_Next = newEvent;
	}

	void TEMP_DeferredInit()
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.TEMP_DeferredInit();
			evt = evt.m_Next;
		}
	}

	void SettingsChanged()
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.SettingsChanged();
			evt = evt.m_Next;
		}
	}

	void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.Control(pState, pDriver);
			evt = evt.m_Next;
		}
	}

	void PreSimulate(ExpansionPhysicsState pState)
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.PreSimulate(pState);
			evt = evt.m_Next;
		}
	}

	void Simulate(ExpansionPhysicsState pState)
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.Simulate(pState);
			evt = evt.m_Next;
		}
	}

	void PostSimulate(ExpansionPhysicsState pState)
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.PostSimulate(pState);
			evt = evt.m_Next;
		}
	}

	void Animate(ExpansionPhysicsState pState)
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.Animate(pState);
			evt = evt.m_Next;
		}
	}

	void NetworkSend(ParamsWriteContext ctx)
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.NetworkSend(ctx);
			evt = evt.m_Next;
		}
	}

	void NetworkRecieve(ParamsWriteContext ctx)
	{
		ExpansionVehicleModuleEvent evt = this.m_Next;
		while (evt)
		{
			ExpansionVehicleModule module = evt.m_Value;
			module.NetworkRecieve(ctx);
			evt = evt.m_Next;
		}
	}
};
