using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks.Comparators
{
    public class GreaterOrEqualThan : Comparator
    {
        public GreaterOrEqualThan()
        {
            Name = "Greater or Equal than";
            Text = "Größer oder gleich als";
            ToString = ">=";
        }
    }
}
