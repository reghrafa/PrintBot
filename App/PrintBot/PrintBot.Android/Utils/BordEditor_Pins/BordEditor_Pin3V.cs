using Android.Content;
using Android.Util;

namespace XamPaint
{
    class BordEditor_Pin3V : BordEditor_BordPin
    {
        public BordEditor_Pin3V(Context context) : base(context) { Init(); }
        public BordEditor_Pin3V(Context context, IAttributeSet attrs): base(context, attrs) { Init(); }

        private void Init()
        {
            this.Type = "V3";
            pw._drawPaint.Color = Android.Graphics.Color.DarkRed;
            this.SetBackgroundColor(Android.Graphics.Color.Red);
        }
    }
}