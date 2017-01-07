
using System.Collections.Generic;
using Android.Content;
using Android.Widget;
using Android.Graphics;
using Android.Util;
using System.Collections;

namespace PrintBot.Droid
{
    class BordEditor_Bord : RelativeLayout
    {
        public BordEditor_Bord(Context context) : base(context, null, 0)
        {
            Init(context);
        }
        public BordEditor_Bord(Context context, IAttributeSet attrs) : base(context, attrs)
        {
            Init(context);
        }
        public BordEditor_Bord(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle)
        { //Init(context); 
        }

        private List<BordEditor_DigitalPin> digitalPins = new List<BordEditor_DigitalPin>();
        private List<BordEditor_AnalogPin> _analogPins = new List<BordEditor_AnalogPin>();
        private BordEditor_Pin5V _pin5V;
        private BordEditor_Pin3V _pin3V;
        private BordEditor_PinVin _pinVin;

        public List<BordEditor_AnalogPin> AnalogPins
        {
            get
            {
                return _analogPins;
            }

            set
            {
                _analogPins = value;
            }
        }

        public BordEditor_Pin5V Pin5V
        {
            get
            {
                return _pin5V;
            }

            set
            {
                _pin5V = value;
            }
        }

        public BordEditor_Pin3V Pin3V
        {
            get
            {
                return _pin3V;
            }

            set
            {
                _pin3V = value;
            }
        }

        public BordEditor_PinVin PinVin
        {
            get
            {
                return _pinVin;
            }

            set
            {
                _pinVin = value;
            }
        }

        public List<BordEditor_DigitalPin> DigitalPins
        {
            get
            {
                return digitalPins;
            }

            set
            {
                digitalPins = value;
            }
        }

        public void Init(Context context)
        {
            this.SetBackgroundColor(Color.Aqua);

            var scale = (int)PrintBot.Droid.Activities.BordEditor_MainActivity._scaleFactor;

            var w = 100 * scale;
            var h = 390 * scale;

            var pinW = 25 * scale;
            var pinH = 25 * scale;
            var pinTransX = w - pinW;

            var OffsetDec = 30 * scale;
            var textX = w - 2 * pinW;

            this.LayoutParameters = new LayoutParams(w, h);

            float tmp = PrintBot.Droid.Activities.BordEditor_MainActivity._screenWidth / 2;
            this.TranslationX = tmp - w / 2;

            // digital
            int yOffset = h - pinH;
            for (int i = 0; i <= 13; i++)
            {
                BordEditor_DigitalPin pin = new BordEditor_DigitalPin(context);
                pin.LayoutParameters = new LayoutParams(pinW, pinH);
                pin.TranslationX = pinTransX;
                pin.TranslationY = yOffset;
                pin.parent = this;
                pin.Nr = i;
                this.AddView(pin);
                DigitalPins.Add(pin);
                pin.Text = i + "";

                //AddText(textX, yOffset, i + "", context);

                yOffset -= OffsetDec;

            }

            //Analog
            yOffset = h - pinH;
            for (int i = 0; i < 7; i++)
            {
                BordEditor_AnalogPin pin = new BordEditor_AnalogPin(context);
                pin.LayoutParameters = new LayoutParams(pinW, pinH);
                pin.TranslationX = 0;
                pin.TranslationY = yOffset;
                pin.parent = this;
                pin.Nr = i;
                this.AddView(pin);
                AnalogPins.Add(pin);
                pin.Text = $"A{i}";

                //AddText(75, yOffset, $"A{i}", context);

                yOffset -= OffsetDec;
            }

            // GND
            yOffset -= 2*OffsetDec;

            //5V
            var pinV5 = new BordEditor_Pin5V(context);
            pinV5.LayoutParameters = new LayoutParams(pinW, pinH);
            pinV5.TranslationX = 0;
            pinV5.TranslationY = yOffset;
            pinV5.parent = this;
            this.AddView(pinV5);
            this._pin5V = pinV5;
            pinV5.Text = "5V";
            yOffset -= OffsetDec;

            // 3V
            var pinV3 = new BordEditor_Pin3V(context);
            pinV3.LayoutParameters = new LayoutParams(pinW, pinH);
            pinV3.TranslationX = 0;
            pinV3.TranslationY = yOffset;
            pinV3.parent = this;
            this.AddView(pinV3);
            this._pin3V = pinV3;
            pinV3.Text = "3V";
            yOffset -= OffsetDec;

            // Vin
            var pinVin = new BordEditor_PinVin(context);
            pinVin.LayoutParameters = new LayoutParams(pinW, pinH);
            pinVin.TranslationX = 0;
            pinVin.TranslationY = yOffset;
            pinVin.parent = this;
            this.AddView(pinVin);
            this._pinVin = pinVin;
            pinVin.Text = "Vin";


        }

        private void AddText(float x, float y, string text, Context context)
        {
            TextView nr = new TextView(context);
            nr.LayoutParameters = new LayoutParams(25, 25);
            nr.SetTextSize(ComplexUnitType.Dip, 10f);
            nr.SetTextColor(Color.Black);
            nr.Text = text;
            nr.TranslationX = x;
            nr.TranslationY = y;
            this.AddView(nr);
        }
    }
}












