using System;
using Android.Views;
using Android.Widget;
using Java.Lang;
using System.Collections.ObjectModel;
using Android.App;
using Android.Content;
using PrintBot.Droid.Controls;
using PrintBot.Domain.Models.Blocks;
using PrintBot.Droid.Controls.Blocks;
using Android.Text;
using PrintBot.Infrastructure.Services;
using System.Collections.Generic;
using System.Linq;
using Android.Graphics;
using PrintBot.Domain.Models.Blocks.Abstract_Classes;

namespace PrintBot.Droid
{
    public class DraggableListAdapter : BaseAdapter, IDraggableListAdapter
    {

        public ObservableCollection<BlockListItem> List { get; set; }
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
        public List<int> ListOfIdents
        {
            get
            {
                return CodeGenerator.GetIdentForBlocks(ListOfIBlocks);
            }
        }
        public int CellPosition { get; set; }

        Context context;

        public DraggableListAdapter(Context c, ObservableCollection<BlockListItem> list) : base()
        {
            List = list;
            context = c;
            CellPosition = int.MinValue;
            List.CollectionChanged += List_CollectionChanged;
        }

        private void List_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            NotifyDataSetChanged();
        }

        public override int Count
        {
            get
            {
                return List.Count;
            }
        }

        public override Java.Lang.Object GetItem(int position)
        {
            if (position > -1) return List[position];
            return null;
        }

        public override long GetItemId(int position)
        {
            return position;
        }

        public override View GetView(int position, View convertView, ViewGroup parent)
        {
            var item = List[position];
            var result = item.BlockHolder.BlockLayout;
            if (item.EndBlock != null)
            {
                item.BlockHolder.Block.EndBlockPosition = GetItemPosition(item.EndBlock);
            }

            if (item.BlockHolder.Block is StartBlock || item.BlockHolder.Block is CommandBlock)
            {
                var delete_image = ((ViewGroup)result).GetChildAt(((ViewGroup)result).ChildCount - 1);
                delete_image.Visibility = item.ShowsDeleteButton ? ViewStates.Visible : ViewStates.Gone;
                delete_image.Clickable = true;
            }

            int amountOffset = ListOfIdents[position];
            amountOffset *= 60;
            amountOffset = amountOffset >= 0 ? amountOffset : 0;
            result.SetPadding(amountOffset, 0, 0, 0);
            result.SetMinimumHeight(0);
            result.Visibility = CellPosition == position ? ViewStates.Invisible : ViewStates.Visible;
            result.TranslationY = 0;
            return result;
        }

        public int GetItemPosition(BlockListItem item)
        {
            return List.ToList().FindIndex(a => a == item);
        }

        public void AddDeleteImageView(int position)
        {
            var image = new ImageView(context, null, Resource.Style.delete_block_button);
            List[position].AddView(image);
        }

        public void DeleteImageView(int position)
        {
            var image = new ImageView(context, null, Resource.Style.delete_block_button);
            List[position].RemoveViewAt(List[position].ChildCount - 1);
        }

        public void HideAllDeleteButtons()
        {
            foreach (BlockListItem item in List)
            {
                item.ShowsDeleteButton = false;
            }
        }

        public void SwapItems(int from, int to, bool isAbove)
        {
            if (from > -1 && to > -1)
            {
                var item = List[from];
                if (item.EndBlock != null)
                {
                    var endblockPos = GetItemPosition(item.EndBlock);

                    if (isAbove)
                    {
                        for (int i = from; i <= endblockPos; i++)
                        {
                            SwapSingleItem(i, to);
                            to++;
                        }
                    }
                    else
                    {
                        for (int i = endblockPos; i >= from; i--)
                        {
                            SwapSingleItem(i, to);
                            to--;
                        }
                    }
                }
                else
                {
                    SwapSingleItem(from, to);
                }
            }
        }

        private void SwapSingleItem(int from, int to)
        {
            if (from > -1 && to > -1)
            {
                var valueOne = List[from];
                var valueTwo = List[to];
                List[from] = valueTwo;
                List[to] = valueOne;
            }
        }
    }
}