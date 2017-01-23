using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace PrintBot.Domain.Models.Blocks
{
    public class ConditionLoop : Loop
    {
        public Condition Condition { get; set; }

        public ConditionLoop()
        {
            Name = "Condition Loop";
            Text = "Schleife bis";
            Condition = new Condition();
        }

    }
}