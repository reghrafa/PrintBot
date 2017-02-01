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
using PrintBot.Droid.Services;
using PrintBot.Domain.Models.Blocks;

namespace PrintBot.Droid.Controls.Blocks
{
    /// <summary>
    /// Saving the layout to interact with user input
    /// </summary>
    public class CountingLoopListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        private EditText editAmountOfLoops;

        public CountingLoopListItem(Context c, IBlock block)
        {
            this.Block = block;
            BlockContext = c;
            init();
        }

        private void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_CountingLoop, null);
            editAmountOfLoops = BlockLayout.FindViewById<EditText>(Resource.Id.CountingLoop_AmountOfLoops);
            editAmountOfLoops.Text = ((CountingLoop)Block).AmountOfLoops.ToString();
            editAmountOfLoops.TextChanged += EditAmountOfLoops_TextChanged;
        }

        private void EditAmountOfLoops_TextChanged(object sender, Android.Text.TextChangedEventArgs e)
        {
            // Save the new value from user input
            int result;
            bool success = int.TryParse(e.Text.ToString(), out result);
            if (success)
            {
                ((CountingLoop)Block).AmountOfLoops = result;

            }
            else
            {
                ((CountingLoop)Block).AmountOfLoops = 0;
            }
        }
    }
}