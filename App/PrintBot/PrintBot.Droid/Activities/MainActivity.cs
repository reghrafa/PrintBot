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
        Button btnBluetooth;
        Button btnFiles;
        Button btnSettings;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);
            
            // UI Controls
            btnBluetooth = FindViewById<Button>(Resource.Id.main_bluetooth_button);
            btnFiles = FindViewById<Button>(Resource.Id.main_files_button);
            btnSettings = FindViewById<Button>(Resource.Id.main_settings_button);

            // Events
            btnBluetooth.Click += delegate { ChangeFragment("bluetooth"); };
            btnFiles.Click += delegate { ChangeFragment("files"); };
            btnSettings.Click += delegate { ChangeFragment("settings"); };
        }

        private void ChangeFragment(string fragment)
        {
            FragmentTransaction ft = FragmentManager.BeginTransaction();
            switch (fragment)
            {
                case "bluetooth":
                    ft.Replace(Resource.Id.main_fragment_container, new BluetoothFragment());
                    break;
                case "files":
                    // ft.Replace(Resource.Id.main_fragment_container, new FilesFragment());
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

