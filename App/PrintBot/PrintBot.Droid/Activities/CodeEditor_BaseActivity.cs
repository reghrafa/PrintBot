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
    class CodeEditor_BaseActivity : Activity
    {
        private CodeEditorViewModel _codeEditorViewModel = ServiceLocator.Current.CodeEditorViewModel;
        public BlockListViewController _blockListViewController = ServiceLocator.Current.BlockListViewController;
        private bool _isOnCodePage = false;
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
            string filename = Intent.GetStringExtra("Path") ?? "no Data";
            var content = await _codeEditorViewModel.LoadData(filename);
            
            FindViewById<TextView>(Resource.Id.main_ProgramName).Text = filename;
            var SwitchButton = FindViewById<ImageButton>(Resource.Id.CodeEditor_SwitchButton);
            SwitchButton.Click += delegate
            {
                if (_isOnCodePage)
                {
                    SwitchButton.SetImageResource(Resource.Drawable.ListIcon);
                    ChangeFragment(new CodeViewFragment());
                    _codeEditorViewModel.GenerateCode(_blockListViewController.ListOfIBlocks);
                }
                else
                {
                    SwitchButton.SetImageResource(Resource.Drawable.CodeIcon);
                    ChangeFragment(new FragmentWorkspace());
                }
                _isOnCodePage = !_isOnCodePage;
            };


            FindViewById<ImageButton>(Resource.Id.CodeEditor_SettingsButton).Click += delegate
            {
                StartActivity(typeof(Settings_Editor));
            };
            

            FindViewById<ImageButton>(Resource.Id.CodeEditor_SaveButton).Click += async delegate
            {
                await _codeEditorViewModel.SaveFile(filename, _blockListViewController.ListOfIBlocks);
                Toast.MakeText(this, "Program saved.", ToastLength.Short).Show();
            };

            try
            {
                var iBlocks = JsonConvert.DeserializeObject<ObservableCollection<IBlock>>(content, new JsonSerializerSettings { TypeNameHandling = TypeNameHandling.Auto });
                CreateSavedList(iBlocks);
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
        private void ChangeFragment(Fragment f)
        {
            FragmentTransaction ft = FragmentManager.BeginTransaction();
            ft.Replace(Resource.Id.CodeEditor_FragmentContainer, f);
            ft.Commit();
        }

        private void CreateSavedList(ObservableCollection<IBlock> listOfBlocks)
        {
            foreach (IBlock block in listOfBlocks)
            {
                var tmp = new BlockListItem(this);
                switch (block.Name)
                {
                    case "Counting Loop":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.CountingLoop;
                        tmp.BlockHolder = new CountingLoopListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "Endless Loop":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.EndlessLoop;
                        tmp.BlockHolder = new EndlessLoopListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "Variable Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.Variable;
                        tmp.BlockHolder = new VariableListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "Else Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.Else;
                        tmp.BlockHolder = new ElseListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "Led Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.LED;
                        tmp.BlockHolder = new LEDListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "If Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.IfBlock;
                        tmp.BlockHolder = new IfListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "End If":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.EndBlock;
                        tmp.BlockHolder = new EndBlockListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "End Loop":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.EndBlock;
                        tmp.BlockHolder = new EndBlockListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    case "Move Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.MoveMotor;
                        tmp.BlockHolder = new MoveListItem(this, block);
                        _blockListViewController.List.Add(tmp);
                        break;
                    default:
                        break;

                }
            }
        }
    }
}