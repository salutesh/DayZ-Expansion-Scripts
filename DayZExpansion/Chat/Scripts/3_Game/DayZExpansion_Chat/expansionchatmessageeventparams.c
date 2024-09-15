//! @note we extend vanilla ChatMessageEventParams: <int> channel, <string> from, <string> message, <string> color
//! and add <string> UID, <string> groupTag (latter only if groups mod loaded)

class ExpansionChatMessageEventParams: ChatMessageEventParams
{
	string param5;  //! UID

#ifdef EXPANSIONMODGROUPS
	string param6;  //! Group tag

	void ExpansionChatMessageEventParams(int p1, string p2, string p3, string p4, string p5 = "", string p6 = "")
	{
		param5 = p5;
		param6 = p6;
	}

	override bool Serialize(Serializer ctx)
	{
		if (!super.Serialize(ctx))
			return false;

		return ctx.Write(param5) && ctx.Write(param6);
	}

	override bool Deserializer(Serializer ctx)
	{
		if (!super.Deserializer(ctx))
			return false;

		return ctx.Read(param5) && ctx.Read(param6);
	}
#else
	void ExpansionChatMessageEventParams(int p1, string p2, string p3, string p4, string p5 = "")
	{
		param5 = p5;
	}

	override bool Serialize(Serializer ctx)
	{
		if (!super.Serialize(ctx))
			return false;

		return ctx.Write(param5);
	}

	override bool Deserializer(Serializer ctx)
	{
		if (!super.Deserializer(ctx))
			return false;

		return ctx.Read(param5);
	}
#endif
}
