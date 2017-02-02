using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.Database;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Java.Lang;
using PrintBot.Domain.Models.Blocks;

namespace PrintBot.Droid.Adapter
{
    public class ComparatorSpinnerAdapter : ISpinnerAdapter
    {
        private Context context;
        List<Comparator> List;

        public ComparatorSpinnerAdapter(Context c)
        {
            List = new List<Comparator>() { new Equals(), new NotEquals() };
            context = c;
        }
        public int Count
        {
            get
            {
                return List.Count;
            }
        }

        public IntPtr Handle
        {
            get
            {
                return IntPtr.Zero;
            }
        }

        public bool HasStableIds
        {
            get
            {
                return true;
            }
        }

        public bool IsEmpty
        {
            get
            {
                return List.Count == 0;
            }
        }

        public int ViewTypeCount
        {
            get
            {
                return List.Count;
            }
        }

        public void Dispose()
        {
            
        }

        public View GetDropDownView(int position, View convertView, ViewGroup parent)
        {
            var item = List[position];
            convertView = LayoutInflater.From(context).Inflate(Android.Resource.Layout.SimpleSpinnerDropDownItem, null);
            convertView.FindViewById<TextView>(Android.Resource.Id.Text1).Text = item.ToString;
            return convertView;
        }

        public Comparator GetItem(int position)
        {
            return List[position];
        }

        public long GetItemId(int position)
        {
            throw new NotImplementedException();
        }

        public int GetItemViewType(int position)
        {
            throw new NotImplementedException();
        }

        public View GetView(int position, View convertView, ViewGroup parent)
        {
            throw new NotImplementedException();
        }

        public void RegisterDataSetObserver(DataSetObserver observer)
        {
            throw new NotImplementedException();
        }

        public void UnregisterDataSetObserver(DataSetObserver observer)
        {
            throw new NotImplementedException();
        }

        Java.Lang.Object IAdapter.GetItem(int position)
        {
            return position;
        }
    }
}