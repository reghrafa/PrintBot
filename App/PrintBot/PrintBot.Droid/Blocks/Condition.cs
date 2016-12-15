using App2.Abstract_Classes;
using App2.Comparators;
using System.ComponentModel;
using System;

namespace App2
{
    /// <summary>
    /// contains the variable, the comparator and the value
    /// </summary>
    public class Condition : INotifyPropertyChanged
    {
        public int Variable { get; set; }
        public int Value { get; set; }
        public Comparator Comparator { get; set; }
        public event PropertyChangedEventHandler PropertyChanged;

        public Condition()
        {
            Variable = 1;
            Value = 1;
            Comparator = new Equals();
            ToString = string.Format("{0} {1} {2}", Variable.ToString(), Comparator.ToString, Value.ToString());
            //SetCodeText();
            //PropertyChanged += Condition_PropertyChanged;
        }

        public Condition(int variable, int value, Comparator compare)
        {
            Variable = variable;
            Value = value;
            Comparator = compare;
            SetCodeText();
            PropertyChanged += Condition_PropertyChanged;
        }

        private void Condition_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            SetCodeText();
        }

        private void SetCodeText()
        {
            ToString = string.Format("{0} {1} {2}", Variable.ToString(), Comparator.ToString, Value.ToString());
        }

        public new string ToString
        {
            get
            {
                return ToString;
            }
            set
            {
                string.Format("{0} {1} {2}", Variable.ToString(), Comparator.ToString, Value.ToString());
            }
        }
    }
}