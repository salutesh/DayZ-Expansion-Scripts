/**
 * ExpansionMarketNetworkItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketNetworkBaseItem
{
	int ItemID;

	int Stock;

	void ExpansionMarketNetworkBaseItem(int itemID = -1, int stock = 0)
	{
		ItemID = itemID;
		Stock = stock;
	}

	void WriteTo(ExpansionScriptRPC rpc)
	{
		rpc.WriteUInt(ItemID, 16);

		rpc.WriteUInt(Stock, 16);
	}

	bool ReadFrom(ExpansionBitStreamReader reader)
	{
		if (!reader.ReadUInt(ItemID, 16))
			return false;

		if (!reader.ReadUInt(Stock, 16))
			return false;

		return true;
	}
};

class ExpansionMarketNetworkItem: ExpansionMarketNetworkBaseItem
{
	int CategoryID;

	string ClassName;

	int MaxPriceThreshold;
	int MinPriceThreshold;

	int MaxStockThreshold;
	int MinStockThreshold;

	ref TIntArray AttachmentIDs;
	ref TStringArray Variants;

	int m_BuySell;

#ifdef EXPANSIONMODHARDLINE
	int m_Rarity;
#endif

	int QuantityPercent;
	int m_SellPricePercent;  //! @note integer representation of bfloat16 (server)
	float SellPricePercent;  //! client

	ref ExpansionMarketItem m_MarketItem;

	[NonSerialized()]
	bool m_StockOnly;

#ifndef SERVER
	void ExpansionMarketNetworkItem(int itemID = -1, int stock = 0)
	{
		AttachmentIDs = {};
		Variants = {};
	}
#endif

	override void WriteTo(ExpansionScriptRPC rpc)
	{
		super.WriteTo(rpc);

		rpc.Write(MaxPriceThreshold);
		rpc.Write(MinPriceThreshold);

		rpc.WriteUInt(MaxStockThreshold, 16);
		rpc.WriteUInt(MinStockThreshold, 16);

		//! Write static network representation of market item (guaranteed to be aligned on write but not necessarily on read)

		if (!m_MarketItem.m_StaticNetworkRepresentation)
		{
			EXError.Warn(this, string.Format("Creating missing static network representation for market item '%1'", m_MarketItem.ClassName));
			m_MarketItem.CreateStaticNetworkRepresentation();
		}

		foreach (int packed: m_MarketItem.m_StaticNetworkRepresentation)
			rpc.Write(packed);

		//! Write final bits

		rpc.WriteUInt(m_BuySell, 4);

#ifdef EXPANSIONMODHARDLINE
		rpc.WriteUInt(m_Rarity, 4);
#endif

		rpc.Write(QuantityPercent, 8);

		rpc.WriteUInt(m_SellPricePercent, 16);

		rpc.Flush();
	}

	override bool ReadFrom(ExpansionBitStreamReader reader)
	{
		if (!super.ReadFrom(reader))
			return false;

		if (!reader.Read(MaxPriceThreshold))
			return false;

		if (!reader.Read(MinPriceThreshold))
			return false;

		if (!reader.ReadUInt(MaxStockThreshold, 16))
			return false;

		if (!reader.ReadUInt(MinStockThreshold, 16))
			return false;

		//! Read static network representation of market item

		if (!reader.ReadUInt(CategoryID, 16))
			return false;

		bool isHashed;
		int hashA;
		int hashB;

		if (!reader.ReadBool(isHashed))
			return false;

		if (isHashed)
		{
			if (!ExpansionItemNameTable.ReadHash(reader, hashA, hashB))
				return false;

			ClassName = ExpansionItemNameTable.GetTypeLowerByHash(hashA, hashB);
		}
		else
		{
			if (!reader.ReadClassNameLower(ClassName))
				return false;
		}

		int attCount;
		if (!reader.ReadUInt(attCount, 8))
			return false;

		if (attCount < 0 || attCount > 255)
			return false;

		while (attCount--)
		{
			int attID;
			if (!reader.ReadUInt(attID, 16))
				return false;
			AttachmentIDs.Insert(attID);
		}

		int variantsCount;
		if (!reader.ReadUInt(variantsCount, 8))
			return false;

		if (variantsCount)
		{
			if (variantsCount < 0 || variantsCount > 255)
				return false;

			bool isSubstring;
			if (!reader.ReadBool(isSubstring))
				return false;

			string className;

			if (isSubstring)
			{
				int substringLength;
				if (!reader.ReadUInt(substringLength, 7))
					return false;

				className = ClassName.Substring(0, substringLength);
			}
			else
			{
				className = ClassName;
			}

			while (variantsCount--)
			{
				if (!reader.ReadBool(isSubstring))
					return false;

				string variant;

				if (isSubstring)
				{
					if (!reader.ReadClassNameLower(variant))
						return false;

				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.MARKET, this, className + "*" + variant);
				#endif

					variant = className + variant;
				}
				else
				{
					if (!reader.ReadBool(isHashed))
						return false;

					if (isHashed)
					{
						if (!ExpansionItemNameTable.ReadHash(reader, hashA, hashB))
							return false;

						variant = ExpansionItemNameTable.GetTypeLowerByHash(hashA, hashB);
					}
					else
					{
						if (!reader.ReadClassNameLower(variant))
							return false;
					}

				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.MARKET, this, variant);
				#endif
				}

				Variants.Insert(variant);
			}
		}

		//! Need to reset after reading static network representation so we are aligned for next reads
		reader.Reset();

		//! Read final bits

		if (!reader.ReadUInt(m_BuySell, 4))
			return false;

#ifdef EXPANSIONMODHARDLINE
		if (!reader.ReadUInt(m_Rarity, 4))
			return false;
#endif

		if (!reader.Read(QuantityPercent, 8))
			return false;

		if (!reader.Read(SellPricePercent, 16))
			return false;

		reader.Reset();

		return true;
	}
};