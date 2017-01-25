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
using PrintBot.Droid.Adapter;
using PrintBot.Domain.Models.Blocks.Comparators;

namespace PrintBot.Droid.Controls.Blocks
{
    public class IfListItem : IBlockHolder
    {
        public IBlock Block { get; set; }
        public View BlockLayout { get; set; }
        public Context BlockContext { get; set; }
        Spinner spinner;
        EditText editVariable;
        EditText editValue;
        private readonly List<Comparator> items = new List<Comparator> { new Equals(), new NotEquals(),
                    new LessThan(), new LessOrEqualThan(), new GreaterThan(),
                    new GreaterOrEqualThan() };
        public List<string> itemListStrings
        {
            get
            {
                var result = new List<string>();
                foreach (Comparator c in items)
                {
                    result.Add(c.ToString);
                }
                return result;
            }
        }

        public IfListItem(Context c, IBlock block)
        {
            BlockContext = c;
            Block = block;
            init();
        }

        public void init()
        {
            BlockLayout = LayoutInflater.From(BlockContext).Inflate(Resource.Layout.BlockListItem_If, null);
            // Other Layout controls
            spinner = BlockLayout.FindViewById<Spinner>(Resource.Id.BlockListItem_If_Spinner);
            var adapter = new ArrayAdapter<string>(BlockContext, Resource.Layout.Variable_Datatype_SpinnerItem, itemListStrings);
            adapter.SetDropDownViewResource(Resource.Layout.Variable_Datatype_SpinnerItem);

            spinner.Adapter = adapter;
            spinner.ItemSelected += Spinner_ItemSelected;
            editVariable = BlockLayout.FindViewById<EditText>(Resource.Id.BlockListItem_If_Variable);
            editValue = BlockLayout.FindViewById<EditText>(Resource.Id.BlockListItem_If_Value);
            editVariable.TextChanged += EditVariable_TextChanged;
            editValue.TextChanged += EditValue_TextChanged;
            editVariable.Text = ((IfBlock)Block).Condition.Variable.ToString();
            editValue.Text = ((IfBlock)Block).Condition.Value.ToString();
            var item = items.Single(a => a.Name == ((IfBlock)Block).Condition.Comparator.Name);
            var index = items.IndexOf(item);
            spinner.SetSelection(index);
        }

        private void Spinner_ItemSelected(object sender, AdapterView.ItemSelectedEventArgs e)
        {
            ((IfBlock)Block).Condition.Comparator = items[e.Position];
        }

        private void EditValue_TextChanged(object sender, Android.Text.TextChangedEventArgs e)
        {
            int result;
            bool success = int.TryParse(e.Text.ToString(), out result);
            if (success)
            {
                ((IfBlock)Block).Condition.Value = result;

            }
            else
            {
                ((IfBlock)Block).Condition.Value = 0;
            }
        }

        private void EditVariable_TextChanged(object sender, Android.Text.TextChangedEventArgs e)
        {
            int result;
            bool success = int.TryParse(e.Text.ToString(), out result);
            if (success)
            {
                ((IfBlock)Block).Condition.Variable = result;

            }
            else
            {
                ((IfBlock)Block).Condition.Variable = 0;
            }
        }
    }
}