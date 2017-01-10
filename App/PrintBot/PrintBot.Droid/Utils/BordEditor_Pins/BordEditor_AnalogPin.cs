using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Util;
using Android.Graphics;

namespace PrintBot.Droid
{
    class BordEditor_AnalogPin : BordEditor_BordPin
    {
        public BordEditor_AnalogPin(Context context) : base(context) { Init(); }
        public BordEditor_AnalogPin(Context context, IAttributeSet attrs): base(context, attrs) { Init(); }

       void Init()
        {
            this.Type = "Analog";
            pw._drawPaint.Color = Color.Blue;
            this.SetBackgroundColor(Color.DarkBlue);
            this.SetTextColor(Color.Yellow);
            this.BackgroundColor = Color.DarkBlue;
        }
    }
}