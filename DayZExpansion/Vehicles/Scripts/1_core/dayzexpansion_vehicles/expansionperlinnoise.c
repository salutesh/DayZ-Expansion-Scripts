class ExpansionPerlinNoiseGenerator
{
    //! Simple pseudo-random hash function
    static float Hash(float n)
    {
        float sine = Math.Sin(n) * 43758.5453123;
        return sine - Math.Floor(sine); 
    }

    static float GetValue(float x)
    {
        float i = Math.Floor(x);
        float f = x - i;
        
        //! Smoothstep
        float u = f * f * (3.0 - 2.0 * f);

        return Math.Lerp(Hash(i), Hash(i + 1.0), u);
    }
}
