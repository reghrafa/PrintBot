using System;
using Android.Content;
using Android.Widget;
using Android.Util;
using Android.Graphics;
using Android.Views;
using System.Collections;
using System.Collections.Generic;

namespace PrintBot.Android
{
    class BordEditor_Modul : RelativeLayout
    {
        public List<ModulButton> modulPins = new List<ModulButton>();
        public Button selfDestrucktion;
        public EditText enterModulName;

        string _name;

        public BordEditor_Modul(Context context, int pinCount) : base(context, null, 0) { Init(context, pinCount); }
        public BordEditor_Modul(Context context, BordEditor_ModulPhysical modul) : base(context, null, 0) {
            Init(context, modul.PinList.Count);
            
        }
        //  public Modul_2Pins(Context context, IAttributeSet attrs) : base(context, attrs) { Init(context); }
        //  public Modul_2Pins(Context context, IAttributeSet attrs, int defStyle) : base(context, attrs, defStyle) { Init(context); }

        private void Init(Context context, int pinCount)
        {
            pinCount += 1; // on extra for gnd
            int modulHeight = 110 + (60 * pinCount);
            this.LayoutParameters = new LayoutParams(200, modulHeight);
            this.SetBackgroundColor(Color.PowderBlue);

            //modul name
            enterModulName = new EditText(context);
            enterModulName.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, 50);
            enterModulName.TextSize = 10;
            this.AddView(enterModulName);
            enterModulName.AfterTextChanged += delegate { SetModulName(enterModulName.Text); };

            //delet modul
            selfDestrucktion = new Button(context);
            selfDestrucktion.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, 50);
            selfDestrucktion.TranslationX = 0;
            selfDestrucktion.TranslationY = modulHeight - 50;
            selfDestrucktion.SetBackgroundColor(Color.Blue);
            this.AddView(selfDestrucktion);

            Button save = new Button(context);
            save.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, 50);
            save.TranslationX = 0;
            save.TranslationY = modulHeight - 100;
            save.SetBackgroundColor(Color.Pink);
            this.AddView(save);
            save.Click += delegate { createPhysicalModul(); };


            //create pins
            int yOffset = 60;
            for (int i = 0; i < pinCount; i++)
            {
                var pin = new ModulButton(context, this);
                pin.LayoutParameters = new LayoutParams(50, 50);
                pin.TranslationX = 150;
                pin.TranslationY = yOffset;

                if (i == pinCount - 1)
                {
                    pin.SetBackgroundColor(Color.Black);
                    pin.Clickable = false;
                    TextView gnd = new TextView(context);
                    gnd.LayoutParameters = new LayoutParams(75, 50);
                    gnd.Text = "Gnd";
                    gnd.TranslationX = 75;
                    gnd.TranslationY = yOffset;
                    this.AddView(gnd);

                }
                else
                {
                    pin.SetBackgroundColor(Color.Red);
                    modulPins.Add(pin);
                }

                
                this.AddView(pin);

                yOffset += 60;
            }
        }

        public void AttachSimulator(int pinNr)
        {
            // just testing
            Console.WriteLine("Device is attached to Pin" + pinNr);
        }

        public void SetModulName(String newName)
        {
            this._name = newName;
            Console.WriteLine("Modul name : " + _name);

        }

        public class ModulButton : Button
        {
            private BordEditor_BordPin _connectePin;
            private BordEditor_Modul _modul;

            internal BordEditor_BordPin ConnectePin
            {
                get
                {
                    return _connectePin;
                }

                set
                {
                    _connectePin = value;
                }
            }

            public BordEditor_Modul Modul
            {
                get
                {
                    return _modul;
                }
                set
                {
                    _modul = value;
                }
            }

            public ModulButton(Context context, BordEditor_Modul parent) : base(context, null, 0) { Init(parent); }

            public ModulButton(Context context, IAttributeSet attrs) : base(context, attrs) { }

            private void Init(BordEditor_Modul rel)
            {
                this.Modul = rel;
            }

            public override float GetX()
            {
                return base.GetX() + Width / 2 + Modul.GetX();
            }

            public override float GetY()
            {
                return base.GetY() + Height / 2 + Modul.GetY();
            }
        }

        private bool isModulRightFromBord()
        {
            var metrics = Resources.DisplayMetrics;
            var widthInDp = ConvertPixelsToDp(metrics.WidthPixels);

            return this.GetX() > widthInDp;

        }

        private int ConvertPixelsToDp(float pixelValue)
        {
            var dp = (int)((pixelValue) / Resources.DisplayMetrics.Density);
            return dp;
        }

        public void SwitchPinSideIfNeeded()
        {
            //Console.WriteLine(this.isModulRightFromBord());
            if (this.isModulRightFromBord())
            {
                foreach (ModulButton pin in modulPins)
                {
                    pin.TranslationX = 0;
                }
            }
        }

        public void createPhysicalModul()
        {
            // Save modul by serializertion
            BordEditor_ModulPhysical tmp = new BordEditor_ModulPhysical();
            tmp.Name = this.enterModulName.Text;
            foreach (ModulButton pin in this.modulPins)
            {
                try{
                    var conectPin = pin.ConnectePin; // get conected bord pin
                    var tmpPin = new BordEditor_PysicalPin(conectPin.Type, conectPin.Nr); // create new physical Pin
                    tmp.PinList.Add(tmpPin); // add physical p to PhysicalModul
                }
                catch { }
            }

            tmp.saveModul();
        }
    }
}