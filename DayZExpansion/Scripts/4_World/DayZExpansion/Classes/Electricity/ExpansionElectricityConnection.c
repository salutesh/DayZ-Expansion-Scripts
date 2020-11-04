/**
 * ExpansionElectricityConnection.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionElectricityConnection: ExpansionElectricityBase
{
	private bool m_IsPaired;
	private bool m_IsPairedSynch;

	private int m_SourceA = 0;
	private int m_SourceB = 0;
	private int m_SourceD = 0;
	private int m_SourceC = 0;

	private int m_SourceNetLow;
	private int m_SourceNetHigh;

	private ItemBase m_Source;
	private ItemBase m_Item;

	void ExpansionElectricityConnection( ItemBase item )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::ExpansionElectricityConnection - Start");
		#endif
		
		m_Item = item;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::ExpansionElectricityConnection - End");
		#endif
	}

	override void OnStoreSave_OLD( ParamsWriteContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreSave - Start");
		#endif
		
		ctx.Write( m_IsPaired );

		if ( m_IsPaired )
		{
			ctx.Write( m_SourceA );
			ctx.Write( m_SourceB );
			ctx.Write( m_SourceD );
			ctx.Write( m_SourceC );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreSave - End");
		#endif
	}

	override bool OnStoreLoad_OLD( ParamsReadContext ctx, int vanillaVersion, int expansionVersion )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreLoad - Start");
		#endif
		
		if ( Expansion_Assert_False( ctx.Read( m_IsPaired ), "[" + this + "] Failed reading m_IsPaired" ) )
			return false;

		if ( m_IsPaired )
		{
			if ( Expansion_Assert_False( ctx.Read( m_SourceA ), "[" + this + "] Failed reading m_SourceA" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_SourceB ), "[" + this + "] Failed reading m_SourceB" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_SourceD ), "[" + this + "] Failed reading m_SourceD" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_SourceC ), "[" + this + "] Failed reading m_SourceC" ) )
				return false;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreLoad - End");
		#endif
		
		return true;
	}

	#ifdef CF_MOD_STORAGE
	override void OnStoreSave( ModStorage storage )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreSave - Start");
		#endif
		
		storage.WriteBool( m_IsPaired );

		if ( m_IsPaired )
		{
			storage.WriteInt( m_SourceA );
			storage.WriteInt( m_SourceB );
			storage.WriteInt( m_SourceD );
			storage.WriteInt( m_SourceC );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreSave - End");
		#endif
	}

	override bool OnStoreLoad( ModStorage storage )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreLoad - Start");
		#endif
		
		if ( Expansion_Assert_False( storage.ReadBool( m_IsPaired ), "[" + this + "] Failed reading m_IsPaired" ) )
			return false;

		if ( m_IsPaired )
		{
			if ( Expansion_Assert_False( storage.ReadInt( m_SourceA ), "[" + this + "] Failed reading m_SourceA" ) )
				return false;
			if ( Expansion_Assert_False( storage.ReadInt( m_SourceB ), "[" + this + "] Failed reading m_SourceB" ) )
				return false;
			if ( Expansion_Assert_False( storage.ReadInt( m_SourceD ), "[" + this + "] Failed reading m_SourceD" ) )
				return false;
			if ( Expansion_Assert_False( storage.ReadInt( m_SourceC ), "[" + this + "] Failed reading m_SourceC" ) )
				return false;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnStoreLoad - End");
		#endif
		
		return true;
	}
	#endif

	void OnAfterLoad()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnAfterLoad - Start");
		#endif
		
		if ( m_IsPaired )
		{
			m_Source = ItemBase.Cast( GetGame().GetEntityByPersitentID( m_SourceA, m_SourceB, m_SourceC, m_SourceD ) );

			if ( m_Source )
			{
				m_Source._ExpansionAddConnection( m_Item );
			} else
			{
				m_IsPaired = false;
			}
		} else
		{
			m_Source = NULL;
		}

		m_IsPairedSynch = m_IsPaired;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnAfterLoad - End");
		#endif
	}

	void Setup()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::Setup - Start");
		#endif
		
		m_IsPaired = false;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::Setup - Start");
		#endif
	}

	void OnVariablesSynchronized()
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnVariablesSynchronized - Start");
		#endif
		if ( m_IsPairedSynch != m_IsPaired )
		{
			m_IsPaired = m_IsPairedSynch;
		}

		ItemBase prev_source = m_Source;

		if ( m_IsPaired )
		{

			m_Source = ItemBase.Cast( GetGame().GetObjectByNetworkId( m_SourceNetLow, m_SourceNetHigh ) );

			if ( prev_source != m_Source )
			{
				if ( prev_source )
					prev_source._ExpansionRemoveConnection( m_Item );
				
				m_Source._ExpansionAddConnection( m_Item );
			}
		} else
		{
			if ( prev_source )
				prev_source._ExpansionRemoveConnection( m_Item );

			m_Source = NULL;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionElectricityConnection::OnVariablesSynchronized - End");
		#endif
	}

	bool CanPairTo( ItemBase source )
	{
		//if ( m_Source )
		//	return false;

		return source.m_EM != NULL;
	}

	void Disconnect()
	{
		Print( "[" + m_Item + "] Disconnect m_Source=" + m_Source );

		if ( m_Source )
			m_Source._ExpansionRemoveConnection( m_Item );

		m_Source = NULL;
		m_IsPaired = false;

		m_IsPairedSynch = m_IsPaired;
		
		Print( "m_IsPairedSynch=" + m_IsPairedSynch );
	}

	void Pair( ItemBase source )
	{
		Print( "[" + m_Item + "] Pair source=" + source + " m_Source=" + m_Source );

		ItemBase old_source = m_Source;
		if ( source == old_source )
			return;

		m_Source = source;
		if ( m_Source && !CanPairTo( m_Source ) )
			m_Source = NULL;

		Print( "m_Source=" + m_Source );

		Print( "m_IsPaired=" + m_IsPaired );
		m_IsPaired = m_Source != NULL;
		Print( "m_IsPaired=" + m_IsPaired );

		if ( old_source != m_Source )
		{
			old_source._ExpansionRemoveConnection( m_Item );
		}

		if ( m_IsPaired )
		{
			m_Source._ExpansionAddConnection( m_Item );
			m_Source.GetPersistentID( m_SourceA, m_SourceB, m_SourceC, m_SourceD );
			m_Source.GetNetworkID( m_SourceNetLow, m_SourceNetHigh );
		}
		Print( "m_SourceA=" + m_SourceA + " m_SourceB=" + m_SourceB + " m_SourceC=" + m_SourceC + " m_SourceD=" + m_SourceD );
		Print( "m_SourceNetLow=" + m_SourceNetLow + " m_SourceNetHigh=" + m_SourceNetHigh );

		m_IsPairedSynch = m_IsPaired;
		Print( "m_IsPairedSynch=" + m_IsPairedSynch );
	}

	bool IsConnected( ItemBase item )
	{
		if ( m_Source == NULL )
			return false;

		if ( item == NULL )
			return m_IsPaired;
			
		if ( m_Source != item )
			return false;

		return true;
	}
};