using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models.Blocks
{
    public class NotEquals : Comparator
    {
        public NotEquals()
        {
            Name = "Ist gleich Komparator";
            Text = "ungleich";
            ToString = "!=";
        }
    }
}