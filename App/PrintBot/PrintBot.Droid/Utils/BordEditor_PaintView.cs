using System;
using Android.Views;
using Android.Graphics;
using Android.Content;
using Android.Util;
using Android.Widget;

namespace PrintBot.Droid
{
    public class BordEditor_PaintView : View
    {
        public BordEditor_PaintView(Context context) : base(context, null, 0) { Init(); }
        public BordEditor_PaintView(Context context, IAttributeSet attrs) : base(context, attrs) { Init(); }
        public BordEditor_PaintView(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle) { Init(); }

        public Path _drawPath;
        public Paint _drawPaint;

        void Init()
        {
            _drawPath = new Path();
            _drawPaint = new Paint()
            {
                Color = Color.Green,
                StrokeWidth = 5f,
            };
            _drawPaint.SetStyle(Paint.Style.Stroke);
        }

        public void Clear()
        {
            _drawPath.Reset();
            base.Invalidate();
        }

        protected override void OnDraw(Canvas canvas)
        {
            canvas.DrawPath(_drawPath, _drawPaint);
        }

        public void connectPins(Button pin1, Button pin2)
        {
            
            _drawPath.MoveTo(pin1.GetX(), pin1.GetY());
            _drawPath.LineTo(pin1.GetX(), pin2.GetY());

            _drawPath.MoveTo(pin1.GetX(), pin2.GetY());
            _drawPath.LineTo(pin2.GetX(), pin2.GetY());

            base.Invalidate();
        }

    }
}