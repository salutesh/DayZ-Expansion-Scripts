modded class AttachmentsOutOfReach
{
	override static bool IsAttachmentReachable(EntityAI e, string att_slot_name = "", int slot_id = -1, float range = 1.5)
	{
		//! Default range extended for helicopters
		if (range < 4.5 && e.IsInherited(ExpansionHelicopterScript))
			range = 4.5;

		//! Verbatim copy of IsAttachmentReachable from scripts\4_world\systems\inventory\attachmentsoutofreach.c
		if( !e.IgnoreOutOfReachCondition() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if( player.IsInVehicle() )
			{
				return false;
			}
			else
			{
				
				vector pos_att;

				if( att_slot_name != "" )
				{
					if( e.MemoryPointExists(att_slot_name) )
					{
						vector mem_point = e.GetMemoryPointPos(att_slot_name);
						pos_att = e.ModelToWorld(mem_point);
					}
					else
					{
						pos_att = e.ModelToWorld(GetAttachmentPosition(e, InventorySlots.GetSlotIdFromString( att_slot_name ) ));
					}
					
				}
				else if( slot_id != -1 )
					pos_att = e.GetPosition() + GetAttachmentPosition(e, slot_id);
				
				vector pos_player = player.GetPosition();
				
				float height_diff = Math.AbsFloat( pos_player[1] - pos_att[1] );
				if( height_diff < range )
				{
					pos_player[1] = 0;
					pos_att[1] = 0;
					if ( vector.Distance(pos_player, pos_att) <= range )
					{
						return true;
					}
					return false;
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return true;
		}
	}
}
