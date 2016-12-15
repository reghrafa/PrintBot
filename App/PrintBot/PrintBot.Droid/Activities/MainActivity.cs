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

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "PrintBot", MainLauncher = true, Icon = "@drawable/icon")]
    public class MainActivity : Activity
    {
        BluetoothViewModel _bluetoothVM = ServiceLocator.Current.BluetoothViewModel;
        Button btnBluetooth;
        Button btnFiles;
        Button btnSettings;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            //_bluetoothVM.PropertyChanged += _bluetoothVM_PropertyChanged;

            // UI Controls
            //btnBluetooth = FindViewById<Button>(Resource.Id.main_bluetooth_button);
            //btnFiles = FindViewById<Button>(Resource.Id.main_files_button);
            //btnSettings = FindViewById<Button>(Resource.Id.main_settings_button);

            // Events
            //btnBluetooth.Click += delegate { ChangeFragment("bluetooth"); };
            //btnFiles.Click += delegate { ChangeFragment("files"); };
            //btnSettings.Click += delegate { ChangeFragment("settings"); };

            var btn = FindViewById<Button>(Resource.Id.main_CreateButton);
            btn.Click += delegate {
                StartActivity(typeof(CodeEditor_BaseActivity));
            };

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
    }
}

