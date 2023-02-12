#ifndef EXPANSIONMODVEHICLE
CarScript g_Expansion_Car;
#endif

modded class CarScript
{
	ref map<int, Human> m_Expansion_ReservedSeats = new map<int, Human>;

	bool Expansion_IsSeatReservedByOther(int posIdx, Human crewMember)
	{
		Human reserved = m_Expansion_ReservedSeats[posIdx];

		if (!reserved)
			return false;

		return reserved != crewMember;
	}

	void Expansion_ReserveSeat(int posIdx, Human crewMember)
	{
		m_Expansion_ReservedSeats[posIdx] = crewMember;
	}
}
