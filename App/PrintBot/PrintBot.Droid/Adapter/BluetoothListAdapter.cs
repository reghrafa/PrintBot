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
using Plugin.BLE.Abstractions.Contracts;
using Android;

namespace PrintBot.Droid.Adapter
{
    public class BluetoothListAdapter : BaseAdapter<IDevice>
    {
        private ObservableCollection<IDevice> _list;
        Context context;

        public BluetoothListAdapter(Context c, ObservableCollection<IDevice> list)
        {
            context = c;
            _list = list;
            _list.CollectionChanged += _list_CollectionChanged;
        }

        public override IDevice this[int position]
        {
            get
            {
                return _list[position];
            }
        }

        public override int Count
        {
            get
            {
                return _list.Count;
            }
        }

        public override long GetItemId(int position)
        {
            return position;
        }

        public override View GetView(int position, View convertView, ViewGroup parent)
        {
            if (convertView == null) convertView = LayoutInflater.From(context).Inflate(Resource.Layout.bluetooth_listview_item, null);
            var txt_name = convertView.FindViewById<TextView>(Resource.Id.bluetooth_listview_name);

            if (string.IsNullOrEmpty(_list[position].Name))
            {
                txt_name.Text = string.Format("Device has no name.");
            }
            else
            {
                txt_name.Text = _list[position].Name;
            }
            return convertView;
        }

        private void _list_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            NotifyDataSetChanged();
        }
    }
}