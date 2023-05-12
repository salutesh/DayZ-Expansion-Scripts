/**
 * ExpansionAirdropContainerManagers.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionAirdropContainerManagers
 * @brief		Keeps track of ExpansionAirdropContainerManager instances
 **/
class ExpansionAirdropContainerManagers
{
	private static ref array< ref ExpansionAirdropContainerManager > m_ContainerManagers = new array< ref ExpansionAirdropContainerManager >;

	static void Add( ExpansionAirdropContainerBase container, TStringArray infected, int infectedCount )
	{
		m_ContainerManagers.Insert( new ExpansionAirdropContainerManager( container, infected, infectedCount ) );
	}

	static ExpansionAirdropContainerManager Find( ExpansionAirdropContainerBase container )
	{
		for ( int i = 0; i < m_ContainerManagers.Count(); i++ )
		{
			if ( m_ContainerManagers[i].m_Container == container )
				return m_ContainerManagers[i];
		}

		return NULL;
	}

	static void Remove( ExpansionAirdropContainerManager manager )
	{
		m_ContainerManagers.RemoveItem( manager );
	}

	static void DeferredCleanup()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( Cleanup, 1000, false );
	}

	protected static void Cleanup()
	{
		for ( int i = 0; i < m_ContainerManagers.Count(); i++ )
		{
			if ( !m_ContainerManagers[i].m_Container )
				m_ContainerManagers[i].Cleanup();
		}
	}
};
