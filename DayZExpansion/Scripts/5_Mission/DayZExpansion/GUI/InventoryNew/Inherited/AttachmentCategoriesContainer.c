/**
 * AttachmentCategoriesContainer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class AttachmentCategoriesContainer
{
	override void InitGhostSlots( EntityAI entity )
	{
		string type = entity.GetType();
		string config_path_attachment_categories = "CfgVehicles " + type + " GUIInventoryAttachmentsProps";

		int attachments_categories_count = GetAttachmentCategoriesCount( config_path_attachment_categories );

		if( attachments_categories_count )
		{
			super.InitGhostSlots( entity );
		}
		else if( m_Body.Count() < attachments_categories_count + 3 + attachments_categories_count / ITEMS_IN_ROW )
		{
			super.InitGhostSlots( entity );
		}
		else
		{
			ContainerWithCargo iwc2 = ContainerWithCargo.Cast( m_Body.Get(attachments_categories_count + 2 + attachments_categories_count / ITEMS_IN_ROW ) );
			iwc2.UpdateInterval();
		}
	}
}