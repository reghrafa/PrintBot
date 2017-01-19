using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.Graphics;
using Android.Graphics.Drawables;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Code Editor")]
    class Settings_Editor : PortraitActivity
    {
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.Settings_Layout);

            ColorDrawable colorDrawable = new ColorDrawable(Color.ParseColor("#3b8686"));
            ActionBar.SetBackgroundDrawable(colorDrawable);

            FindViewById<Button>(Resource.Id.Settings_ModuleSetup).Click += delegate
            {
                StartActivity(typeof(BordEditor_MainActivity));
            };

            FindViewById<Button>(Resource.Id.Settings_Bluetooth).Click += delegate
            {
                StartActivity(typeof(BluetoothActivity));
            };

            FindViewById<Button>(Resource.Id.Settings_Other).Click += delegate
            {

            };

        }
    }
}