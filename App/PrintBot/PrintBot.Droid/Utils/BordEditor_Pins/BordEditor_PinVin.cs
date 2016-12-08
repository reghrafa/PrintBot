using Android.Content;
using Android.Util;
using Android.Graphics;

namespace PrintBot.Droid
{
    class BordEditor_PinVin : BordEditor_BordPin
    {
        public BordEditor_PinVin(Context context) : base(context) { Init(); }
        public BordEditor_PinVin(Context context, IAttributeSet attrs): base(context, attrs) { Init(); }

        private void Init()
        {
            this.Type = "Vin";
            pw._drawPaint.Color = Color.Green;
            this.SetBackgroundColor(Color.DarkGreen);
        }
    }
}