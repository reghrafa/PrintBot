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
    public class DelayListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        private EditText editDelayTime;

        public DelayListItem(Context c, IBlock block)
        {
            this.Block = block;
            BlockContext = c;
            init();
        }

        private void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_Delay, null);
            editDelayTime = BlockLayout.FindViewById<EditText>(Resource.Id.BlockListItem_Delay_DelayTime);
            editDelayTime.Text = ((DelayBlock)Block).DelayTime.ToString();
            editDelayTime.TextChanged += EditDelayTime_TextChanged;
        }

        private void EditDelayTime_TextChanged(object sender, Android.Text.TextChangedEventArgs e)
        {
            int result;
            bool success = int.TryParse(e.Text.ToString(), out result);
            if (success)
            {
                ((DelayBlock)Block).DelayTime = result;

            }
            else
            {
                ((DelayBlock)Block).DelayTime = 0;
            }
        }
    }
}