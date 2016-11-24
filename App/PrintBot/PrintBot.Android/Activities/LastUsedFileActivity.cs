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
using PrintBot.Android.Adapter;
using PrintBot.Infrastructure.ViewModels;

namespace PrintBot.Android.Activities
{
    [Activity(Label = "LastUsedFileActivity")]
    public class LastUsedFileActivity : Activity
    {
        private LastUsedFileViewModel _lastUsedFileViewModel;
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            SetContentView(Resource.Layout.LastUsedFileLayout);
            Button but1 = FindViewById<Button>(Resource.Id.button1);
            tw = FindViewById<EditText>(Resource.Id.editText1);
            but1.Click += But1_Click;

            listView = FindViewById<ListView>(Resource.Id.FileList);
            listView.Adapter = new FileListAdapter(this, filelist);

            listView.ItemClick += (s, e) => {
                var path = _lastUsedFileViewModel.ChangeCreationDate(e.Position);

                var fileOpenedActivity = new Intent(this, typeof(FileOpened));
                fileOpenedActivity.PutExtra("Path", path);
                StartActivity(fileOpenedActivity);
            };

            _lastUsedFileViewModel = ServiceLocator.Current.LastUsedFileViewModel;
            _lastUsedFileViewModel.OnPropertyChanged += (e) => { ((BaseAdapter)listView.Adapter).NotifyDataSetChanged(); };
            _lastUsedFileViewModel.LoadData();
        }
        protected override void OnResume()
        {
            base.OnResume();
            filelist.Sort((a, b) => b.CreationDate.CompareTo(a.CreationDate));
            ((BaseAdapter)listView.Adapter).NotifyDataSetChanged();
        }
        protected override void OnStop()
        {
            base.OnStop();
            writeandrefresh();
        }
    }
}