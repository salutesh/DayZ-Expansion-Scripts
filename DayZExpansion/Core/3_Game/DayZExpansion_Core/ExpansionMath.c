
/**
 * ExpansionMath.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMath
{
	static float LinearConversion(float minFrom, float maxFrom, float value, float minTo = 0, float maxTo = 1, bool clamp = true)
	{
		float newValue = (((value - minFrom) * (maxTo - minTo)) / (maxFrom - minFrom)) + minTo;
		
		if (clamp)
		{
			float min = Math.Min(minTo, maxTo);
			float max = Math.Max(minTo, maxTo);
			return Math.Clamp(newValue, min, max);
		}

		return newValue;
	}

	//! Returns binomial coefficient without explicit use of factorials,
	//! which can't be used with negative integers
	static float PascalTriangle(int a, int b)
	{
		float result = 1; 
		for (int i = 0; i < b; ++i)
		{
			result *= (a - i) / (i + 1);
		}
		return result;
	}

	//! Generalized smoothstep https://en.wikipedia.org/wiki/Smoothstep#Generalization_to_higher-order_equations
	//! This function is well-behaved up to including N=5. Beyond that, errors accumulate, until the function eventually breaks.
	static float SmoothStep(float x, int N = 1, float edge0 = 0, float edge1 = 1)
	{
		x = Math.Clamp((x - edge0) / (edge1 - edge0), 0, 1); //! x must be equal to or between 0 and 1
		float result = 0;
		for (int n = 0; n <= N; ++n)
		{
			result += PascalTriangle(-N - 1, n) * PascalTriangle(2 * N + 1, N - n) * Math.Pow(x, N + n + 1);
		}
		return edge0 + result * (edge1 - edge0);
	}

	//! Alternative to smoothstep
	static float SCurve(float x, float edge0 = 0, float edge1 = 1)
	{
		x = Math.Clamp((x - edge0) / (edge1 - edge0), 0, 1); //! x must be equal to or between 0 and 1
		float result = 0;
		if (x < 0.5)
            result = 2 * Math.Pow(x, 2);
        else
            result = 1 - 2 * Math.Pow(1 - x, 2);
		return edge0 + result * (edge1 - edge0);
	}
}
