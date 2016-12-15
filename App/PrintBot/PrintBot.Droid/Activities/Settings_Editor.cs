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

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Code Editor")]
    class Settings_Editor : Activity
    {
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.Settings_Layout);

            FindViewById<LinearLayout>(Resource.Id.Settings_ModuleSetup).Click += delegate
            {
                StartActivity(typeof(BordEditor_MainActivity));
            };

            FindViewById<LinearLayout>(Resource.Id.Settings_Bluetooth).Click += delegate
            {

            };

            FindViewById<LinearLayout>(Resource.Id.Settings_Other).Click += delegate
            {

            };

        }
    }
}