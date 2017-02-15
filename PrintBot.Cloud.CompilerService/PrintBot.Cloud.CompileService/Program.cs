using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;

namespace PrintBot.Cloud.CompileService
{
    public class Program
    {
        public static void Main(string[] args)
        {
            Process.Start(new ProcessStartInfo (
    "yo",
    "aspnet")
    { UseShellExecute = false });
            Console.ReadLine();
        }
    }
}
