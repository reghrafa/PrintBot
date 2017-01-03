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
using PrintBot.Droid.Controls.Blocks;
using static Android.Views.View;
using PrintBot.Droid.Controls;

namespace PrintBot.Droid
{
    public class FragmentTools : Fragment
    {
        CountingLoopToolbar countingLoopToolbar;
        EndlessLoopToolbar endlessLoopToolbar;
        IfToolbar ifToolbar;
        LEDToolbar ledToolbar;
        VariableToolbar variableToolbar;

        public override void OnCreate(Bundle savedInstanceState)
        {
            base.OnCreate(savedInstanceState);

            // Create your fragment here
        }

        public override View OnCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
        {
            var view = inflater.Inflate(Resource.Layout.fragment_tools, container, false);
            countingLoopToolbar = view.FindViewById<CountingLoopToolbar>(Resource.Id.tools_CountingLoop);
            endlessLoopToolbar = view.FindViewById<EndlessLoopToolbar>(Resource.Id.tools_EndlessLoop);
            ifToolbar = view.FindViewById<IfToolbar>(Resource.Id.tools_If);
            ledToolbar = view.FindViewById<LEDToolbar>(Resource.Id.tools_LED);
            variableToolbar = view.FindViewById<VariableToolbar>(Resource.Id.tools_Variable);

            countingLoopToolbar.LongClick += HandleClick;
            endlessLoopToolbar.LongClick += HandleClick;
            ifToolbar.LongClick += HandleClick;
            ledToolbar.LongClick += HandleClick;
            variableToolbar.LongClick += HandleClick;

            return view;
        }

        public void HandleClick(object sender, EventArgs e)
        {
            var s = (BlockListItem)sender;
            s.StartDrag(ClipData.NewPlainText("", ""), new DragShadowBuilder(s), s, 0);

            //s.StartDragAndDrop(data, new DragShadowBuilder(s), s, 0);
        }
    }
}