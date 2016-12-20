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
    /// <summary>
    /// Layout for counting loop in the toolbar
    /// </summary>
    public class CountingLoopToolbarLayout : BlockLayout
    {
        public CountingLoopToolbarLayout(Context context) : base(context)
        {
            init();
        }

        public CountingLoopToolbarLayout(Context context, IAttributeSet attrs) : base(context, attrs)
        {
            init();
        }

        public CountingLoopToolbarLayout(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle)
        {
            init();
        }

        private void init()
        {
            Inflate(Context, Resource.Layout.Toolbar_CountingLoop, this);
            BlockType = BlockTypeEnum.CountingLoop;
            //blockLayout = new CountingLoopBlock(this);
            //block = new CountingLoop();
        }
    }
}