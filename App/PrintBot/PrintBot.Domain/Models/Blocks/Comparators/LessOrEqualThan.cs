using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks.Comparators
{
    public class LessOrEqualThan : Comparator
    {
        public LessOrEqualThan()
        {
            Name = "Less or Equal than";
            Text = "Kleiner oder gleich als";
            ToString = "<=";
        }
    }
}
