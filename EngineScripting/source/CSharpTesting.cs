using System;

public class CSharpTesting
{
    public float FloatVar = 5.0f;


    private string m_Name = "Hello";
    public string Name 
    {
        get => m_Name;
        
        set 
        {
            m_Name = value;
            FloatVar += 5.0f;
        }
    }
    
    public void PrintVar()
    {
        Console.WriteLine("CSharp Var = {0:F}", FloatVar);
    }

    private void IncrementVar(float value)
    {
        FloatVar += value;
    }
}

