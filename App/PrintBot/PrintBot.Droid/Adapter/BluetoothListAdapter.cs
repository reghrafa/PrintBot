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

        public BluetoothListAdapter(ObservableCollection<IDevice> list)
        {
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
            var txt_name = convertView.FindViewById<TextView>(Resource.Id.bluetooth_listview_name);
            var txt_adress = convertView.FindViewById<TextView>(Resource.Id.bluetooth_listview_adress);
            txt_name.Text = _list[position].Name;
            txt_adress.Text = _list[position].Id.ToString();

            return convertView;
        }

        private void _list_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            NotifyDataSetChanged();
        }
    }
}