using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace PrintBot.Domain.Models.Blocks
{
    public class Equals : Comparator
    {
        public Equals()
        {
            Name = "Ist ungleich Comparator";
            Text = "Entspricht";
            ToString = "==";
        }
    }
}