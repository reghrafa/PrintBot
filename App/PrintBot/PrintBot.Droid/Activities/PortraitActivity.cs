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
    [Activity(Label = "PortraitActivity")]
    public class PortraitActivity : Activity
    {
        protected override void OnCreate(Bundle savedInstanceState)
        {
            RequestedOrientation = Android.Content.PM.ScreenOrientation.Portrait;
            base.OnCreate(savedInstanceState);
            
            // Create your application here
        }
    }
}