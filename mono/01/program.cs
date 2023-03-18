using System;
using System.Runtime.InteropServices;

class Program
{
    [DllImport("libsum.so")]
    public static extern int sum(int a, int b);

    static void Main(string[] args)
    {
        int a = 2;
        int b = 3;
        int result = sum(a, b);

        Console.WriteLine("{0} + {1} = {2}", a, b, result);
    }
}
