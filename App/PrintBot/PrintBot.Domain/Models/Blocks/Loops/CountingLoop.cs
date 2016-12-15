using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace PrintBot.Domain.Models.Blocks
{
    public class CountingLoop : Loop
    {
        public int AmountOfLoops { get; set; }

        public CountingLoop()
        {
            Name = "Counting Loop";
            Text = "Zählschleife";
            AmountOfLoops = 5;
            CodeBody = "";
            SetCodeText("for (i = 0; i < " + AmountOfLoops.ToString() + "; i++) {");
        }
    }
}