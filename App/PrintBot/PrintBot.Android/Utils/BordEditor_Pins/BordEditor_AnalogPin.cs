using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Util;

namespace XamPaint
{
    class BordEditor_AnalogPin : BordEditor_BordPin
    {
        public BordEditor_AnalogPin(Context context) : base(context) { Init(); }
        public BordEditor_AnalogPin(Context context, IAttributeSet attrs): base(context, attrs) { Init(); }

       void Init()
        {
            this.Type = "Analog";
            pw._drawPaint.Color = Android.Graphics.Color.Blue;
            this.SetBackgroundColor(Android.Graphics.Color.DarkBlue);
        }
    }
}