modded class ExpansionVehicle
{
	ref map<int, Human> m_ReservedSeats = new map<int, Human>;

	bool IsSeatReservedByOther(int posIdx, Human crewMember)
	{
		Human reserved = m_ReservedSeats[posIdx];

		if (!reserved || (!reserved.IsAlive() && !CrewMember(posIdx)))
			return false;

		return reserved != crewMember;
	}

	void ReserveSeat(int posIdx, Human crewMember)
	{
		m_ReservedSeats[posIdx] = crewMember;
	}
}
