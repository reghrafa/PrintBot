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

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Code Editor")]
    class CodeEditor_BaseActivity : Activity
    {
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);
            SetContentView(Resource.Layout.CodeEditor_Layout);

            string text = Intent.GetStringExtra("Path") ?? "no Data";
            FindViewById<TextView>(Resource.Id.main_Sampel).Text = text;

            FindViewById<Button>(Resource.Id.CodeEditor_SettingsButton).Click += delegate
            {
                StartActivity(typeof(Settings_Editor));
            };

        }
    }
}