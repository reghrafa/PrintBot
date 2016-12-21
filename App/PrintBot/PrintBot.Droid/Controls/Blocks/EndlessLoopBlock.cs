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
using PrintBot.Droid.Services;
using PrintBot.Domain.Models.Blocks;

namespace PrintBot.Droid.Controls.Blocks
{
    public class EndlessLoopBlock : IBlockLayoutHolder
    {
        public IBlock Block { get; set; }
        private View _blockLayout;
        public View BlockLayout
        {
            get
            {
                return _blockLayout;
            }
            set
            {
                _blockLayout = value;
            }
        }
        private Context context;

        public EndlessLoopBlock(Context c, IBlock block)
        {
            this.Block = block;
            context = c;
            init();
        }

        private void init()
        {
            BlockLayout = LayoutInflater.From(context).Inflate(Resource.Layout.Block_EndlessLoop, null);
        }
    }
}