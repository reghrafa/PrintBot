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
using PrintBot.Droid.Services;

namespace PrintBot.Droid.Controls
{
    /// <summary>
    /// Layout for one item in the ListView.
    /// </summary>
    public class BlockLayout : RelativeLayout
    {
        private BlockTypeEnum _blockType;
        public BlockTypeEnum BlockType
        {
            get
            {
                return _blockType;
            }
            set
            {
                if (_blockType != value)
                {
                    _blockType = value;
                }
            }
        }

        public IBlock Block;
        public IBlockLayoutHolder BlockLayoutHolder;

        public BlockLayout(Context context) : base(context)
        {
        }

        public BlockLayout(Context context, IAttributeSet attrs) : base(context, attrs)
        {
        }

        public BlockLayout(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle)
        {
        }

        public BlockLayout GetAnInstance()
        {
            return new BlockLayout(Context);
        }

        public enum BlockTypeEnum
        {
            CountingLoop = 1,
            EndlessLoop = 2,
            ForLoop = 3,
            DoWhileLoop = 4,
            MoveMotor = 5,
            Variable = 6,
            LED = 7,
            IfBlock = 8
        }
    }
}