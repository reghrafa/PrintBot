using Android.Content;
using Android.Util;
using Android.Graphics;

namespace PrintBot.Droid
{
    class BordEditor_Pin3V : BordEditor_BordPin
    {
        public BordEditor_Pin3V(Context context) : base(context) { Init(); }
        public BordEditor_Pin3V(Context context, IAttributeSet attrs): base(context, attrs) { Init(); }

        private void Init()
        {
            this.Type = "V3";
            pw._drawPaint.Color = Color.DarkRed;
            this.SetBackgroundColor(Color.Red);
            this.SetTextColor(Color.DarkGreen);
            this.BackgroundColor = Color.Red;
        }
    }
}