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
using PrintBot.Domain.Models.Blocks;
using PrintBot.Droid.Services;

namespace PrintBot.Droid.Controls.Blocks
{
    public class VariableListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        Spinner spinner;

        public VariableListItem(Context c, IBlock block)
        {
            BlockContext = c;
            Block = block;
            init();
        }

        public void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_Variable, null);
            // Other Layout controls
            spinner = BlockLayout.FindViewById<Spinner>(Resource.Id.BlockListItem_Variable_Spinner);
            var itemList = BlockContext.Resources.GetStringArray(Resource.Array.variable_datatypes);
            var adapter = new ArrayAdapter<string>(BlockContext, Resource.Layout.Variable_Datatype_SpinnerItem, itemList);
            adapter.SetDropDownViewResource(Resource.Layout.Variable_Datatype_SpinnerItem);
            spinner.Adapter = adapter;
        }
    }
}