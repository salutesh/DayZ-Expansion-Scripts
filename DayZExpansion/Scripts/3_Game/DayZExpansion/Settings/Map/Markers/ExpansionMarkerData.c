/**
 * ExpansionMarkerData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

const int EXPANSION_MARKER_VIS_NONE = 0;
const int EXPANSION_MARKER_VIS_WORLD = 2;
const int EXPANSION_MARKER_VIS_MAP = 4;

class ExpansionMarkerData
{
	protected string m_UID;

	protected int m_Visibility;

	protected bool m_Is3D;
	protected string m_Text;
	protected string m_IconName;
	protected int m_Color;

	protected vector m_Position;
	protected Object m_Object;

	protected int m_ObjectNetworkLow;
	protected int m_ObjectNetworkHigh;

	protected ScriptedWidgetEventHandler m_Handler;

	void ExpansionMarkerData( string uid = "" )
	{
		m_UID = uid;
		m_Handler = NULL;
		
		SetVisibility( EXPANSION_MARKER_VIS_WORLD | EXPANSION_MARKER_VIS_MAP );
	}

	static ref ExpansionMarkerData Create( ExpansionMapMarkerType type, string uid = "" )
	{
		switch ( type )
		{
		case ExpansionMapMarkerType.PERSONAL:
			return new ExpansionPersonalMarkerData( uid );
		case ExpansionMapMarkerType.PARTY:
			return new ExpansionPartyMarkerData( uid );
		case ExpansionMapMarkerType.SERVER:
			return new ExpansionServerMarkerData( uid );
		case ExpansionMapMarkerType.PLAYER:
			return new ExpansionPlayerMarkerData( uid );
		case ExpansionMapMarkerType.PARTY_QUICK:
			return new ExpansionPartyQuickMarkerData( uid );
		}

		return NULL;
	}

	static ref ExpansionMarkerData Copy( ref ExpansionMarkerData src, ExpansionMapMarkerType type = ExpansionMapMarkerType.UNKNOWN, string uid = "" )
	{
		if ( type == ExpansionMapMarkerType.UNKNOWN )
			type = src.GetType();

		ref ExpansionMarkerData dst = Create( type, uid );
		if ( !dst )
			return NULL;

		if ( src.GetType() == dst.GetType() )
		{
			ScriptReadWriteContext ctx = new ScriptReadWriteContext;
			src.OnStoreSave( ctx.GetWriteContext() );
			dst.OnStoreLoad( ctx.GetReadContext(), int.MAX );
		} else
		{
			dst.m_Is3D = src.m_Is3D;
			dst.m_Text = src.m_Text;
			dst.m_IconName = src.m_IconName;
			dst.m_Color = src.m_Color;
			dst.m_Position = src.m_Position;
			dst.m_Object = src.m_Object;
			dst.m_ObjectNetworkLow = src.m_ObjectNetworkLow;
			dst.m_ObjectNetworkHigh = src.m_ObjectNetworkHigh;
		}

		return dst;
	}

	ExpansionMapMarkerType GetType()
	{
		return ExpansionMapMarkerType.UNKNOWN;
	}

	// Do not call
	void SetUID( string uid )
	{
		m_UID = uid;
	}

	string GetUID()
	{
		return m_UID;
	}

	void SetName( string text )
	{
		m_Text = text;
	}

	string GetName()
	{
		return m_Text;
	}
	
	void SetPosition( vector pos )
	{
		m_Position = pos;
	}

	vector GetPosition()
	{
		if ( m_Object )
			m_Position = m_Object.GetPosition();
		return m_Position;
	}

	void UpdateObject( Object object )
	{
		m_Object = object;

		if ( m_Object )
		{
			m_Position = m_Object.GetPosition();

			m_Object.GetNetworkID( m_ObjectNetworkLow, m_ObjectNetworkHigh );
		}
	}

	Object GetObject()
	{
		if ( IsMissionClient() )
			m_Object = GetGame().GetObjectByNetworkId( m_ObjectNetworkLow, m_ObjectNetworkHigh );
		
		return m_Object;
	}

	vector GetAdjustedPosition()
	{
		return m_Position;
	}

	void SetIconName( string iconName )
	{
		m_IconName = iconName;
	}

	string GetIconName()
	{
		return m_IconName;
	}

	void SetIcon( ExpansionIcon icon )
	{
		if ( icon == NULL )
		{
			m_IconName = "";
		} else
		{
			m_IconName = icon.Name;
		}
	}

	ref ExpansionIcon GetIcon()
	{
		return ExpansionIcons.Get( m_IconName );
	}

	void SetColor( int color )
	{
		m_Color = color;
	}
	
	int GetColor()
	{
		return m_Color;
	}

	void Set3D( bool is )
	{
		m_Is3D = is;
	}

	bool Is3D()
	{
		return m_Is3D;
	}

	int SetVisibility( int vis )
	{
		m_Visibility |= vis;
		return m_Visibility;
	}

	int RemoveVisibility( int vis )
	{
		m_Visibility &= ~vis;
		return m_Visibility;
	}

	int ClearVisibility()
	{
		m_Visibility = 0;
		return m_Visibility;
	}

	int GetVisibility()
	{
		return m_Visibility;
	}

	bool IsVisible()
	{
		return m_Visibility != 0;
	}

	bool IsWorldVisible()
	{
		return (m_Visibility & EXPANSION_MARKER_VIS_WORLD) != 0;
	}

	bool IsMapVisible()
	{
		return (m_Visibility & EXPANSION_MARKER_VIS_MAP) != 0;
	}

	void SetHandler( ref ScriptedWidgetEventHandler handler )
	{
		m_Handler = handler;
	}

	ref ScriptedWidgetEventHandler GetHandler()
	{
		return m_Handler;
	}

	void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write( m_Text );
		ctx.Write( Is3D() );
		ctx.Write( m_IconName );
		ctx.Write( m_Color );
		
		ctx.Write( m_Position );

		if ( m_Object )
		{
			ctx.Write( true );

			m_Object.GetNetworkID( m_ObjectNetworkLow, m_ObjectNetworkHigh );
			ctx.Write( m_ObjectNetworkLow );
			ctx.Write( m_ObjectNetworkHigh );
		} else
		{
			ctx.Write( false );
		}
	}

	bool OnRecieve( ParamsReadContext ctx )
	{
		if ( !ctx.Read( m_Text ) )
			return false;
		if ( !ctx.Read( m_Is3D ) )
			return false;
		if ( !ctx.Read( m_IconName ) )
			return false;
		if ( !ctx.Read( m_Color ) )
			return false;

		if ( !ctx.Read( m_Position ) )
			return false;

		bool hasObject;
		if ( !ctx.Read( hasObject ) )
			return false;

		if ( hasObject )
		{
			if ( !ctx.Read( m_ObjectNetworkLow ) || !ctx.Read( m_ObjectNetworkHigh ) )
				return false;
			
			m_Object = GetGame().GetObjectByNetworkId( m_ObjectNetworkLow, m_ObjectNetworkHigh );
		}

		return true;
	}
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		ctx.Write( Is3D() );
		ctx.Write( m_Text );
		ctx.Write( m_IconName );
		ctx.Write( m_Color );
		ctx.Write( m_Position );
	}
	
	bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( version < 7 )
		{
			bool isParty;
			if ( Expansion_Assert_False( ctx.Read( isParty ), "[" + this + "] Failed reading isParty" ) )
				return false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_Is3D ), "[" + this + "] Failed reading m_Is3D" ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( m_Text ), "[" + this + "] Failed reading m_Text" ) )
			return false;
		
		if ( version < 7 )
		{
			int iconIndex;
			if ( Expansion_Assert_False( ctx.Read( iconIndex ), "[" + this + "] Failed reading iconIndex" ) )
				return false;

			m_IconName = ExpansionIcons.GetNameByIndex_Old( iconIndex );
		} else
		{
			if ( Expansion_Assert_False( ctx.Read( m_IconName ), "[" + this + "] Failed reading m_IconName" ) )
				return false;
		}
		
		if ( Expansion_Assert_False( ctx.Read( m_Color ), "[" + this + "] Failed reading m_Color" ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( m_Position ), "[" + this + "] Failed reading m_Position" ) )
			return false;
		
		return true;
	}
}