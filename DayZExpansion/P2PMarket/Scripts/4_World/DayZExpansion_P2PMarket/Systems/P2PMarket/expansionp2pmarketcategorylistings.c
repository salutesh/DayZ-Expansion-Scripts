/**
 * ExpansionP2PMarketCategoryListings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketCategoryListings
{
	int m_CategoryIndex = -1;
	int m_SubCategoryIndex = -1;
	int m_Count;
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(m_CategoryIndex);
		ctx.Write(m_SubCategoryIndex);
		ctx.Write(m_Count);
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_CategoryIndex))
		{
			Error(ToString() + "::OnRecieve - m_CategoryIndex");
			return false;
		}
		
		if (!ctx.Read(m_SubCategoryIndex))
		{
			Error(ToString() + "::OnRecieve - m_SubCategoryIndex");
			return false;
		}
		
		if (!ctx.Read(m_Count))
		{
			Error(ToString() + "::OnRecieve - m_Count");
			return false;
		}
		
		return true;
	}
	
	void Debug()
	{
		if (!EXTrace.P2PMARKET)
			return;

		ErrorEx("m_CategoryIndex: " + m_CategoryIndex, ErrorExSeverity.INFO);
		ErrorEx("m_SubCategoryIndex: " + m_SubCategoryIndex, ErrorExSeverity.INFO);
		ErrorEx("m_Count: " + m_Count, ErrorExSeverity.INFO);
	}
};