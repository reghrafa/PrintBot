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
using System.Linq;
using Android.Views.InputMethods;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "PrintBot", WindowSoftInputMode = Android.Views.SoftInput.AdjustNothing, MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : PortraitActivity
    {
        BluetoothViewModel _bluetoothVM = ServiceLocator.Current.BluetoothViewModel;

        private LastUsedFileViewModel _lastUsedFileVM = ServiceLocator.Current.LastUsedFileViewModel;
        ListView _listOldFiles;
        private FileModel _selectedFile;
        private ImageView _addButton;
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
            _addButton = mCustomView.FindViewById<ImageView>(Resource.Id.Main_addButton);
            _addButton.Click += _addButton_Click;

            var fileName = Intent.GetStringExtra("DeletedFileName") ?? "";
            if (!string.IsNullOrEmpty(fileName))
            {
                var file = _lastUsedFileVM.FileList.First(a => a.FileName == fileName);
                await _lastUsedFileVM.DeleteFile(file);
            }

            await _lastUsedFileVM.LoadData();

            _listOldFiles = FindViewById<ListView>(Resource.Id.main_LastFileList);
            _listOldFiles.Adapter = new FileListAdapter(this, _lastUsedFileVM.FileList);
                
       
            _listOldFiles.ItemClick += (s, e) =>
            {
                        var filename = _lastUsedFileVM.ChangeCreationDate(e.Position);
                        var tmp = new Intent(this, typeof(CodeEditor_BaseActivity));
                        tmp.PutExtra("Path", filename);
                        StartActivity(tmp);
            };
            _listOldFiles.ItemLongClick += (s, e) =>
            {
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.SetPositiveButton("Delete", async (se, evt) =>
                    {
                        var index = _lastUsedFileVM.FileList.IndexOf(_selectedFile);
                        await _lastUsedFileVM.DeleteFile(_selectedFile);
                        if (_lastUsedFileVM.FileList.Count > 0)
                        {
                            _listOldFiles.SetSelection(index);
                            _selectedFile = _lastUsedFileVM.FileList[index];
                        }
                        else
                        {
                            _selectedFile = null;
                            _listOldFiles.Selected = false;
                        }
                    });
                    builder.SetNegativeButton("Cancel", (se, evt) =>
                    {
                    
                    });
                    builder.SetMessage($"Do you really want to delete file {_lastUsedFileVM.FileList[e.Position].FileName}?");
                    builder.Show();
                    
            };
            
        }
        
        private void _addButton_Click(object sender, EventArgs e)
        {
            var inputDialog = new AlertDialog.Builder(this);
            EditText userInput = new EditText(this);
            userInput.Hint = "Enter a project name";
            userInput.InputType = Android.Text.InputTypes.TextFlagCapCharacters;
            inputDialog.SetView(userInput);
            inputDialog.SetPositiveButton("Create", async (s, evt) =>
            {
                await _lastUsedFileVM.AddFile(userInput.Text);
                HideKeyboard(userInput);
            });
            inputDialog.SetNegativeButton("Cancel", (s, evt) =>
            {
                HideKeyboard(userInput);
            });
            inputDialog.Show();
            ShowKeyboard(userInput);
        }
        private void ShowKeyboard(EditText userInput)
        {
            userInput.RequestFocus();
            InputMethodManager imm = (InputMethodManager)this.GetSystemService(Context.InputMethodService);
            imm.ToggleSoftInput(ShowFlags.Forced, 0);
        }
        private void HideKeyboard(EditText userInput)
        {
            InputMethodManager imm = (InputMethodManager)this.GetSystemService(Context.InputMethodService);
            //imm.HideSoftInputFromWindow(userInput.WindowToken, 0);
            imm.HideSoftInputFromWindow(this.CurrentFocus.WindowToken, HideSoftInputFlags.None);
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

