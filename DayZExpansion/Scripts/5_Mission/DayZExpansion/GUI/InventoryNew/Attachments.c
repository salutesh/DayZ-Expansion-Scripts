/**
 * Attachments.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Attachments
{
	override void InitAttachmentGrid( int att_row_index )
	{
		super.InitAttachmentGrid( att_row_index );

		for ( int i = 0; i < m_AttachmentSlotNames.Count(); ++i )
		{
			SlotsIcon icon2 = SlotsContainer.Cast( m_AttachmentsContainer.Get( ( i / ITEMS_IN_ROW ) ) ).GetSlotIcon( i % ITEMS_IN_ROW );

			string path = "CfgSlots" + " Slot_" + m_AttachmentSlotNames[i];

			string icon_name = "";
			if ( GetGame().ConfigGetText( path + " expansionGhostIcon", icon_name ) && icon_name != "" )
				icon2.GetGhostSlot().LoadImageFile( 0, icon_name );
		}
	}
};