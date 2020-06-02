/**
 * CreditsDepartmentElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CreditsDepartmentElement 
{
	protected bool 				m_Inited = false;
	// ------------------------------------------------------------
	// CreditsDepartmentElement Constructor
	// ------------------------------------------------------------
	void CreditsDepartmentElement( int index, Widget parent, JsonDataCreditsDepartment department_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::Constructor - Start");
		#endif
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.DeleteBaseWidget, 250, false, index, parent, department_data );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::Constructor - End");
		#endif
	}
	
	void Init( int index, Widget parent, JsonDataCreditsDepartment department_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::Init - Start");
		#endif
		m_Root					= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/credits/expansion_department_element.layout", parent );
		m_DepartmentTitle		= TextWidget.Cast( m_Root.FindAnyWidget( "department_title" ) );
		m_DepartmentSections	= GridSpacerWidget.Cast( m_Root.FindAnyWidget( "department_sections" ) );
		
		if( department_data.DepartmentName != "" )
		{
			m_DepartmentTitle.SetText( department_data.DepartmentName );
		}
		else
		{
			m_DepartmentTitle.Show( false );
			m_Root.FindAnyWidget( "SeparatorPanel" ).Show( false );
		}
		
		m_Root.SetSort( index );
		m_Inited = true;
		LoadDataAsync( department_data.Sections );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::Init - End");
		#endif
	}

	override void LoadDataAsync( array<ref JsonDataCreditsSection> department_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::LoadDataAsync - Start");
		#endif
		if (m_Inited)
		{
			super.LoadDataAsync(department_data);
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::LoadDataAsync - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteBaseWidget
	// ------------------------------------------------------------
	void DeleteBaseWidget( int index, Widget parent, JsonDataCreditsDepartment department_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::DeleteBaseWidget - Start");
		#endif
		if (m_Root)
		{
			m_Root.Unlink();
		}
		Init( index, parent, department_data );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsDepartmentElement::DeleteBaseWidget - End");
		#endif
	}
}

modded class CreditsDepartmentSection
{
	protected bool 				m_Inited = false;
	// ------------------------------------------------------------
	// CreditsDepartmentSection Constructor
	// ------------------------------------------------------------
	void CreditsDepartmentSection( int index, Widget parent, JsonDataCreditsSection section_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::Constructor - Start");
		#endif
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.DeleteBaseWidget, 250, false, index, parent, section_data);
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::Constructor - End");
		#endif
	}
	
	void Init( int index, Widget parent, JsonDataCreditsSection section_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::Init - Start");
		#endif
		m_Root				= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/credits/expansion_department_section.layout", parent );
		m_SectionTitle		= TextWidget.Cast( m_Root.FindAnyWidget( "section_title" ) );
		m_SectionSections	= WrapSpacerWidget.Cast( m_Root.FindAnyWidget( "section_elements" ) );
		
		if( section_data.SectionName != "" )
		{
			m_SectionTitle.SetText( section_data.SectionName );
		}
		else
		{
			m_SectionTitle.Show( false );
			m_Root.FindAnyWidget( "SeparatorPanel" ).Show( false );
		}	
		
		m_Inited = true;
		LoadDataAsync( section_data.SectionLines );
		m_Root.SetSort( index );
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::Init - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override LoadDataAsync
	// ------------------------------------------------------------
	override void LoadDataAsync( array<string> section_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::LoadDataAsync - Start");
		#endif
		if (m_Inited)
		{
			for( int i = 1; i <= section_data.Count(); i++ )
			{
				TextWidget w = TextWidget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/credits/expansion_department_section_element.layout", m_SectionSections ) );
				w.SetText( section_data.Get( i - 1 ) + " \n" );
				w.Update();
				m_Root.SetSort( i );
				m_SectionEntries.Insert( w );
			}
			
			m_SectionSections.Update();
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::LoadDataAsync - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteBaseWidget
	// ------------------------------------------------------------
	void DeleteBaseWidget( int index, Widget parent, JsonDataCreditsSection section_data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::DeleteBaseWidget - Start");
		#endif
		if (m_Root)
		{
			m_Root.Unlink();
		}
		Init( index, parent, section_data );
		#ifdef EXPANSIONEXPRINT
		EXPrint("CreditsDepartmentSection::DeleteBaseWidget - End");
		#endif
	}
}
