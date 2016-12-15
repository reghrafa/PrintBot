using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Util;
using Android.Views;
using Android.Widget;
using static Android.Views.View;

namespace DragAndDrop.Droid
{
    public class FragmentTools : Fragment
    {
        Button button1;
        Button button2;
        Button button3;
        Button button4;
        Button button5;
        Button button6;

        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            // Create your fragment here
        }

        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.fragment_tools, container, false);
            button1 = view.FindViewById<Button>(Resource.Id.tools_button1);
            button2 = view.FindViewById<Button>(Resource.Id.tools_button2);
            button3 = view.FindViewById<Button>(Resource.Id.tools_button3);
            button4 = view.FindViewById<Button>(Resource.Id.tools_button4);
            button5 = view.FindViewById<Button>(Resource.Id.tools_button5);
            button6 = view.FindViewById<Button>(Resource.Id.tools_button6);

            button1.LongClick += HandleClick;
            button2.LongClick += HandleClick;
            button3.LongClick += HandleClick;
            button4.LongClick += HandleClick;
            button5.LongClick += HandleClick;
            button6.LongClick += HandleClick;

            return view;

        }

        public void HandleClick(object sender, EventArgs e)
        {
            var s = (Button)sender;
            var data = ClipData.NewPlainText(s.Text, s.Text);

            s.StartDrag(data, new DragShadowBuilder(s), s, 0);

            //s.StartDragAndDrop(data, new DragShadowBuilder(s), null, 0);
        }
        private class MyTouchListener : IOnTouchListener
        {
            public IntPtr Handle
            {
                get
                {
                    return (IntPtr)null;
                }
            }

            public void Dispose()
            {

            }

            public bool OnTouch(View v, MotionEvent e)
            {
                if (e.Action == MotionEventActions.Down)
                {
                    ClipData data = ClipData.NewPlainText("Bla", "Blub");
                    DragShadowBuilder shadowBuilder = new DragShadowBuilder(v);
                    // v.StartDrag(data, shadowBuilder, v, 0);
                    v.StartDragAndDrop(data, shadowBuilder, v, 0);
                    // v.SetVisibility(View.INVISIBLE) ?
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
}