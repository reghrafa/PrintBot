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
using PrintBot.Droid.Activities;
using PrintBot.Infrastructure.ViewModels;
using System.Collections.ObjectModel;
using PrintBot.Droid.Controls;
using PrintBot.Domain.Models.Blocks;
using Newtonsoft.Json;
using PrintBot.Droid.Controls.Blocks;
using Android.Graphics.Drawables;
using Android.Graphics;
using PrintBot.Droid.Fragments;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Code Editor")]
    class CodeEditor_BaseActivity : PortraitActivity
    {
        private CodeEditorViewModel _codeEditorViewModel = ServiceLocator.Current.CodeEditorViewModel;
        public BlockListViewController _blockListViewController = ServiceLocator.Current.BlockListViewController;
        private bool _isOnCodePage = false;
        private string _fileName;
        public ImageView DeleteBlockImage { get; set; }
        protected override async void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.CodeEditor_Layout);

            ActionBar.LayoutParams lp = new ActionBar.LayoutParams(ActionBar.LayoutParams.WrapContent, ActionBar.LayoutParams.WrapContent, GravityFlags.Right | GravityFlags.CenterVertical);
            LayoutInflater mInflater = LayoutInflater.From(this);
            View mCustomView = mInflater.Inflate(Resource.Layout.actionbar_layout, null);
            ActionBar.SetCustomView(mCustomView, lp);
            ColorDrawable colorDrawable = new ColorDrawable(Color.ParseColor("#3b8686"));
            ActionBar.SetBackgroundDrawable(colorDrawable);
            ActionBar.SetDisplayShowCustomEnabled(true);

            _blockListViewController.List = new ObservableCollection<BlockListItem>();
            _fileName = Intent.GetStringExtra("Path") ?? "no Data";
            var content = await _codeEditorViewModel.LoadData(_fileName);

            FindViewById<TextView>(Resource.Id.main_ProgramName).Text = _fileName;
            var SwitchButton = FindViewById<ImageButton>(Resource.Id.CodeEditor_SwitchButton);
            var toolbarFragment = FindViewById<FrameLayout>(Resource.Id.CodeEditor_FragmentContainerTools);
            DeleteBlockImage = FindViewById<ImageView>(Resource.Id.CodeEditor_deleteButton);
            DeleteBlockImage.Click += DeleteBlockImage_Click;
            SwitchButton.Click += delegate
            {
                if (_isOnCodePage)
                {
                    SwitchButton.SetImageResource(Resource.Drawable.ListIcon);
                    ChangeFragment(new CodeViewFragment());
                    toolbarFragment.Visibility = ViewStates.Gone;
                    _codeEditorViewModel.GenerateCode(_blockListViewController.ListOfIBlocks);
                }
                else
                {
                    SwitchButton.SetImageResource(Resource.Drawable.CodeIcon);
                    ChangeFragment(new FragmentWorkspace());
                    toolbarFragment.Visibility = ViewStates.Visible;
                }
                _isOnCodePage = !_isOnCodePage;
            };


            FindViewById<ImageButton>(Resource.Id.CodeEditor_SettingsButton).Click += delegate
            {
                StartActivity(typeof(Settings_Editor));
            };


            FindViewById<ImageButton>(Resource.Id.CodeEditor_SaveButton).Click += async delegate
            {
                await _codeEditorViewModel.SaveFile(_fileName, _blockListViewController.ListOfIBlocks);
                Toast.MakeText(this, "Program saved.", ToastLength.Short).Show();
            };

            try
            {
                var iBlocks = JsonConvert.DeserializeObject<ObservableCollection<IBlock>>(content, new JsonSerializerSettings { TypeNameHandling = TypeNameHandling.Auto });
                _blockListViewController.CreateSavedList(this, iBlocks);
            }
            catch (Exception e)
            {
                // Error message log
            }

            FragmentTransaction ft = FragmentManager.BeginTransaction();
            ft.Add(Resource.Id.CodeEditor_FragmentContainer, new FragmentWorkspace());
            ft.Commit();
            FragmentTransaction ft2 = FragmentManager.BeginTransaction();
            ft2.Add(Resource.Id.CodeEditor_FragmentContainerTools, new FragmentTools());
            ft2.Commit();
        }

        private void DeleteBlockImage_Click(object sender, EventArgs e)
        {
            _codeEditorViewModel.DeleteFileByName(_fileName);
        }

        private void DeleteBlockImage_Drag(object sender, View.DragEventArgs e)
        {
            var evt = e.Event;            
            var item = (BlockListItem)sender;
            switch (evt.Action)
            {
                case DragAction.Started:
                    e.Handled = true;
                    break;
                case DragAction.Entered:
                    break;
                case DragAction.Exited:
                    break;
                case DragAction.Drop:
                    e.Handled = true;
                    _blockListViewController.DeleteBlockByObject(item);
                    break;
                case DragAction.Ended:
                    e.Handled = true;
                    break;
                case DragAction.Location:
                    break;
            }
        }

        private void ChangeFragment(Fragment f)
        {
            FragmentTransaction ft = FragmentManager.BeginTransaction();
            ft.Replace(Resource.Id.CodeEditor_FragmentContainer, f);
            ft.Commit();
        }
    }
}