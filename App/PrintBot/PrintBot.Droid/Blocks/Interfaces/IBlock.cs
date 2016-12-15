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
    public interface IBlock
    {
        string CodeText { get; set; }
        string Name { get; set; }
        string Text { get; set; }
        bool IsChild { get; set; }
    }
}