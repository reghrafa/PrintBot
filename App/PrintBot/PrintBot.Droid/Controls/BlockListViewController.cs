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

        public void InsertBlockToList(BlockListItem block, int position)
        {
            var tmpBlock = block.GetAnInstanceAndInitialize();
            position = position == -1 ? List.Count : position;
            List.Insert(position, tmpBlock);

            if (tmpBlock.BlockType == BlockListItem.BlockTypeEnum.IfBlock)
            {
                position++;
                List.Insert(position, block.GetAnInstanceOfElseBlock());
            }
            if (tmpBlock.BlockHolder.Block is StartBlock)
            {
                position++;
                List.Insert(position, block.GetAnInstanceOfEndBlock(block.BlockType));
            }
        }
    }
}