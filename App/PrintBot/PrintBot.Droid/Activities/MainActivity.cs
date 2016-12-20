using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using PrintBot.Infrastructure.ViewModels;
using PrintBot.Droid.Fragments;
using Android;
using PrintBot.Droid.Adapter;
using System.ComponentModel;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "PrintBot", WindowSoftInputMode = Android.Views.SoftInput.AdjustNothing, MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        BluetoothViewModel _bluetoothVM = ServiceLocator.Current.BluetoothViewModel;

        private LastUsedFileViewModel _lastUsedFileVM;
        ListView _listOldFiles;
        EditText NewProjectName;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            _lastUsedFileVM = ServiceLocator.Current.LastUsedFileViewModel;
            _lastUsedFileVM.PropertyChanged += _lUFVM_PropertyChanged;

            var btn = FindViewById<Button>(Resource.Id.main_CreateButton);
            btn.Click += CreateFile;

            NewProjectName = FindViewById<EditText>(Resource.Id.main_NewProjectName);

            _listOldFiles = FindViewById<ListView>(Resource.Id.main_LastFileList);
            _listOldFiles.Adapter = new FileListAdapter(this,_lastUsedFileVM.FileList);
            _listOldFiles.ItemClick += (s, e) =>
            {
                var path = _lastUsedFileVM.ChangeCreationDate(e.Position);
                var tmp = new Intent(this, typeof(CodeEditor_BaseActivity));
                tmp.PutExtra("Path", path);
                StartActivity(tmp);
            };
        }

        private async void CreateFile(Object sender, EventArgs e)
        {
            await _lastUsedFileVM.AddFile(NewProjectName.Text);
        }

        private void _lUFVM_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            ((BaseAdapter)_listOldFiles.Adapter).NotifyDataSetChanged();
        }

        private void _bluetoothVM_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName == nameof(BluetoothViewModel.Connected))
            {
                ChangeFragment("bluetooth");
            }
        }

        private void ChangeFragment(string fragment)
        {
            FragmentTransaction ft = FragmentManager.BeginTransaction();
            switch (fragment)
            {
                case "bluetooth":
                    if (!_bluetoothVM.Connected)
                    {
                        //ft.Replace(Resource.Id.main_fragment_container, new BluetoothScanFragment());
                    }
                    else
                    {
                        //ft.Replace(Resource.Id.main_fragment_container, new BluetoothSendFragment());
                    }
                    break;
                case "files":
                    // ft.Replace(Resource.Id.main_fragment_container, new FilesFragment());
                    var fileBrowserActivity = new Intent(this, typeof(LastUsedFileActivity));
                    StartActivity(fileBrowserActivity);
                    
                    break;
                case "settings":
                    // ft.Replace(Resource.Id.main_fragment_container, new SettingsFragment());
                    break;
                default:
                    break;
            }
            ft.Commit();
        }

        protected override void OnResume()
        {
            base.OnResume();
            if (_lastUsedFileVM != null)
            {
                _lastUsedFileVM.Sort();
            }

        }

        protected async override void OnStop()
        {
            base.OnStop();
            await _lastUsedFileVM.Save();
        }
    }
}

