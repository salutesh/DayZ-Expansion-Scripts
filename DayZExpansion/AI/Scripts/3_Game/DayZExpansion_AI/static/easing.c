modded class Easing
{
	/**
	 * @brief Easing function with variable exponent
	 * 
	 * Can be used as replacement for EaseInOutQuad/Cubic/Quart/Quint
	 * Integer exponents 2 through 5 will match the output of these functions, floating point exponents will blend between them
	 * n = 1 will produce linear output
	 */
	static float EaseInOutPow(float t, float n = 2) 
	{
		if (t < 0.5)
			return 0.5 * Math.Pow(2 * t, n);
		else
			return 1 - 0.5 * Math.Pow(2 * (1 - t), n);
	}
}
