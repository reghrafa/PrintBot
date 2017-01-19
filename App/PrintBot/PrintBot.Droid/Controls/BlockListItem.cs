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
using PrintBot.Droid.Controls.Blocks;
using System.Collections.ObjectModel;

namespace PrintBot.Droid.Controls
{
    /// <summary>
    /// Layout for one item in the ListView.
    /// </summary>
    public class BlockListItem : RelativeLayout
    {
        public BlockTypeEnum BlockType { get; set; }
        public IBlockHolder BlockHolder;
        public BlockListItem EndBlock { get; set; }
        public int EndBlockPosition { get; set; }
        public BlockListItem ElseBlock { get; set; }

        public BlockListItem(Context context) : base(context)
        {
        }

        public BlockListItem(Context context, IAttributeSet attrs) : base(context, attrs)
        {
        }

        public BlockListItem(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle)
        {
        }

        public BlockListItem GetAnInstanceAndInitialize()
        {
            var result = new BlockListItem(Context);
            result.BlockType = BlockType;
            switch (result.BlockType)
            {
                case BlockTypeEnum.CountingLoop:
                    result.BlockHolder = new CountingLoopListItem(Context, new CountingLoop());
                    break;
                case BlockTypeEnum.EndlessLoop:
                    result.BlockHolder = new EndlessLoopListItem(Context, new EndlessLoop());
                    break;
                case BlockTypeEnum.IfBlock:
                    result.BlockHolder = new IfListItem(Context, new IfBlock());
                    break;
                case BlockTypeEnum.LED:
                    result.BlockHolder = new LEDListItem(Context, new LedBlock());
                    break;
                case BlockTypeEnum.MoveMotor:
                    result.BlockHolder = new MoveListItem(Context, new MoveBlock());
                    break;
                case BlockTypeEnum.Variable:
                    result.BlockHolder = new VariableListItem(Context, new VariableBlock());
                    break;
            }
            return result;
        }

        public BlockListItem GetAnInstanceOfEndBlock(BlockTypeEnum blockType)
        {
            var result = new BlockListItem(Context);
            result.BlockType = BlockTypeEnum.EndBlock;
            switch (blockType)
            {
                case BlockTypeEnum.CountingLoop:
                    result.BlockHolder = new EndBlockListItem(Context, new EndLoop());
                    break;
                case BlockTypeEnum.EndlessLoop:
                    result.BlockHolder = new EndBlockListItem(Context, new EndLoop());
                    break;
                case BlockTypeEnum.IfBlock:
                    result.BlockHolder = new EndBlockListItem(Context, new EndIf());
                    break;
            }
            return result;
        }

        public BlockListItem GetAnInstanceOfElseBlock()
        {
            var result = new BlockListItem(Context);
            result.BlockType = BlockTypeEnum.Else;
            result.BlockHolder = new ElseListItem(Context, new ElseBlock());
            return result;
        }

        public enum BlockTypeEnum
        {
            CountingLoop = 1,
            EndlessLoop = 2,
            MoveMotor = 5,
            Variable = 6,
            LED = 7,
            IfBlock = 8,
            Else = 9,
            EndBlock = 10
        }
    }
}