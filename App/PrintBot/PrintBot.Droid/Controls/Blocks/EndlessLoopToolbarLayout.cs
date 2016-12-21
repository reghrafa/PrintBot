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
using Android.Util;
using PrintBot.Domain.Models.Blocks;

namespace PrintBot.Droid.Controls.Blocks
{
    public class EndlessLoopToolbarLayout : BlockLayout
    {
        public EndlessLoopToolbarLayout(Context context) : base(context)
        {
            init();
        }

        public EndlessLoopToolbarLayout(Context context, IAttributeSet attrs) : base(context, attrs)
        {
            init();
        }

        public EndlessLoopToolbarLayout(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle)
        {
            init();
        }

        private void init()
        {
            Inflate(Context, Resource.Layout.Toolbar_EndlessLoop, this);
            BlockType = BlockTypeEnum.EndlessLoop;
            Block = new EndlessLoop();
        }
    }
}