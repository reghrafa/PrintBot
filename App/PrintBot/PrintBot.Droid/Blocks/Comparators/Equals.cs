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
    public class Equals : Comparator
    {
        public Equals()
        {
            Name = "Ist ungleich Comparator";
            Text = "Entspricht";
            ToString = "==";
        }
    }
}