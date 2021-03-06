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
    public class EndBlockListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        private TextView txtBlockText;

        public EndBlockListItem(Context c, IBlock block)
        {
            this.Block = block;
            BlockContext = c;
            init();
        }

        private void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_EndBlock, null);
            txtBlockText = BlockLayout.FindViewById<TextView>(Resource.Id.BlockListItem_EndBlock_Text);
            txtBlockText.Text = Block is EndIf ? "End If" : "End Loop";
            var mainLayout = BlockLayout.FindViewById<LinearLayout>(Resource.Id.block_endblock_layout);
            var drawableRes = Block is EndIf ? Resource.Drawable.block_orange_bg : Resource.Drawable.block_purple_bg;
            mainLayout.SetBackgroundResource(drawableRes);
        }
    }
}