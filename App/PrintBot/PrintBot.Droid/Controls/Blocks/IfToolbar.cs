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

namespace PrintBot.Droid.Controls.Blocks
{
    public class IfToolbar : BlockListItem
    {
        public IfToolbar(Context context) : base(context)
        {
            init();
        }

        public IfToolbar(Context context, IAttributeSet attrs) : base(context, attrs)
        {
            init();
        }

        public IfToolbar(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle)
        {
            init();
        }

        private void init()
        {
            Inflate(Context, Resource.Layout.Toolbar_If, this);
            BlockType = BlockTypeEnum.IfBlock;
        }
    }
}