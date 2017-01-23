using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PrintBot.Domain.Models.Blocks.Comparators
{
    public class GreaterThan : Comparator
    {
        public GreaterThan()
        {
            Name = "Greater Than";
            Text = "Größer als";
            ToString = ">";
        }
    }
}
