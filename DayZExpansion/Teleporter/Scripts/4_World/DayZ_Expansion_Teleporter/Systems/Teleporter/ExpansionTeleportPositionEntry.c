/**
 * ExpansionTeleportPositionEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTeleportPositionEntry
{
	protected vector m_Position;
	protected vector m_Orientation;

	void SetPosition(vector pos, vector ori)
	{
		m_Position = pos;
		m_Orientation = ori;
	}
	
	vector GetPosition()
	{
		return m_Position;
	}
	
	vector GetOrientation()
	{
		return m_Orientation;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		ctx.Write(m_Position);
		ctx.Write(m_Orientation);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_Position))
			return false;

		if (!ctx.Read(m_Orientation))
			return false;

		return true;
	}
};