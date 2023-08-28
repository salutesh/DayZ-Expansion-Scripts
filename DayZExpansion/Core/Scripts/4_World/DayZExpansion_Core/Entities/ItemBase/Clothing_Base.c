modded class Clothing
{
	//! This controls arms/legs texture for ExpansionNotPlayerBase and inheriting classes (e.g. static Expansion NPCs)
	override void Expansion_SwitchItemSelectionTextureEx(EItemManipulationContext context, Param par = null)
	{
		super.Expansion_SwitchItemSelectionTextureEx(context, par);

		Param1<ExpansionNotPlayerBase> data = Param1<ExpansionNotPlayerBase>.Cast(par);
		if (!data)
		{
			return;
		}
		
		ExpansionNotPlayerBase player = data.param1;
		
		int personality = GetHiddenSelectionIndex("personality");
		if (personality >= 0)
		{
			string tone_mat = player.m_EmptyGloves.GetHiddenSelectionsMaterials().Get(0);
			string tone_texture;
			
			if (player.m_CorpseState > PlayerConstants.CORPSE_STATE_FRESH)
			{
				tone_texture = player.m_DecayedTexture;
			}
			else
			{
				tone_texture = player.m_EmptyGloves.GetHiddenSelectionsTextures().Get(0);
			}
			
			SetObjectMaterial( personality, tone_mat );
			SetObjectTexture( personality, tone_texture );
		}
	}
}
