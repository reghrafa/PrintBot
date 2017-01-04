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
    public class LEDListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        private ToggleButton tglIsOn;

        public LEDListItem(Context c, IBlock block)
        {
            BlockContext = c;
            Block = block;
            init();
        }

        public void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_LED, null);
            // Other Layout controls
            tglIsOn = BlockLayout.FindViewById<ToggleButton>(Resource.Id.BlockListItem_LED_toggleIsOn);
            tglIsOn.Checked = ((LedBlock)Block).IsOn;
            tglIsOn.Click += TglIsOn_Click;
        }

        private void TglIsOn_Click(object sender, EventArgs e)
        {
            ((LedBlock)Block).IsOn = tglIsOn.Checked;
        }
    }
}