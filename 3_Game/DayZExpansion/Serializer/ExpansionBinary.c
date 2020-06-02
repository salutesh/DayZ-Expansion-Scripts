class ExpansionBinary
{
	protected Serializer m_src = NULL;

	Serializer GetSource()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionBinary::GetSource - Return: " + m_src.ToString() );
		#endif
		return m_src;
	}
}