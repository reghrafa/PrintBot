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
using PrintBot.Droid.Adapter;
using PrintBot.Infrastructure.ViewModels;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "LastUsedFileActivity")]
    public class LastUsedFileActivity : Activity
    {
        private LastUsedFileViewModel _lastUsedFileViewModel;
        private EditText _projectName;
        private ListView _listView;
        protected async override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            SetContentView(Resource.Layout.LastUsedFileLayout);
            Button but1 = FindViewById<Button>(Resource.Id.button1);
            _projectName = FindViewById<EditText>(Resource.Id.editText1);
            but1.Click += But1_Click; ;

            _listView = FindViewById<ListView>(Resource.Id.FileList);
            _listView.Adapter = new FileListAdapter(this, _lastUsedFileViewModel.FileList);

            _listView.ItemClick += (s, e) => {
                var path = _lastUsedFileViewModel.ChangeCreationDate(e.Position);

                var fileOpenedActivity = new Intent(this, typeof(FileOpenedActivity));
                fileOpenedActivity.PutExtra("Path", path);
                StartActivity(fileOpenedActivity);
            };

            _lastUsedFileViewModel = ServiceLocator.Current.LastUsedFileViewModel;
            _lastUsedFileViewModel.PropertyChanged += _lastUsedFileViewModel_PropertyChanged;
            await _lastUsedFileViewModel.LoadData();
        }

        private void _lastUsedFileViewModel_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            ((BaseAdapter)_listView.Adapter).NotifyDataSetChanged();
        }

        private async void But1_Click(object sender, EventArgs e)
        {
            await _lastUsedFileViewModel.AddFile(_projectName.Text);
        }

        protected override void OnResume()
        {
            base.OnResume();
            _lastUsedFileViewModel.Sort();
        }
        protected async override void OnStop()
        {
            base.OnStop();
            await _lastUsedFileViewModel.Save();
        }
    }
}