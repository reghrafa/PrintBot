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
using PrintBot.Domain.Models.Blocks;

namespace PrintBot.Droid.Services
{
    /// <summary>
    /// Holds the Layout and IBlock for each ListItem in the ListView
    /// </summary>
    public interface IBlockHolder
    {
        View BlockLayout { get; set; }
        IBlock Block { get; set; }
        Context BlockContext { get; set; }
    }
}