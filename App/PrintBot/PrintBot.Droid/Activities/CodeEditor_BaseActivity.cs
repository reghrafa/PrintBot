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

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Code Editor")]
    class CodeEditor_BaseActivity : Activity
    {
        private CodeEditorViewModel _codeEditorViewModel = ServiceLocator.Current.CodeEditorViewModel;
        public LastUsedFileViewModel _lastUsedFileViewModel = ServiceLocator.Current.LastUsedFileViewModel;
        public ObservableCollection<BlockListItem> List { get; set; }
        private ObservableCollection<IBlock> _listOfIBlocks;
        public ObservableCollection<IBlock> ListOfIBlocks
        {
            get
            {
                _listOfIBlocks = new ObservableCollection<IBlock>();
                foreach (BlockListItem b in List)
                {
                    _listOfIBlocks.Add(b.BlockHolder.Block);
                }
                return _listOfIBlocks;
            }
        }

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.CodeEditor_Layout);
            List = new ObservableCollection<BlockListItem>();
            string filename = Intent.GetStringExtra("Path") ?? "no Data";
            string content = Intent.GetStringExtra("Content") ?? "no Data";
            FindViewById<TextView>(Resource.Id.main_ProgramName).Text = content;

            FindViewById<Button>(Resource.Id.CodeEditor_SettingsButton).Click += delegate
            {
                StartActivity(typeof(Settings_Editor));
            };

            FindViewById<Button>(Resource.Id.CodeEditor_SaveButton).Click += async delegate
            {
                await _lastUsedFileViewModel.SaveFile(filename, ListOfIBlocks);
            };

            FragmentTransaction ft = FragmentManager.BeginTransaction();
            ft.Add(Resource.Id.CodeEditor_FragmentContainer, new FragmentWorkspace());
            ft.Commit();
            FragmentTransaction ft2 = FragmentManager.BeginTransaction();
            ft2.Add(Resource.Id.CodeEditor_FragmentContainerTools, new FragmentTools());
            ft2.Commit();
        }
    }
}