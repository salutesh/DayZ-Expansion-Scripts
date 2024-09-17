/**
 * ExpansionMarketMasks.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMasks: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 38;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MASKS";
		m_FileName = "Masks";

		AddItem("surgicalmask", 	625, 	1045,	1,100);
				
		AddItem("nioshfacemask", 	560, 	935,	1,100);
		
		AddItem("hockeymask", 	205, 	340,	1,100);
		
		#ifndef DAYZ_1_25
		AddItem("shemag_brown", 	70, 	115,	1,100,null,{"shemag_green","shemag_red","shemag_white"});
		#endif

		#ifndef DAYZ_1_25
		AddItem("balaclavamask_beige", 	1245, 	2070,	1,100,null,{"BalaclavaMask_Blue","BalaclavaMask_Pink","BalaclavaMask_White","BalaclavaMask_Blackskull","BalaclavaMask_Black","BalaclavaMask_Green","BalaclavaMask_BDU","BalaclavaMask_Chain","BalaclavaMask_Pig"});
		#else
		AddItem("balaclavamask_beige", 	1245, 	2070,	1,100,null,{"BalaclavaMask_Blue","BalaclavaMask_Pink","BalaclavaMask_White","BalaclavaMask_Blackskull","BalaclavaMask_Black","BalaclavaMask_Green"});
		#endif
		AddItem("balaclavamask_blackskull", 	7105, 	11845,	1,100);
		AddItem("balaclavamask_black", 	2115, 	3525,	1,100);
		AddItem("balaclavamask_green", 	1745, 	2905,	1,100);
		#ifndef DAYZ_1_25
		AddItem("balaclavamask_bdu", 	7045, 	11745,	1,100);
		#endif
		
		#ifndef DAYZ_1_25
		AddItem("balaclava3holes_beige", 	1005, 	1675,	1,100,null,{"Balaclava3Holes_Blue","Balaclava3Holes_Black","Balaclava3Holes_Green","Balaclava3Holes_White"});
		#else
		AddItem("balaclava3holes_beige", 	1005, 	1675,	1,100,null,{"Balaclava3Holes_Blue","Balaclava3Holes_Black","Balaclava3Holes_Green"});
		#endif
		AddItem("balaclava3holes_black", 	1840, 	3065,	1,100);
		AddItem("balaclava3holes_green", 	1445, 	2410,	1,100);
		#ifndef DAYZ_1_25
		AddItem("balaclava3holes_white", 	655, 	1090,	1,100);
		#endif
		
		AddItem("weldingmask", 	500, 	835,	1,100);
		
		AddItem("gasmask", 	3070, 	5120,	1,100);
		AddItem("gp5gasmask", 	7105, 	11845,	1,100,{"gasmask_filter"});
		AddItem("airbornemask", 	460, 	765,	1,100,{"gasmask_filter"});

		AddItem("mimemask_female", 	355, 	590,	1,100,null,{"MimeMask_Male"});
	}
};
