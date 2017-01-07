using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Graphics;
using Android.Util;

namespace PrintBot.Droid.Utils
{
    class BordEditor_ModulSlot : RelativeLayout
    {
        public Button CreateBtn { get; set; }
        private TextView _pinCountTextView;
        private Button _pinCountINC;
        private Button _pinCountDEC;
        private ListView _moduleFileNamesList;
        public Button loadNamesBtn;

        public BordEditor_ListAdapter Adapter { get; set; }

        public int PinCount
        {
            get
            {
                return _pinCount;
            }

            set
            {
                _pinCount = value;
            }
        }

        public ListView ModuleFileNamesList
        {
            get
            {
                return _moduleFileNamesList;
            }

            set
            {
                _moduleFileNamesList = value;
            }
        }

        private int _pinCount = 2;

        public BordEditor_ModulSlot(Context context, BordEditor_ListAdapter adapter,bool isRightFromBorad) : base(context, null, 0)
        {
            Init(context, adapter, isRightFromBorad);
        }

        private void Init(Context context, BordEditor_ListAdapter adapter, bool isRightfromBorad)
        {
            Adapter = adapter;

            var scale = PrintBot.Droid.Activities.BordEditor_MainActivity._scaleFactor;
            var screenWidth = PrintBot.Droid.Activities.BordEditor_MainActivity._screenWidth;

            int w = (int)scale * 100;
            int h = (int)scale * 250;
            int buttonHeight = (int)scale * 25;
            int textViewHeight = (int)scale * 25;

            int yOffset = 0; // postion of elements

            this.LayoutParameters = new LayoutParams(w, h);
            this.SetBackgroundColor(Color.LightGray);

            //Create
            CreateBtn = new Button(context);
            CreateBtn.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, 2 * buttonHeight);
            CreateBtn.Text = "NEW";
            CreateBtn.TranslationY = yOffset;
            this.AddView(CreateBtn);
            yOffset += 2 * buttonHeight;

            //Place Slot rigth From Bord;
            if (isRightfromBorad)
            {
                this.TranslationX = screenWidth - w;
            }

            //PinCount
            _pinCountTextView = new TextView(context);
            _pinCountTextView.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, textViewHeight);
            _pinCountTextView.Text = $"{_pinCount} pins";
            _pinCountTextView.TextAlignment = TextAlignment.Center;
            _pinCountTextView.SetTextColor(Color.Black);
            _pinCountTextView.TranslationY = yOffset;
            this.AddView(_pinCountTextView);
            yOffset += textViewHeight;

            // inc Pins
            _pinCountINC = new Button(context);
            _pinCountINC.LayoutParameters = new LayoutParams(2 * buttonHeight, buttonHeight * 2);
            _pinCountINC.Text = "+";
            _pinCountINC.TranslationY = yOffset;
            this.AddView(_pinCountINC);
            _pinCountINC.Click += delegate
            {
                SetPinCount(1);
            };


            // dec Pins
            _pinCountDEC = new Button(context);
            _pinCountDEC.LayoutParameters = new LayoutParams(2 * buttonHeight, buttonHeight * 2);
            _pinCountDEC.TranslationY = yOffset;
            _pinCountDEC.Text = "-";
            _pinCountDEC.TranslationX = w / 2;
            this.AddView(_pinCountDEC);
            _pinCountDEC.Click += delegate
            {
                SetPinCount(-1);
            };
            yOffset += 2 * buttonHeight;

            //load
            loadNamesBtn = new Button(context);
            loadNamesBtn.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, buttonHeight * 2);
            loadNamesBtn.TranslationY = yOffset;
            loadNamesBtn.Text = "Load";
            loadNamesBtn.SetTextColor(Color.White);
            this.AddView(loadNamesBtn);
            yOffset += buttonHeight * 2;

            //scroll
            var tmp = new ScrollView(context);
            tmp.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, buttonHeight * 2);
            tmp.SetBackgroundColor(Color.White);
            tmp.TranslationY = yOffset;
            this.AddView(tmp);

            //list
            _moduleFileNamesList = new ListView(context);
            _moduleFileNamesList.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, LayoutParams.WrapContent);
            _moduleFileNamesList.Adapter = Adapter;

            tmp.AddView(_moduleFileNamesList);

        }

        private void SetPinCount(int amount)
        {
            var tmp = _pinCount + amount;
            tmp = tmp < 2 ? 2 : tmp;

            _pinCount = tmp;
            _pinCountTextView.Text = $"{_pinCount} pins";
        }
    }
}