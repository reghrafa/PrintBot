using System;
using Android.Views;
using Android.Widget;
using Java.Lang;
using System.Collections.ObjectModel;
using Android.App;
using Android.Content;

namespace DragAndDrop.Droid
{
    public class DraggableListAdapter : BaseAdapter, IDraggableListAdapter
    {
        public ObservableCollection<Button> List { get; set; }
        public int CellPosition { get; set; }

        Context context;

        public DraggableListAdapter(Context c, ObservableCollection<Button> list) : base()
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
            if (convertView == null)
            {
                convertView = LayoutInflater.From(context).Inflate(Resource.Layout.list_view_item, null);
                convertView.SetMinimumHeight(50);
                convertView.SetBackgroundColor(Android.Graphics.Color.AliceBlue);
            }

            var item = List[position];
            var btn = convertView.FindViewById<Button>(Resource.Id.listView_button);
            btn.Text = $" Hello { item.Text }";
            btn.Clickable = false;
            btn.LongClickable = false;

            convertView.Visibility = CellPosition == position ? ViewStates.Invisible : ViewStates.Visible;
            convertView.TranslationY = 0;

            return convertView;
        }

        public void SwapItems(int from, int to)
        {
            var valueOne = List[from];
            var valueTwo = List[to];
            List[from] = valueTwo;
            List[to] = valueOne;
            NotifyDataSetChanged();
        }
    }
}