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
using App2.Abstract_Classes;

namespace App2.Comparators
{
    public class NotEquals : Comparator
    {
        public NotEquals()
        {
            Name = "Ist gleich Komparator";
            Text = "ungleich";
        }

        public new string ToString
        {
            get
            {
                return "!=";
            }
        }
    }
}