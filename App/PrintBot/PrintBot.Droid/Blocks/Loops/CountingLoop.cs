using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System.ComponentModel;

namespace App2
{
    public class CountingLoop : Loop
    {
        public int AmountOfLoops { get; set; }

        public CountingLoop()
        {
            Name = "Counting Loop";
            Text = "Zählschleife";
            AmountOfLoops = 5;
            CodeBody = "";
            SetCodeText("for (i = 0; i < " + AmountOfLoops.ToString() + "; i++) {");
        }
    }
}