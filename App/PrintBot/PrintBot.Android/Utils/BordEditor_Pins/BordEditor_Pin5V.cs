using Android.Content;
using Android.Util;

namespace PrintBot.Android
{
    class BordEditor_Pin5V : BordEditor_BordPin
    {
        public BordEditor_Pin5V(Context context) : base(context) { Init(); }
        public BordEditor_Pin5V(Context context, IAttributeSet attrs): base(context, attrs) { Init(); }

        private void Init()
        {
            this.Type = "V5";
            pw._drawPaint.Color = Android.Graphics.Color.Red;
            this.SetBackgroundColor(Android.Graphics.Color.DarkRed);
        }
    }
}