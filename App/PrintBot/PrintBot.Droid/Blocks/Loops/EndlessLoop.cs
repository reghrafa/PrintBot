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
    public class EndlessLoop : Loop
    {
        public EndlessLoop()
        {
            Name = "Endless Loop";
            Text = "Dauerschleife";
            CodeBody = "";
            IsChild = false;
            SetCodeText("while (true) { ");
        }
    }
}