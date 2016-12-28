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

namespace PrintBot.Droid
{
    public class DraggableListAdapter : BaseAdapter, IDraggableListAdapter
    {
        public ObservableCollection<BlockListItem> List { get; set; }
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
            convertView = item.BlockHolder.BlockLayout;
            switch (item.BlockType)
            {
                case BlockListItem.BlockTypeEnum.CountingLoop:
                    var edit = convertView.FindViewById<EditText>(Resource.Id.CountingLoop_AmountOfLoops);
                    edit.Text = ((CountingLoop)item.BlockHolder.Block).AmountOfLoops.ToString();
                    break;
                default:
                    break;
            }

            convertView.SetMinimumHeight(100);
            convertView.Visibility = CellPosition == position ? ViewStates.Invisible : ViewStates.Visible;
            convertView.TranslationY = 0;

            return convertView;
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