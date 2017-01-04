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
    public class MoveListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        private EditText editAcceleration;

        public MoveListItem(Context c, IBlock block)
        {
            BlockContext = c;
            Block = block;
            init();
        }

        public void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_Move, null);
            // Other Layout controls
            editAcceleration = BlockLayout.FindViewById<EditText>(Resource.Id.BlockListItem_Move_Acceleration);
            editAcceleration.Text = ((MoveBlock)Block).Acceleration.ToString();
            editAcceleration.TextChanged += EditAcceleration_TextChanged;
        }

        private void EditAcceleration_TextChanged(object sender, Android.Text.TextChangedEventArgs e)
        {
            // Save the new value from user input
            int result;
            bool success = int.TryParse(e.Text.ToString(), out result);
            if (success)
            {
                ((MoveBlock)Block).Acceleration = result;
            }
            else
            {
                ((MoveBlock)Block).Acceleration = 0;
            }
        }
    }
}