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
using Android;

namespace PrintBot.Droid
{
    class BordEditor_ListAdapter : BaseAdapter<string>
    {
        private List<string> _items;
        private Context _context;

        public BordEditor_ListAdapter(Context context, List<string> list)
        {
            this._items = list;
            this._context = context;
        }

        public override string this[int position]
        {
            get
            {
                return _items[position];
            }
        }

        public override int Count
        {
            get
            {
                return _items.Count;
            }
        }

        public List<string> Items
        {
            get
            {
                return _items;
            }

            set
            {
                _items = value;
            }
        }

        public override long GetItemId(int position)
        {
            return position;
        }

        public override View GetView(int position, View convertView, ViewGroup parent)
        {
            View row = convertView;

            if(row == null)
            {
                row = LayoutInflater.From(_context).Inflate(Resource.Layout.BordEditor_ListView_Row, null, false);
            }

            TextView text = row.FindViewById<TextView>(Resource.Id.BordEditor_ListView_Row_filename);
            text.Text = _items[position];
            return row;
        }
    }
}