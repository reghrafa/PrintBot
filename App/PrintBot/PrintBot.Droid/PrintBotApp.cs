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
using PrintBot.Droid.Utils;

namespace PrintBot.Droid
{
    [Application]
    public class PrintBotApplication : Application
    {
        public PrintBotApplication(IntPtr handle, JniHandleOwnership ownership) : base(handle, ownership)
        {
        }

        public override void OnCreate()
        {
            base.OnCreate();

            //FontsOverride.SetDefaultFont(this, "DEFAULT", "blablabla.ttf");
        }
    }
}