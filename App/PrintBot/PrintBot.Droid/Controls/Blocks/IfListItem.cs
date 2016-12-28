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
using PrintBot.Droid.Services;

namespace PrintBot.Droid.Controls.Blocks
{
    public class IfListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }

        public IfListItem(Context c, IBlock block)
        {
            BlockContext = c;
            Block = block;
            init();
        }

        public void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_If, null);
            // Other Layout controls
        }
    }
}