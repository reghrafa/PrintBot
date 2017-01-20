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
using System.Collections.ObjectModel;
using PrintBot.Domain.Models.Blocks;
using PrintBot.Droid.Controls.Blocks;

namespace PrintBot.Droid.Controls
{
    public class BlockListViewController
    {
        private ObservableCollection<BlockListItem> _list;
        public ObservableCollection<BlockListItem> List
        {
            get { return _list ?? (_list = new ObservableCollection<BlockListItem>()); }
            set { _list = value; }
        }
        private ObservableCollection<IBlock> _listOfIBlocks;
        public ObservableCollection<IBlock> ListOfIBlocks
        {
            get
            {
                _listOfIBlocks = new ObservableCollection<IBlock>();
                foreach (BlockListItem b in List)
                {
                    _listOfIBlocks.Add(b.BlockHolder.Block);
                }
                return _listOfIBlocks;
            }
        }
        public BlockListViewController() { }

        public void SetEndBlockInstances()
        {
            foreach (BlockListItem item in List)
            {
                if (item.BlockHolder.Block.EndBlockPosition > 0)
                {
                    item.EndBlock = List[item.BlockHolder.Block.EndBlockPosition];
                }
            }
        }

        public void CreateSavedList(Context context, ObservableCollection<IBlock> list)
        {
            foreach (IBlock block in list)
            {
                List.Add(BlockListItem.GetASavedInstance(context, block));
            }
            SetEndBlockInstances();
        }

        public void DeleteBlockByObject(BlockListItem item)
        {
            if (item.EndBlock != null)
            {
                var pos = List.IndexOf(item);
                var endPos = List.IndexOf(item.EndBlock);
                if (pos >= 0 && endPos >= 0)
                {
                    for (int i = endPos; i >= pos; i--)
                    {
                        List.RemoveAt(i);
                    }
                }
            }
            else
            {
                List.Remove(item);
            }
        }

        public void InsertBlockToList(Context c, BlockListItem block, int position)
        {
            var tmpBlock = BlockListItem.GetAnInstanceAndInitialize(c, block);
            position = position == -1 ? List.Count : position;
            List.Insert(position, tmpBlock);

            if (tmpBlock.BlockType == BlockListItem.BlockTypeEnum.IfBlock)
            {
                position++;
                List.Insert(position, block.GetAnInstanceOfElseBlock());
                tmpBlock.ElseBlock = List[position];
            }
            if (tmpBlock.BlockHolder.Block is StartBlock)
            {
                position++;
                List.Insert(position, block.GetAnInstanceOfEndBlock(block.BlockType));
                tmpBlock.EndBlock = List[position];
            }
        }
    }
}