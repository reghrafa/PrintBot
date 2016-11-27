
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

        private void Init(Context context)
        {
            this.SetBackgroundColor(Color.Aqua);
            /*ImageView img = new ImageView(context);
            img.ScaleY = 1.5f;
            img.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, LayoutParams.MatchParent);
            img.SetImageResource(Resource.Drawable.Platine);
            this.AddView(img);*/

            // digital
            int yOffset = 400;
            for (int i = 0; i <= 13; i++)
            {
                BordEditor_DigitalPin pin = new BordEditor_DigitalPin(context);
                pin.LayoutParameters = new LayoutParams(30, 25);
                pin.TranslationX = 170;
                pin.TranslationY = yOffset;
                pin.parent = this;
                pin.Nr = i;
                this.AddView(pin);
                DigitalPins.Add(pin);

                AddText(145, yOffset, i + "", context);

                yOffset -= 30;

            }

            //Analog
            yOffset = 400;
            for (int i = 0; i < 7; i++)
            {
                BordEditor_AnalogPin pin = new BordEditor_AnalogPin(context);
                pin.LayoutParameters = new LayoutParams(30, 25);
                pin.TranslationX = 0;
                pin.TranslationY = yOffset;
                pin.parent = this;
                pin.Nr = i;
                this.AddView(pin);
                AnalogPins.Add(pin);

                AddText(35, yOffset, $"A{i}", context);

                yOffset -= 30;
            }

            // GND
            yOffset -= 60;

            //5V
            var pinV5 = new BordEditor_Pin5V(context);
            pinV5.LayoutParameters = new LayoutParams(30, 25);
            pinV5.TranslationX = 0;
            pinV5.TranslationY = yOffset;
            pinV5.parent = this;
            this.AddView(pinV5);
            this._pin5V = pinV5;
            AddText(35, yOffset, "5V", context);
            yOffset -= 30;

            // 3V
            var pinV3 = new BordEditor_Pin3V(context);
            pinV3.LayoutParameters = new LayoutParams(30, 25);
            pinV3.TranslationX = 0;
            pinV3.TranslationY = yOffset;
            pinV3.parent = this;
            this.AddView(pinV3);
            this._pin3V = pinV3;
            AddText(35, yOffset, "3V", context);
            yOffset -= 30;

            // Vin
            var pinVin = new BordEditor_PinVin(context);
            pinVin.LayoutParameters = new LayoutParams(30, 25);
            pinVin.TranslationX = 0;
            pinVin.TranslationY = yOffset;
            pinVin.parent = this;
            this.AddView(pinVin);
            this._pinVin = pinVin;
            AddText(35, yOffset, "Vin", context);


        }

        private void AddText(float x, float y, string text, Context context)
        {
            TextView nr = new TextView(context);
            nr.LayoutParameters = new LayoutParams(25, 25);
            nr.SetTextSize(ComplexUnitType.Dip, 7.5f);
            nr.Text = text;
            nr.TranslationX = x;
            nr.TranslationY = y;
            this.AddView(nr);
        }
    }
}