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
    public class RotateListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        private EditText editDegrees;

        public RotateListItem(Context c, IBlock block)
        {
            BlockContext = c;
            Block = block;
            init();
        }

        public void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_Rotate, null);
            // Other Layout controls
            editDegrees = BlockLayout.FindViewById<EditText>(Resource.Id.BlockListItem_Rotate_Degrees);
            editDegrees.Text = ((RotateBlock)Block).Degrees.ToString();
            editDegrees.TextChanged += EditAcceleration_TextChanged;
        }

        private void EditAcceleration_TextChanged(object sender, Android.Text.TextChangedEventArgs e)
        {
            // Save the new value from user input
            int result;
            bool success = int.TryParse(e.Text.ToString(), out result);
            if (success)
            {
                ((RotateBlock)Block).Degrees = result;
            }
            else
            {
                ((RotateBlock)Block).Degrees = 0;
            }
        }
    }
}