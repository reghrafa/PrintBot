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
using PrintBot.Droid.Fragments;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "Bluetooth Setup")]
    public class BluetoothActivity : PortraitActivity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.bluetooth_layout);
            FragmentTransaction ft = FragmentManager.BeginTransaction();
            ft.Replace(Resource.Id.bluetooth_fragment_container, new BluetoothScanFragment());
            ft.Commit();

            ColorDrawable colorDrawable = new ColorDrawable(Color.ParseColor("#3b8686"));
            ActionBar.SetBackgroundDrawable(colorDrawable);
        }
    }
}