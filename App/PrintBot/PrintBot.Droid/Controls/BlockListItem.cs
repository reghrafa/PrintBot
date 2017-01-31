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
        BlockListController _blockListController = ServiceLocator.Current.BlockListController;
        public BlockTypeEnum BlockType { get; set; }
        public IBlockHolder BlockHolder;
        public BlockListItem EndBlock { get; set; }
        public int EndBlockPosition { get; set; }
        public BlockListItem ElseBlock { get; set; }
        public bool ShowsDeleteButton { get; set; }
        public ImageView DeleteButton { get; set; }

        public BlockListItem(Context context) : base(context)
        {
        }

        public BlockListItem(Context context, IAttributeSet attrs) : base(context, attrs)
        {
        }

        public BlockListItem(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle)
        {
        }

        public static BlockListItem GetASavedInstance(Context c, IBlock block)
        {
            var result = new BlockListItem(c);
            switch (block.Name)
            {
                case "Counting Loop":
                    result.BlockType = BlockTypeEnum.CountingLoop;
                    result.BlockHolder = new CountingLoopListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                case "Endless Loop":
                    result.BlockType = BlockTypeEnum.EndlessLoop;
                    result.BlockHolder = new EndlessLoopListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                case "Variable Block":
                    result.BlockType = BlockTypeEnum.Variable;
                    result.BlockHolder = new VariableListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                case "Else Block":
                    result.BlockType = BlockTypeEnum.Else;
                    result.BlockHolder = new ElseListItem(c, block);
                    break;
                case "Led Block":
                    result.BlockType = BlockTypeEnum.LED;
                    result.BlockHolder = new LEDListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                case "If Block":
                    result.BlockType = BlockTypeEnum.IfBlock;
                    result.BlockHolder = new IfListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                case "End If":
                    result.BlockType = BlockTypeEnum.EndBlock;
                    result.BlockHolder = new EndBlockListItem(c, block);
                    break;
                case "End Loop":
                    result.BlockType = BlockTypeEnum.EndBlock;
                    result.BlockHolder = new EndBlockListItem(c, block);
                    break;
                case "Move Block":
                    result.BlockType = BlockTypeEnum.MoveMotor;
                    result.BlockHolder = new MoveListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                case "Rotate Block":
                    result.BlockType = BlockTypeEnum.Rotate;
                    result.BlockHolder = new RotateListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                case "Delay Block":
                    result.BlockType = BlockTypeEnum.Delay;
                    result.BlockHolder = new DelayListItem(c, block);
                    result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
                    result.DeleteButton.Click += result.DeleteButton_Click;
                    break;
                default:
                    break;
            }
            return result;
        }

        public static BlockListItem GetAnInstanceAndInitialize(Context c, BlockListItem item)
        {
            var result = new BlockListItem(c);
            result.BlockType = item.BlockType;
            switch (result.BlockType)
            {
                case BlockTypeEnum.CountingLoop:
                    result.BlockHolder = new CountingLoopListItem(c, new CountingLoop());
                    break;
                case BlockTypeEnum.EndlessLoop:
                    result.BlockHolder = new EndlessLoopListItem(c, new EndlessLoop());
                    break;
                case BlockTypeEnum.IfBlock:
                    result.BlockHolder = new IfListItem(c, new IfBlock());
                    break;
                case BlockTypeEnum.LED:
                    result.BlockHolder = new LEDListItem(c, new LedBlock());
                    break;
                case BlockTypeEnum.MoveMotor:
                    result.BlockHolder = new MoveListItem(c, new MoveBlock());
                    break;
                case BlockTypeEnum.Rotate:
                    result.BlockHolder = new RotateListItem(c, new RotateBlock());
                    break;
                case BlockTypeEnum.Variable:
                    result.BlockHolder = new VariableListItem(c, new VariableBlock());
                    break;
                case BlockTypeEnum.Delay:
                    result.BlockHolder = new DelayListItem(c, new DelayBlock());
                    break;
            }
            result.DeleteButton = result.BlockHolder.BlockLayout.FindViewById<ImageView>(Resource.Id.delete_button);
            result.DeleteButton.Click += result.DeleteButton_Click;
            return result;
        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
            _blockListController.DeleteBlockByObject(this);
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
            EndBlock = 10,
            Rotate = 11,
            Delay
        }
    }
}