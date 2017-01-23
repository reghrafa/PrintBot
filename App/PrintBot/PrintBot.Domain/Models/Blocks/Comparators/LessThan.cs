using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks.Comparators
{
    public class LessThan : Comparator
    {
        public LessThan()
        {
            Name = "Less than";
            Text = "Kleiner als";
            ToString = "<";
        }
    }
}
