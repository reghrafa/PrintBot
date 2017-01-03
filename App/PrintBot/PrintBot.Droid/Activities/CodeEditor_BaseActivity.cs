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
            FindViewById<TextView>(Resource.Id.main_ProgramName).Text = filename;

            FindViewById<Button>(Resource.Id.CodeEditor_SettingsButton).Click += delegate
            {
                StartActivity(typeof(Settings_Editor));
            };

            FindViewById<Button>(Resource.Id.CodeEditor_SaveButton).Click += async delegate
            {
                await _lastUsedFileViewModel.SaveFile(filename, ListOfIBlocks);
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
                        List.Add(tmp);
                        break;
                    case "Endless Loop":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.EndlessLoop;
                        tmp.BlockHolder = new EndlessLoopListItem(this, block);
                        List.Add(tmp);
                        break;
                    case "Variable Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.Variable;
                        tmp.BlockHolder = new VariableListItem(this, block);
                        List.Add(tmp);
                        break;
                    case "Else Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.Else;
                        tmp.BlockHolder = new ElseListItem(this, block);
                        List.Add(tmp);
                        break;
                    case "Led Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.LED;
                        tmp.BlockHolder = new LEDListItem(this, block);
                        List.Add(tmp);
                        break;
                    case "If Block":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.IfBlock;
                        tmp.BlockHolder = new IfListItem(this, block);
                        List.Add(tmp);
                        break;
                    case "End If":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.EndBlock;
                        tmp.BlockHolder = new EndBlockListItem(this, block);
                        List.Add(tmp);
                        break;
                    case "End Loop":
                        tmp.BlockType = BlockListItem.BlockTypeEnum.EndBlock;
                        tmp.BlockHolder = new EndBlockListItem(this, block);
                        List.Add(tmp);
                        break;
                    default:
                        break;

                }
            }
        }
    }
}