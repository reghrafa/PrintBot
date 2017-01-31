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
using PrintBot.Infrastructure.ViewModels;
using Android.Content.Res;
using System.IO;
using System.Text.RegularExpressions;

namespace PrintBot.Droid.Activities
{
    [Activity(Label = "TestActivity")]
    public class TestActivity : Activity
    {
        private BluetoothViewModel _vm = ServiceLocator.Current.BluetoothViewModel;
        protected override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);
            SetContentView(Resource.Layout.TestActivityLayout);

            FindViewById<Button>(Resource.Id.test_epic_shit).Click += TestActivity_Click;
            
            // Create your application here
        }

        private async void TestActivity_Click(object sender, EventArgs e)
        {
            // Read Hex
            string hexFile;
            AssetManager assets = Assets;
            using (StreamReader sr = new StreamReader(assets.Open("samplehex.txt")))
            {
                hexFile = sr.ReadToEnd();
            }
            var split = Regex.Split(hexFile, @"?<=[:]");
            _vm.WriteAsync(Encoding.ASCII.GetBytes("A"));


            var r = await _vm.ReadAsync();


        }
    }
}