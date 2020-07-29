class MatMenu extends PopupMenu
{
	EditBoxWidget m_matPath;
	EditBoxWidget m_matName;
	EditBoxWidget m_matValue;

	ButtonWidget m_gameScriptButton;

	void MatMenu()
	{
	}

	void ~MatMenu()
	{
	}

	override void Init()
	{
		m_matPath = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mat_path"));
		m_matName = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mat_name"));
		m_matValue = EditBoxWidget.Cast(layoutRoot.FindAnyWidget("mat_effect"));

		m_gameScriptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("game_script_button"));
	}

	override void OnShow()
	{
	}

	override void OnHide() 
	{	
	}

	void Update() 
	{
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_gameScriptButton ) 
		{
			Material test1 = GetGame().GetWorld().GetMaterial( "Graphics/Materials/postprocess/" + m_matPath.GetText() );
			test1.SetParam( m_matName.GetText(), m_matValue.GetText().ToFloat() );

			Message( GetPlayer(), "Graphics/Materials/postprocess/" + m_matPath.GetText() );
			Message( GetPlayer(), m_matName.GetText() );
			Message( GetPlayer(), "" + m_matValue.GetText().ToFloat() );
		}

		return false;
	}
}