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

namespace PrintBot.Droid
{
    public class DraggableListAdapter : BaseAdapter, IDraggableListAdapter
    {
        public ObservableCollection<BlockListItem> List { get; set; }
        public ObservableCollection<IBlock> ListOfIBlocks
        {
            get
            {
                var list = new ObservableCollection<IBlock>();
                foreach (BlockListItem b in List)
                {
                    list.Add(b.BlockHolder.Block);
                }
                return list;
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
            return List[position];
        }

        public override long GetItemId(int position)
        {
            return position;
        }

        public override View GetView(int position, View convertView, ViewGroup parent)
        {
            var item = List[position];
            var result = item.BlockHolder.BlockLayout;
            switch (item.BlockType)
            {
                case BlockListItem.BlockTypeEnum.CountingLoop:
                    var edit = item.FindViewById<EditText>(Resource.Id.CountingLoop_AmountOfLoops);
                    edit.Text = ((CountingLoop)item.BlockHolder.Block).AmountOfLoops.ToString();
                    break;
                default:
                    break;
            }

            var leftMarginForBlock = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WrapContent,
                RelativeLayout.LayoutParams.WrapContent);
            int amountOffset = ListOfIdents[position];
            amountOffset *= 45;
            amountOffset = amountOffset >= 0 ? amountOffset : 0;
            result.SetPadding(amountOffset, 0, 0, 0);
            result.SetMinimumHeight(150);
            result.Visibility = CellPosition == position ? ViewStates.Invisible : ViewStates.Visible;
            result.TranslationY = 0;

            return result;
        }

        public void SwapItems(int from, int to)
        {
            if (from != -1)
            {
                if (to != -1)
                {
                    var valueOne = List[from];
                    var valueTwo = List[to];
                    List[from] = valueTwo;
                    List[to] = valueOne;
                }
            }
        }
    }
}