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
using PrintBot.Domain.Models;
using System.Collections.ObjectModel;

namespace PrintBot.Droid.Adapter
{
    public class FileListAdapter : BaseAdapter<FileModel>
    {
        Activity context;
        ObservableCollection<FileModel> list;

        public FileListAdapter(Activity _context, ObservableCollection<FileModel> _list)
            : base()
        {
            context = _context;
            list = _list;
            _list.CollectionChanged += _list_CollectionChanged;
        }

        private void _list_CollectionChanged(object sender, System.Collections.Specialized.NotifyCollectionChangedEventArgs e)
        {
            NotifyDataSetChanged();
        }

        public override int Count
        {
            get { return list.Count; }
        }

        public override long GetItemId(int position)
        {
            return position;
        }

        public override FileModel this[int index]
        {
            get { return list[index]; }
        }

        public override View GetView(int position, View convertView, ViewGroup parent)
        {
            View view = convertView;

            // re-use an existing view, if one is available
            // otherwise create a new one
            if (view == null)
                view = context.LayoutInflater.Inflate(Resource.Layout.ListRowItem, parent, false);

            FileModel item = this[position];
            view.FindViewById<TextView>(Resource.Id.Title).Text = item.Title;
            view.FindViewById<TextView>(Resource.Id.Description).Text = item.CreationDate.ToString();

            
            return view;
        }
    }
}