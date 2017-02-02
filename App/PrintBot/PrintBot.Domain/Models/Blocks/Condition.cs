using System.ComponentModel;
using System;

namespace PrintBot.Domain.Models.Blocks
{
    /// <summary>
    /// contains the variable, the comparator and the value
    /// </summary>
    public class Condition
    {
        public int Variable { get; set; }
        public int Value { get; set; }
        public Comparator Comparator { get; set; }

        public Condition()
        {
            Variable = 1;
            Value = 1;
            Comparator = new Equals();
        }

        public Condition(int variable, int value, Comparator compare)
        {
            Variable = variable;
            Value = value;
            Comparator = compare;
        }

        public new string ToString
        {
            get
            {
                return string.Format("{0} {1} {2}", Variable.ToString(), Comparator.ToString, Value.ToString());
            }
        }
    }
}