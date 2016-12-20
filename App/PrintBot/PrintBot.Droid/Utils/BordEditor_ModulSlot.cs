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

        public BordEditor_ModulSlot(Context context, BordEditor_ListAdapter adapter) : base(context, null, 0)
        {
            Init(context, adapter);
        }

        private void Init(Context context, BordEditor_ListAdapter adapter)
        {
            Adapter = adapter;

            this.LayoutParameters = new LayoutParams(200, 300);
            this.SetBackgroundColor(Color.LightGray);

            //Create
            CreateBtn = new Button(context);
            CreateBtn.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, 100);
            CreateBtn.Text = "NEW";
            this.AddView(CreateBtn);

            //PinCount
            _pinCountTextView = new TextView(context);
            _pinCountTextView.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, 50);
            _pinCountTextView.TranslationY = 100;
            _pinCountTextView.Text = $"{_pinCount} pins";
            _pinCountTextView.SetTextColor(Color.Black);
            this.AddView(_pinCountTextView);

            // inc Pins
            _pinCountINC = new Button(context);
            _pinCountINC.LayoutParameters = new LayoutParams(100, 50);
            _pinCountINC.TranslationY = 150;
            _pinCountINC.Text = "+";
            this.AddView(_pinCountINC);
            _pinCountINC.Click += delegate
            {
                SetPinCount(1);
            };

            // dec Pins
            _pinCountDEC = new Button(context);
            _pinCountDEC.LayoutParameters = new LayoutParams(100, 50);
            _pinCountDEC.TranslationX = 100;
            _pinCountDEC.TranslationY = 150;
            _pinCountDEC.Text = "-";
            this.AddView(_pinCountDEC);
            _pinCountDEC.Click += delegate
            {
                SetPinCount(-1);
            };

            //Text
            loadNamesBtn = new Button(context);
            loadNamesBtn.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, 50);
            loadNamesBtn.TranslationY = 200;
            loadNamesBtn.Text = "Load Moduls";
            loadNamesBtn.TextSize = 20;
            loadNamesBtn.SetTextColor(Color.White);
            this.AddView(loadNamesBtn);

            //scroll
            var tmp = new ScrollView(context);
            tmp.LayoutParameters = new LayoutParams(300, LayoutParams.WrapContent);
            tmp.SetBackgroundColor(Color.White);
            tmp.TranslationY = 250;
            this.AddView(tmp);

            //list
            _moduleFileNamesList = new ListView(context);
            _moduleFileNamesList.LayoutParameters = new LayoutParams(300, LayoutParams.WrapContent);
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