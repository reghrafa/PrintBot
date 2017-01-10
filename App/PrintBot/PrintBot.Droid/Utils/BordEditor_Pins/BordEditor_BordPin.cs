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
using Android.Util;
using static Android.App.ActionBar;
using Android.Graphics;

namespace PrintBot.Droid
{
    class BordEditor_BordPin : Button
    {
        int nr;
        public BordEditor_PaintView pw;
        public BordEditor_Bord parent;
        private string _type;  // to be set in child pin classes
        public Color BackgroundColor { get; set; } // to be set in child pin classes
        public BordEditor_Modul.ModulButton ConectetPin { get; set; }

        public int Nr
        {
            get
            {
                return nr;
            }

            set
            {
                nr = value;
            }
        }

        public string Type
        {
            get
            {
                return _type;
            }

            set
            {
                _type = value;
            }
        }

        public BordEditor_BordPin(Context context) : base(context, null, 0)
        {
            pw = new BordEditor_PaintView(context);
            pw.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, LayoutParams.MatchParent);
            
        }
        public BordEditor_BordPin(Context context, IAttributeSet attrs) : base(context, attrs)
        {
            pw = new BordEditor_PaintView(context);
            pw.LayoutParameters = new LayoutParams(LayoutParams.MatchParent, LayoutParams.MatchParent);
        }

        public override float GetX()
        {
            return base.GetX() + Width / 2 + parent.GetX();
        }

        public override float GetY()
        { 
            return base.GetY() + Height / 2 + parent.GetY();
        }

    }
}