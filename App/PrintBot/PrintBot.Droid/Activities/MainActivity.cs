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
using Android.Graphics.Drawables;
using Android.Graphics;
using PrintBot.Domain.Models;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "PrintBot", WindowSoftInputMode = Android.Views.SoftInput.AdjustNothing, MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : PortraitActivity
    {
        BluetoothViewModel _bluetoothVM = ServiceLocator.Current.BluetoothViewModel;

        private LastUsedFileViewModel _lastUsedFileVM = ServiceLocator.Current.LastUsedFileViewModel;
        ListView _listOldFiles;
        EditText NewProjectName;
        private FileModel _selectedFile;
        private ImageView _deleteButton;

        protected override async void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);
            ActionBar.LayoutParams lp = new ActionBar.LayoutParams(ActionBar.LayoutParams.WrapContent, ActionBar.LayoutParams.WrapContent, GravityFlags.Right | GravityFlags.CenterVertical);
            LayoutInflater mInflater = LayoutInflater.From(this);
            View mCustomView = mInflater.Inflate(Resource.Layout.actionbar_main_layout, null);
            ActionBar.SetCustomView(mCustomView, lp);
            ColorDrawable colorDrawable = new ColorDrawable(Color.ParseColor("#3b8686"));
            ActionBar.SetBackgroundDrawable(colorDrawable);
            ActionBar.SetDisplayShowCustomEnabled(true);
            _deleteButton = mCustomView.FindViewById<ImageView>(Resource.Id.Main_deleteButton);
            _deleteButton.Click += _deleteButton_Click;
            _deleteButton.Visibility = _selectedFile == null ? ViewStates.Gone : ViewStates.Visible;

            var btn = FindViewById<Button>(Resource.Id.main_CreateButton);
            btn.Click += CreateFile;

            NewProjectName = FindViewById<EditText>(Resource.Id.main_NewProjectName);

            await _lastUsedFileVM.LoadData();
            _listOldFiles = FindViewById<ListView>(Resource.Id.main_LastFileList);
            _listOldFiles.Adapter = new FileListAdapter(this, _lastUsedFileVM.FileList);
            _listOldFiles.ItemClick += (s, e) =>
            {
                if (_selectedFile != null)
                {
                    if (_selectedFile == _lastUsedFileVM.FileList[e.Position])
                    {
                        var filename = _lastUsedFileVM.ChangeCreationDate(e.Position);
                        var tmp = new Intent(this, typeof(CodeEditor_BaseActivity));
                        tmp.PutExtra("Path", filename);
                        StartActivity(tmp);
                    }
                }
                _selectedFile = _lastUsedFileVM.FileList[e.Position];
                _deleteButton.Visibility = _selectedFile == null ? ViewStates.Gone : ViewStates.Visible;
            };
        }

        private async void _deleteButton_Click(object sender, EventArgs e)
        {
            if (_selectedFile != null)
            {
                await _lastUsedFileVM.DeleteFile(_selectedFile);
                _listOldFiles.Selected = false;
            }
        }

        private async void CreateFile(Object sender, EventArgs e)
        {
            await _lastUsedFileVM.AddFile(NewProjectName.Text);
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

