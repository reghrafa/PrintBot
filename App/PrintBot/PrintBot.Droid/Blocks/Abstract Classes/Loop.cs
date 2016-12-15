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
    public abstract class Loop : IBlock
    {
        public string Name { get; set; }
        public string CodeText { get; set; }
        public string CodeBody { get; set; }
        public string Text { get; set; }
        public bool IsChild { get; set; }
        public void SetCodeText(string s)
        {
            CodeText = s;
        }
    }
}