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

namespace App2
{
    public class ConditionLoop : Loop
    {
        public Condition Condition { get; set; }

        public ConditionLoop()
        {
            Name = "Condition Loop";
            Text = "Schleife bis";
            Condition = new Condition();
            SetCodeText("while (" + Condition.ToString + ") {");
        }

    }
}