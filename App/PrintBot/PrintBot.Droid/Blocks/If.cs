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
using App2.Comparators;

namespace App2
{
    public class IfBlock : IBlock
    {
        private Condition condition;
        public string CodeText { get; set; }
        public IfBlock()
        {
            Equals equ = new Equals();
            condition = new Condition(1,3,equ);
            CodeText = "If (" + condition + ") {";
        }
        public bool IsChild { get; set; }

        public string Name
        {get;set;
        }

        public string Text
        {
            get; set;
        }
    }
}