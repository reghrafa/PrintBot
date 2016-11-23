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
using Android.Util;
using static Android.Views.ViewGroup;

namespace XamPaint
{
    class BordEditor_DigitalPin : BordEditor_BordPin
    {
        public BordEditor_DigitalPin(Context context) : base(context) { init(); }
        public BordEditor_DigitalPin(Context context, IAttributeSet attrs): base(context, attrs) { init(); }

        void init()
        {
            this.Type = "Digital";
            pw._drawPaint.Color = Android.Graphics.Color.Yellow;
            this.SetBackgroundColor(Android.Graphics.Color.Gold);
        }
    }
}