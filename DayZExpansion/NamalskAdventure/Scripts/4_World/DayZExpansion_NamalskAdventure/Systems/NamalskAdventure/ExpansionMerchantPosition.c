/**
 * ExpansionMerchantPosition.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_NAMALSK_ADVENTURE
#ifdef NAMALSK_TERRAIN
#ifdef EXPANSIONMODMARKET
class ExpansionMerchantPosition
{
	protected int m_ID = -1;
	protected vector m_Position;
	protected vector m_Orientation;

	int GetID()
	{
		return m_ID;
	}

	vector GetPosition()
	{
		return m_Position;
	}

	vector GetOrientation()
	{
		return m_Orientation;
	}
};

class ExpansionMerchantPosition_1: ExpansionMerchantPosition
{
	//! Vorkuta
	void ExpansionMerchantPosition_1()
	{
		m_ID = 1;
		m_Position = Vector(6831.73, 16.4077, 11346.5);
		m_Orientation = Vector(310.0, 0, 0);
	}
};
class ExpansionMerchantPosition_2: ExpansionMerchantPosition
{
	//! Jalovisco
	void ExpansionMerchantPosition_2()
	{
		m_ID = 2;
		m_Position = Vector(8355.28, 15.5027, 10777.1);
		m_Orientation = Vector(230.0, 0, 0);
	}
};
class ExpansionMerchantPosition_3: ExpansionMerchantPosition
{
	//! Lubjansk
	void ExpansionMerchantPosition_3()
	{
		m_ID = 3;
		m_Position = Vector(4425.47, 3.26632, 11278.7);
		m_Orientation = Vector(300.0, 0, 0);
	}
};
class ExpansionMerchantPosition_4: ExpansionMerchantPosition
{
	//! Tara harbor
	void ExpansionMerchantPosition_4()
	{
		m_ID = 4;
		m_Position = Vector(7835.72, 10.0497, 7667.68);
		m_Orientation = Vector(215.0, 0, 0);
	}
};
class ExpansionMerchantPosition_5: ExpansionMerchantPosition
{
	//! Norinsk
	void ExpansionMerchantPosition_5()
	{
		m_ID = 5;
		m_Position = Vector(4010.66, 53.0584, 7587.21);
		m_Orientation = Vector(320.0, 0, 0);
	}
};
#endif
#endif
#endif