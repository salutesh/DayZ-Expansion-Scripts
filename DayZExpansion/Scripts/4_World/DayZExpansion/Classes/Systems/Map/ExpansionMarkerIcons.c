

class ExpansionMarkerIcons
{
	private static autoptr array< ref ExpansionMarkerIcon > m_MarkersIconList;

	static int Count()
	{
		if ( m_MarkersIconList == NULL )
		{
			JsonFileLoader< ref array< ref ExpansionMarkerIcon > >.JsonLoadFile( "DayZExpansion/Scripts/Data/Markers.json", m_MarkersIconList );
		}

		return m_MarkersIconList.Count();
	}

	static string GetMarkerPath( int index )
	{
		if ( m_MarkersIconList == NULL )
		{
			JsonFileLoader< ref array< ref ExpansionMarkerIcon > >.JsonLoadFile( "DayZExpansion/Scripts/Data/Markers.json", m_MarkersIconList );
		}

		if ( index < 0 )
		{
			return "";
		}

		if ( index >= m_MarkersIconList.Count() )
		{
			return "";
		}

		return m_MarkersIconList.Get( index ).Path;
	}

	static int GetMarkerIndex(string findPath)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarkerIcons:: GetMarkerPath - Start");
		#endif

		if ( m_MarkersIconList == NULL )
		{
			JsonFileLoader< ref array< ref ExpansionMarkerIcon > >.JsonLoadFile( "DayZExpansion/Scripts/Data/Markers.json", m_MarkersIconList );
		}

		if (findPath != "")
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("ExpansionMarkerIcons:: GetMarkerPath - Try to find index for icon path: " + findPath);
			#endif
			
			for ( int i = 0; i < m_MarkersIconList.Count(); ++i )
			{
				ExpansionMarkerIcon icon = m_MarkersIconList.Get(i);
				int index;
				string iconPath = icon.Path;

				if (findPath == iconPath)
				{
					index = i;

					if ( index < 0 )
					{
						return -1;
					}

					if ( index >= m_MarkersIconList.Count() )
					{
						return -1;
					}
				}				
			}
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionMarkerIcons:: GetMarkerPath - Index for path: " + findPath + " is " + index.ToString());
		#endif

		return index;
	}
	
	static string GetMarkerName( int index )
	{
		if ( m_MarkersIconList == NULL )
		{
			JsonFileLoader< ref array< ref ExpansionMarkerIcon > >.JsonLoadFile( "DayZExpansion/Scripts/Data/Markers.json", m_MarkersIconList );
		}

		if ( index < 0 )
		{
			return "";
		}

		if ( index >= m_MarkersIconList.Count() )
		{
			return "";
		}

		return m_MarkersIconList.Get( index ).Name;
	}
}