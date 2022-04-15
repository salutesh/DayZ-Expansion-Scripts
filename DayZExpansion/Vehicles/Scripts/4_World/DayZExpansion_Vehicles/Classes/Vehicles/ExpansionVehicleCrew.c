/**
 * ExpansionVehicleCrew.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleCrew
{
	private ExpansionVehicleBase m_Vehicle;
	private string m_Name;

	private int m_ComponentIndex;
	private string m_ActionSelection;
	private bool m_IsDriver;

	private vector m_GetInPos;
	private vector m_GetInDir;

	private DayZPlayerImplement m_Player;

	private bool m_Occupied;
	private int m_NetworkIDLow;
	private int m_NetworkIDHigh;

	private vector m_SeatTransform[4];

	void ExpansionVehicleCrew(ExpansionVehicleBase vehicle, string name)
	{
		//Print( "ExpansionVehicleCrew - Start" );

		m_Vehicle = vehicle;
		m_Name = name;

		string getinpos_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " getInPos";
		m_GetInPos = m_Vehicle.GetSelectionPositionMS(GetGame().ConfigGetTextOut(getinpos_path));

		string getindir_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " getInDir";
		m_GetInDir = m_Vehicle.GetSelectionPositionMS(GetGame().ConfigGetTextOut(getindir_path));

		m_GetInDir = vector.Direction(m_GetInPos, m_GetInDir).Normalized();

		string action_selection_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " actionSel";
		m_ActionSelection = GetGame().ConfigGetTextOut(action_selection_path);
		m_ActionSelection.ToLower();
		m_ActionSelection.Trim();

		m_ComponentIndex = -1;

		array<Selection> selections();
		m_Vehicle.GetLODByName("geometryView").GetSelections(selections);

		for (int i = 0; i < 200; ++i)
		{
			if (m_Vehicle.IsActionComponentPartOfSelection(i, m_ActionSelection, "view"))
			{
				m_ComponentIndex = i;
				break;
			}
		}

		string is_driver_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " isDriver";
		m_IsDriver = GetGame().ConfigGetInt(is_driver_path) == 1;

		string proxy_pos_path = "CfgVehicles " + m_Vehicle.GetType() + " Crew " + name + " proxyPos";
		string proxyPos = GetGame().ConfigGetTextOut(proxy_pos_path);

		m_SeatTransform[0] = Vector(1, 0, 0);
		m_SeatTransform[1] = Vector(0, 1, 0);
		m_SeatTransform[2] = Vector(0, 0, 1);
		m_SeatTransform[3] = Vector(0, 0, 0);

		LOD lod = m_Vehicle.GetLODByName("geometryView");
		Selection selection = lod.GetSelectionByName(proxyPos);
		if (selection)
		{
			m_SeatTransform[3] = selection.GetVertexPosition(lod, 0);
			m_SeatTransform[1] = vector.Direction(m_SeatTransform[3], selection.GetVertexPosition(lod, 1)).Normalized();
			m_SeatTransform[2] = -vector.Direction(m_SeatTransform[3], selection.GetVertexPosition(lod, 2)).Normalized();
			m_SeatTransform[0] = m_SeatTransform[1] * m_SeatTransform[2];
		}
	}

	string GetName()
	{
		return m_Name;
	}

	ExpansionVehicleBase GetVehicle()
	{
		return m_Vehicle;
	}

	int GetComponentIndex()
	{
		return m_ComponentIndex;
	}

	string GetComponentSelection()
	{
		return m_ActionSelection;
	}

	bool IsDriver()
	{
		return m_IsDriver;
	}

	void GetIn(out vector pos, out vector dir)
	{
		pos = m_GetInPos;
		dir = m_GetInDir;
	}

	void GetTransform(out vector trans[4])
	{
		trans[0] = m_SeatTransform[0];
		trans[1] = m_SeatTransform[1];
		trans[2] = m_SeatTransform[2];
		trans[3] = m_SeatTransform[3];
	}

	void SetPlayer(DayZPlayerImplement player)
	{
		m_Player = player;

		m_Occupied = m_Player != NULL;

		if (m_Occupied)
			m_Player.GetNetworkID(m_NetworkIDLow, m_NetworkIDHigh);
	}

	DayZPlayerImplement GetPlayer()
	{
		return m_Player;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		NetworkBubbleFix();

		ctx.Write(m_Occupied);
		if (m_Occupied)
		{
			ctx.Write(m_NetworkIDLow);
			ctx.Write(m_NetworkIDHigh);
		}
	}

	void OnRead(ParamsReadContext ctx)
	{
		ctx.Read(m_Occupied);
		if (m_Occupied)
		{
			ctx.Read(m_NetworkIDLow);
			ctx.Read(m_NetworkIDHigh);
		}

		if (m_Occupied)
		{
			PlayerBase player = PlayerBase.Cast(m_Player);
			m_Player = DayZPlayerImplement.Cast(GetGame().GetObjectByNetworkId(m_NetworkIDLow, m_NetworkIDHigh));

			if (player != m_Player && m_Player != GetGame().GetPlayer())
			{
				vector tmPlayer[4];
				vector tmTarget[4];
				vector tmLocal[4];

				m_Player.GetTransformWS(tmPlayer);
				m_Vehicle.GetTransform(tmTarget);
				Math3D.MatrixInvMultiply4(tmTarget, tmPlayer, tmLocal);

				m_Player.LinkToLocalSpaceOf(m_Vehicle, tmLocal);
			}
		}
		else
		{
			if (m_Player && m_Player != GetGame().GetPlayer())
			{
				m_Player.UnlinkFromLocalSpace();
			}

			m_Player = NULL;
		}
	}

	/**
	 * Due to Enfusion not properly updating the network bubble we
	 * unfortunately have to do this. This can be performance intensive.
	 * 
	 * TODO: Ask Mirek or Adam for an export into Enforce Script
	 * 
	 * Issues: constant teleportation, can look bad, affects server performance
	 */
	void NetworkBubbleFix()
	{
		if (m_Player)
		{
			vector ori = m_Player.GetOrientation();
			m_Player.SetPosition(m_Player.GetPosition());
			m_Player.SetOrientation(ori);

			if (GetExpansionSettings().GetDebug().DebugVehiclePlayerNetworkBubbleMode > 0)
			{
				if (GetExpansionSettings().GetDebug().DebugVehiclePlayerNetworkBubbleMode > 1)
				{
					GetGame().RemoteObjectTreeDelete(m_Player);
				}

				GetGame().RemoteObjectTreeCreate(m_Player);
			}
		}
	}
};
