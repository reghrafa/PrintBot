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

namespace App2.Abstract_Classes
{
    /// <summary>
    /// Comparators: Equals, NotEquals, LessThan, LessThanOrEqual, GreaterThan, GreaterThanOrEqual, And, Or
    /// </summary>
    public abstract class Comparator
    {
        public string Name { get; set; }
        public string Text { get; set; }
        public string Code { get; set; }
        public new string ToString { get; set; }
    }
}