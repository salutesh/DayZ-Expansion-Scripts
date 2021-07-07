/**
 * ExpansionScriptView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDialogBookBase: ExpansionDialogBase
{
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_base_book.layout";
	}
	
	override void SetBaseDialogView()
	{
		super.SetBaseDialogView();
		
		/*float main_height, main_width;
		float height, width;
		float max;
		
		dialog_base_title.GetScreenSize(width, height);
		max += width; 
		
		dialog_base_footer.GetScreenSize(width, height);
		max += width;
		
		dialog_body_content.GetScreenSize(width, height);
		max += width;
		
		GetLayoutRoot().SetSize(300, 300);
		dialog_base_backround.SetSize(300, 300);*/
		SetContentSize();
	}
	
	void SetContentSize()
	{
		for ( int i = 0; i < m_DialogBaseController.DialogContents.Count(); i++ )
		{
			float s_x, s_y, s_1, s_h, p_x, p_y, p_h, p_1;
			GetLayoutRoot().GetScreenPos(s_x, s_y);
			dialog_body_content.GetScreenPos(p_x, p_y);
			dialog_body_content.GetScreenSize(p_1, p_h);
			
			m_DialogBaseController.DialogContents[i].GetLayoutRoot().GetScreenSize(s_1, s_h);
			m_DialogBaseController.DialogContents[i].GetLayoutRoot().SetPos(p_x, p_y + s_h + (s_h * i));
		}
		
		float r_1, r_h, r_x, r_y, g_1, g_h, b_1, b_h;
		GetLayoutRoot().GetScreenSize(r_1, r_h);
		GetLayoutRoot().GetScreenPos(r_x, r_y);
		
		dialog_body_content.GetScreenSize(g_1, g_h);
		//dialog_buttons_grid.GetScreenSize(b_1, b_h);
		
		dialog_base_backround.SetSize(r_1, r_h + g_h + b_h);
		dialog_base_backround.SetPos(r_x, r_y);
	}
}