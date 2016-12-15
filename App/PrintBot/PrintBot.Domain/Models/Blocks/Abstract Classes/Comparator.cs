using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PrintBot.Domain.Models.Blocks
{
    /// <summary>
    /// Comparators: Equals, NotEquals, LessThan, LessThanOrEqual, GreaterThan, GreaterThanOrEqual, And, Or
    /// </summary>
    public abstract class Comparator
    {
        public string Name { get; set; }
        public string Text { get; set; }
        public string Code { get; set; }
        public new string ToString { get; set; }
    }
}